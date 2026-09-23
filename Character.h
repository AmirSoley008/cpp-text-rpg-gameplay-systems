//
// Created by parsian on 6/27/2026.
//

#ifndef UNTITLED3_CHARACTER_H
#define UNTITLED3_CHARACTER_H

#include <string>
#include <memory>
#include <vector>
#include "Skill.h"

struct Stats{
    int health;
    int maxHealth;

    int mana;
    int maxMana;

    int damage;

    Stats(int h, int mh, int m, int mm, int d)
        :health(h),
        maxHealth(mh),
        mana(m),
        maxMana(mm),
        damage(d){}
};

enum class CharacterClass {
    Warrior,
    Mage,
    Archer
};

class Character{
protected:
    std::string name;

    CharacterClass playerClass;

    Stats stats;

    std::vector<Skill>skills;

public:
    Character(const std::string &name, CharacterClass PlayeClass)
            : name(name),
              playerClass(PlayeClass),
              stats(getDefaultStats(playerClass))
    {
    }


    void attack(Character& target);

    void takeDamage(int damage);

    void useMana(int manaCost);

    virtual std::string getAttackMessage() const = 0;

    void heal(int amout);

    void addSkill(const Skill& skil);

    bool useSkill(int index , Character& target);

    static Stats getDefaultStats(CharacterClass characterClass);

    void printInfo();

    const std::string& getName() const;

    int getHealth() const;

    int getMaxHealth() const;

    bool isAlive() const;

    bool hasEnoughMana(int amount) const;

    Skill& getSkill(int index);

    void onRoundEnd();

    void addMana(int amount);
};



#endif //UNTITLED3_CHARACTER_H
