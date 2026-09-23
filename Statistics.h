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
    std::unordered_map<std::string ,int>countList;

    std::unordered_map<std::string ,std::vector<std::string>>killedList;
public:
    void registerKill(const CharacterKilled& event);

    void reset();

    void showSummary();
};


#endif //UNTITLED3_STATISTICS_H
