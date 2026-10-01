//
// Created by parsian on 6/27/2026.
//

#include <iostream>
#include <thread>
#include <chrono>
#include "Character.h"
#include "Skill.h"


void Character::heal(int amout) {
    if (stats.health < stats.maxHealth) {
        stats.health += amout;
        if (stats.health > stats.maxHealth){
            stats.health = stats.maxHealth;}
    }
    else std::cout<<"heath is full!"<<std::endl;
}

void Character::addMana(int amount) {
    if (stats.mana < stats.maxMana) {
        stats.mana += amount;
        if (stats.mana > stats.maxMana){
            stats.mana = stats.maxMana;}
    }
    else std::cout<<"mana is full!"<<std::endl;
}

bool Character::isAlive() const {
    return stats.health > 0;
}

int Character::getHealth() const {
    return stats.health;
}

int Character::getMaxHealth() const {
    return stats.maxHealth;
}

const std::string& Character::getName() const{
    return name;
}

void Character::takeDamage(int damage) {
    stats.health -= damage;
    if (stats.health < 0) stats.health = 0;
}

void Character::useMana(int manaCost) {
    stats.mana -= manaCost;
}

void Character::attack(Character &target) {
    std::cout << name << this->getAttackMessage() << std::endl;
    target.takeDamage(stats.damage);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout <<name
              << " attacks "
              <<target.getName()
              <<std::endl
              <<target.getName()
              <<" "
              <<target.getHealth()
              <<"\\"
              <<target.getMaxHealth()<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (!target.isAlive()) std::cout<< target.getName() <<" is dead!"<<std::endl;
}

Stats Character::getDefaultStats(CharacterClass characterClass) {
    if (characterClass == CharacterClass::Warrior)return {250 , 350, 30, 35, 50};
    if (characterClass == CharacterClass::Mage)return {200 , 250, 60, 65, 30};
    if (characterClass == CharacterClass::Archer)return {200 , 250, 40, 45, 40};
    return {0,0,0,0,0};
}

void Character::printInfo() {
    switch(playerClass)
    {
        case CharacterClass::Warrior:
            std::cout << "Warrior"<<std::endl;
            break;

        case CharacterClass::Mage:
            std::cout << "Mage"<<std::endl;
            break;

        case CharacterClass::Archer:
            std::cout << "Archer"<<std::endl;
            break;
    }
    std::cout <<"name is "
              <<name
              <<std::endl
              <<"health is "
              <<stats.health
              <<std::endl
              <<"mana is "
              <<stats.mana
              <<std::endl
              <<"damage is "
              <<stats.damage
              <<std::endl
              << std::endl ;
}

void Character::addSkill(const Skill& skil) {
    if (!this->isAlive()) {
        std::cout<<"Error "<< this->getName() <<" is dead"<<std::endl;
    }else skills.push_back(skil);
}

bool Character::useSkill(int index, Character &target, EffectRegistrar& effectRegistrar) {
    if (index >=0 && index < skills.size()) {
        Skill &skill = skills[index];
        std::cout << this->getName() << " wants to use " << skill.getSkillName() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (skill.use(*this, target, effectRegistrar)) {
            std::cout << this->getName() << " used a " << skill.getSkillName() << " on " << target.getName()
                      << std::endl;
            std::cout << target.getName()
                      << " "
                      << target.getHealth()
                      << "\\"
                      << target.getMaxHealth() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return true;
        } else return false;
    } else return false;
}

bool Character::hasEnoughMana(int amount) const {
    return stats.mana >= amount;
}

Skill &Character::getSkill(int index) {
    return skills[index];
}

void Character::onRoundEnd() {
    for (auto &skill : skills) {
        skill.tickCooldown();
    }
}

const std::vector<Skill>& Character::getSkillsList() const{
    return skills;
}

int Character::getDamageAmount() {
    return stats.damage;
}