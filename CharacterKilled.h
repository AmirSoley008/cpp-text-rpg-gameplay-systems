//
// Created by parsian on 9/20/2026.
//

#ifndef UNTITLED3_CHARACTERKILLED_H
#define UNTITLED3_CHARACTERKILLED_H

class Character;

struct CharacterKilled{
    Character* killer;
    Character* victim;
    CharacterKilled(Character* killer, Character* victim)
    : killer(killer), victim(victim) {}
};

#endif //UNTITLED3_CHARACTERKILLED_H