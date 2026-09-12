//
// Created by parsian on 7/1/2026.
//

#include <iostream>
#include "Skill.h"
#include "Character.h"

bool Skill::use(Character& caster, Character& target) {
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
    if (skillType == SkillType::damage) {
        target.takeDamage(skillStats.damage);
        caster.useMana(skillStats.manaCost);
        currentCoolDown = skillStats.coolDown;
    } else if (skillType == SkillType::heal) {
        target.heal(skillStats.healAmount);
        caster.useMana(skillStats.manaCost);
        currentCoolDown = skillStats.coolDown;
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