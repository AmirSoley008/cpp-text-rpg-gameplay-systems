//
// Created by parsian on 7/1/2026.
//

#include <iostream>
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
                target.heal(data.amount);
            }
            else if constexpr (std::is_same_v<T, PersistentDamageData>){
                effectRegistrar.registerEffect(std::make_unique<Effect>(&caster,&target,data));
            }
            else if constexpr (std::is_same_v<T, PersistentHealData>){
                effectRegistrar.registerEffect(std::make_unique<Effect>(&caster,&target,data));
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