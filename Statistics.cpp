//
// Created by parsian on 9/20/2026.
//


#include <iostream>
#include "Statistics.h"
#include "Character.h"

void Statistics::registerKill(const CharacterKilled& event) {
    killedList[event.killer->getName()].push_back(event.victim->getName());

    countList[event.killer->getName()] += 1;
}

void Statistics::showSummary() {
    for (auto name:countList) {
        std::cout << "----------------------------------------"
                  << std::endl
                  << name.first
                  << std::endl
                  << "kills : "
                  << name.second
                  << std::endl;
        auto itarator = killedList.find(name.first);
        if (itarator != killedList.end()){
            std::cout << "Kill History :"
                      << std::endl;
            for (auto victimName:itarator->second) {
                std::cout << victimName
                          << std::endl;
            }
        }
    }
}

void Statistics::reset() {
    killedList.clear();
    countList.clear();
}