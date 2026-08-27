//
// Created by parsian on 6/29/2026.
//

#ifndef UNTITLED3_MAGE_H
#define UNTITLED3_MAGE_H


#include "Character.h"

class Mage : public Character{
public:
    Mage(const std::string& name);

    std::string getAttackMessage() const override;

    //void attack(Character& target)override;

    //void takeDamage(int damage)override;


};


#endif //UNTITLED3_MAGE_H
