//
// Created by parsian on 6/28/2026.
//

#include <iostream>
#include "Warrior.h"
#include "Character.h"


Warrior::Warrior(const std::string &name)
    : Character(name,CharacterClass::Warrior)
{
}

std::string Warrior::getAttackMessage() const
{
    return " swings his SWORD!!";
}

