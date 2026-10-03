//
// Created by parsian on 6/29/2026.
//

#ifndef UNTITLED3_ARCHER_H
#define UNTITLED3_ARCHER_H


#include "Character.h"

class Archer : public Character {
public:
    Archer(const std::string& name);

    std::string getAttackMessage() const override;
};


#endif //UNTITLED3_ARCHER_H
