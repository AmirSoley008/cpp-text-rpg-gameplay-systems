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
    skillDefinitions.emplace_back("Heavy Slash",std::vector<Operation>{{DamageData{60}}}, 25, 3);
    skillDefinitions.emplace_back("Poison Slash",std::vector<Operation>{{DamageData{20}},{PersistentDamageData{10,3}}},15,2);
    skillDefinitions.emplace_back("Rest And Peace",std::vector<Operation>{{HealData{50}}}, 15, 3);
    skillDefinitions.emplace_back("One Bite Each Time",std::vector<Operation>{{PersistentHealData{10,3}}}, 10, 3);
    skillDefinitions.emplace_back("Fireball",std::vector<Operation>{{DamageData{45}}}, 15, 3);
    skillDefinitions.emplace_back("Burning Fury",std::vector<Operation>{{DamageData{20}},{PersistentDamageData{10,2}}},10,3);
    skillDefinitions.emplace_back("Idle Evil Spirits",std::vector<Operation>{{PersistentDamageData{8,5}}}, 10, 2);
    skillDefinitions.emplace_back("potion",std::vector<Operation>{{HealData{40}},{PersistentHealData{10,2}}}, 20, 3);
    skillDefinitions.emplace_back("Healing magic",std::vector<Operation>{{PersistentHealData{10,4}}}, 15, 2);
    skillDefinitions.emplace_back("Leech Spirit",std::vector<Operation>{{PersistentDamageData{10,3}},{PersistentHealData{10,3}}},15,2);
    skillDefinitions.emplace_back("Rapid Arrows",std::vector<Operation>{{DamageData{8}},{DamageData{8}},{DamageData{8}}}, 5, 1);
    skillDefinitions.emplace_back("Focus shot",std::vector<Operation>{{DamageData{35}}}, 10, 3);
    skillDefinitions.emplace_back("Poison shot",std::vector<Operation>{{DamageData{5}},{PersistentDamageData{10,3}}},8,2);
    skillDefinitions.emplace_back("Burning Wound",std::vector<Operation>{{PersistentDamageData{10,3}}}, 7, 1);
    skillDefinitions.emplace_back("Medicinal plant of the Elves' forest",std::vector<Operation>{{HealData{40}}}, 10, 3);
    skillDefinitions.emplace_back("death refuser",std::vector<Operation>{{PersistentHealData{5,3}},{PersistentHealData{5,2}},{PersistentHealData{5,1}}}, 10, 2);
    skillDefinitions.emplace_back("Rain of Arrows",std::vector<Operation>{{PersistentDamageData{5,3}},{PersistentDamageData{5,2}},{PersistentDamageData{5,1}}}, 8, 1);
    skillDefinitions.emplace_back("Heavy Punch",std::vector<Operation>{{DamageData{60}}}, 15, 3);
    skillDefinitions.emplace_back("Tyson mod",std::vector<Operation>{{DamageData{15}},{DamageData{15}},{DamageData{15}},{PersistentDamageData{5,1}}}, 20, 4);
    skillDefinitions.emplace_back("Boneacher",std::vector<Operation>{{DamageData{10}},{PersistentDamageData{15,3}}},20,3);
    skillDefinitions.emplace_back("Timeout!!",std::vector<Operation>{{HealData{60}}}, 20, 3);
    skillDefinitions.emplace_back("Blood Drinker",std::vector<Operation>{{DamageData{30}},{HealData{30}}}, 15, 2);
    skillDefinitions.emplace_back("Thirst of the Damned",std::vector<Operation>{{DamageData{15}},{PersistentHealData{15,3}}}, 12, 2);
    skillDefinitions.emplace_back("Sanguine Curse",std::vector<Operation>{{DamageData{10}},{PersistentDamageData{5,3}},{PersistentHealData{5,3}},{PersistentDamageData{5,4}},{PersistentHealData{5,4}},{PersistentDamageData{5,5}},{PersistentHealData{5,5}}}, 20, 2);
    skillDefinitions.emplace_back("Fang of Renewal",std::vector<Operation>{{DamageData{5}},{HealData{10}},{PersistentHealData{10,3}}}, 10, 2);
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

void Skill::printSkillsInfo() const {
    std::cout << name << " -> ";

    for (const auto& operation : operations) {
        std::visit([&](const auto& data){

            using T = std::decay_t<decltype(data)>;

            if constexpr (std::is_same_v<T, DamageData>){
                std::cout << "Damage: "<< std::get_if<DamageData>(&operation.data)->amount << " ";
            }
            else if constexpr (std::is_same_v<T, HealData>){
                std::cout << "Heal: "<< std::get_if<HealData>(&operation.data)->amount << " ";
            }
            else if constexpr (std::is_same_v<T, PersistentDamageData>){
                std::cout << "Damage: "
                          << std::get_if<PersistentDamageData>(&operation.data)->damagePerRound
                          << "*"
                          << std::get_if<PersistentDamageData>(&operation.data)->duration
                          << " round ";
            }
            else if constexpr (std::is_same_v<T, PersistentHealData>){
                std::cout << "Heal: "
                          << std::get_if<PersistentHealData>(&operation.data)->healPerRound
                          << "*"
                          << std::get_if<PersistentHealData>(&operation.data)->duration
                          << " round ";
            }
        }, operation.data);
    }
}