//
// Created by parsian on 6/29/2026.
//

#include <iostream>
#include "Mage.h"

Mage::Mage(const std::string &name)
    : Character(name, CharacterClass::Mage)
{
}

std::string Mage::getAttackMessage() const
{
    return " Here comes FIREBALLS!!";
}

