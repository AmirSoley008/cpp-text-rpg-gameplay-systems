//
// Created by parsian on 10/3/2026.
//

#include "Bulky.h"
#include <iostream>

Bulky::Bulky(const std::string &name)
    : Character(name, CharacterClass::Bulky)
{
}

std::string Bulky::getAttackMessage() const {
    return " : now you CAN'T see me!!";
}