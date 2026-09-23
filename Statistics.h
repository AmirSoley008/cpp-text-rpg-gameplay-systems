//
// Created by parsian on 9/20/2026.
//

#ifndef UNTITLED3_STATISTICS_H
#define UNTITLED3_STATISTICS_H

#include <unordered_map>
#include <vector>
#include "CharacterKilled.h"

class Character;

class Statistics {
private:
    std::unordered_map<const Character*,int>countList;

    std::unordered_map<const Character*,std::vector<Character*>>killedList;
public:
    void registerKill(const CharacterKilled& event);

    void reset();
};


#endif //UNTITLED3_STATISTICS_H
