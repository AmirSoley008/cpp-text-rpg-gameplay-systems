//
// Created by parsian on 9/20/2026.
//


#include "Statistics.h"

void Statistics::registerKill(const CharacterKilled& event) {
    killedList[event.killer].push_back(event.victim);

    countList[event.killer] += 1;
}

void Statistics::reset() {
    killedList.clear();
    countList.clear();
}