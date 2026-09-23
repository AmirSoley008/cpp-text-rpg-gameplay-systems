//
// Created by parsian on 9/23/2026.
//

#ifndef UNTITLED3_KILLREWARDER_H
#define UNTITLED3_KILLREWARDER_H

#include "Character.h"
#include "CharacterKilled.h"

class KillRewarder {
public:
    void onCharacterKilled(const CharacterKilled& event);
};


#endif //UNTITLED3_KILLREWARDER_H
