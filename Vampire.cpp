//
// Created by parsian on 10/3/2026.
//

#include "Vampire.h"
#include <iostream>

Vampire::Vampire(const std::string &name)
        : Character(name, CharacterClass::Vampire)
{
}

std::string Vampire::getAttackMessage() const {
    return " : I want your blood!";
}