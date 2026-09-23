//
// Created by parsian on 9/23/2026.
//

#ifndef UNTITLED3_BLOODLUST_H
#define UNTITLED3_BLOODLUST_H

#include <unordered_map>
#include "CharacterKilled.h"

class Bloodlust {
private:
    std::unordered_map<const Character*,int>countList;

public:
    void onCharacterKilled(const CharacterKilled& event);

    void reset();
};


#endif //UNTITLED3_BLOODLUST_H
