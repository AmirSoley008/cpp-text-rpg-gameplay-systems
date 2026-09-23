//
// Created by parsian on 9/23/2026.
//

#include <iostream>
#include "KillRewarder.h"

void KillRewarder::onCharacterKilled(const CharacterKilled &event) {
    event.killer->addMana(10);
    std::cout << "10 mana reward for "
              << event.killer->getName()
              << std::endl;
}