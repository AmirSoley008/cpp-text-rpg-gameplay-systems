//
// Created by parsian on 9/23/2026.
//

#include <iostream>
#include "Bloodlust.h"
#include "Character.h"

void Bloodlust::onCharacterKilled(const CharacterKilled &event) {
    countList[event.killer] += 1;
    if (countList.find(event.killer)->second == 3){
        event.killer->addMana(20);
        event.killer->heal(30);
        std::cout << "20 mana and 30 Health reward for "
                  << event.killer->getName()
                  << std::endl;
        countList[event.killer] = 0;
    }
}

void Bloodlust::reset() {
    countList.clear();
}