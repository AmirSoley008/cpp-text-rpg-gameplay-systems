//
// Created by parsian on 6/28/2026.
//

#ifndef UNTITLED3_WARRIOR_H
#define UNTITLED3_WARRIOR_H


#include "Character.h"

class Warrior : public Character{
public:
    Warrior(const std::string& name);

    std::string getAttackMessage() const override;

    //void attack(Character& target)override;

    //void takeDamage(int damage)override;

};


#endif //UNTITLED3_WARRIOR_H
