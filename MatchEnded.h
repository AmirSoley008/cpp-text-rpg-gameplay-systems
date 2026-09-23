//
// Created by parsian on 9/23/2026.
//

#ifndef UNTITLED3_MATCHENDED_H
#define UNTITLED3_MATCHENDED_H

class Character;

struct MatchEnded{
    Character* winner;
    MatchEnded(Character* winner)
    : winner(winner) {}
};

#endif //UNTITLED3_MATCHENDED_H
