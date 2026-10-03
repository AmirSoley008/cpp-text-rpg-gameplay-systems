//
// Created by parsian on 7/1/2026.
//

#include <iostream>
#include <cassert>
#include "Skill.h"
#include "Character.h"
#include "Effect.h"

bool Skill::use(Character& caster, Character& target, EffectRegistrar& effectRegistrar) {
    if (!caster.isAlive()) {
        std::cout<<"Error "<< caster.getName() <<" is dead"<<std::endl;
        return false;
    }
    if (!target.isAlive()) {
        std::cout<<"Error "<< target.getName() <<" is dead"<<std::endl;
        return false;
    }
    if (!caster.hasEnoughMana(getManaCost())) {
        std::cout << "not enough mana" << std::endl;
        return false;
    }
    if (!isReady()) {
        std::cout<< getSkillName() <<" cooldown is not ready yet! wait"<<std::endl;
        return false;
    }
    caster.useMana(skillStats.manaCost);
    currentCoolDown = skillStats.coolDown;
    for (const auto& operation : operations) {
        std::visit([&](const auto& data){

            using T = std::decay_t<decltype(data)>;

            if constexpr (std::is_same_v<T, DamageData>){
                target.takeDamage(data.amount);
            }
            else if constexpr (std::is_same_v<T, HealData>){
                caster.heal(data.amount);
            }
            else if constexpr (std::is_same_v<T, PersistentDamageData>){
                effectRegistrar.registerEffect(std::make_unique<Effect>(&caster,&target,data));
            }
            else if constexpr (std::is_same_v<T, PersistentHealData>){
                effectRegistrar.registerEffect(std::make_unique<Effect>(&caster,&caster,data));
            }
            }, operation.data);
    }return true;
}

const std::string& Skill::getSkillName() const{
    return name;
}

int Skill::getManaCost() const{
    return this->skillStats.manaCost;
}

bool Skill::isReady() const {
    return currentCoolDown == 0;
}

void Skill::tickCooldown() {
    if (currentCoolDown > 0) currentCoolDown--;
}

bool Skill::providesHealing() const {
    for (const auto& operation : operations) {
        if (std::visit([](const auto& data){
            using T = std::decay_t<decltype(data)>;

            return std::is_same_v<T, HealData> || std::is_same_v<T, PersistentHealData>;

        },operation.data)){
            return true;
        }
    }return false;
}

bool Skill::providesDamaging() const {
    for (const auto& operation : operations) {
        if (std::visit([](const auto& data){
            using T = std::decay_t<decltype(data)>;

            return std::is_same_v<T, DamageData> || std::is_same_v<T, PersistentDamageData>;

        },operation.data)){
            return true;
        }
    }return false;
}

std::vector<Skill> Skill::skillDefinitions;

void Skill::skillCreator() {
    if (!skillDefinitions.empty()){
        return;
    }
    skillDefinitions.emplace_back("Fireball",std::vector<Operation>{{DamageData{70}}}, 20, 2);
    skillDefinitions.emplace_back("Heal",std::vector<Operation>{{HealData{50}}}, 10, 1);
    skillDefinitions.emplace_back("PoisonStrike",std::vector<Operation>{{PersistentDamageData{10,3}}}, 15, 3);
    skillDefinitions.emplace_back("Regeneration",std::vector<Operation>{{PersistentHealData{10,2}}}, 15, 2);
    skillDefinitions.emplace_back("VenomStrike",std::vector<Operation>{{DamageData{40}},{PersistentDamageData{5,2}}},20,2);
    skillDefinitions.emplace_back("VampireStrike",std::vector<Operation>{{DamageData{40}},{HealData{20}}},25,3);
}

Skill& Skill::findSkill(const std::string &name) {
    for (auto& skill:skillDefinitions) {
        if (skill.getSkillName() == name){
            return skill;
        }
    }
    assert(false && "Skill not found");
    std::abort();
}

std::optional<int> Skill::getSkillDamage() const{
    for (const auto& operation: operations){
        if (const auto& damage = std::get_if<DamageData>(&operation.data)){
            return damage->amount;
        }
    }
    return std::nullopt;
}