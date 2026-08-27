//
// Created by parsian on 6/29/2026.
//

#include <iostream>
#include "Archer.h"

Archer::Archer(const std::string &name)
    : Character(name,CharacterClass::Archer)
{
}

std::string Archer::getAttackMessage() const
{
    return " Takes out his ARROWS!!";
}
