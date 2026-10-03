//
// Created by parsian on 10/3/2026.
//

#ifndef UNTITLED3_BULKY_H
#define UNTITLED3_BULKY_H

#include "Character.h"

class Bulky : public Character {
public:
    Bulky(const std::string& name);

    std::string getAttackMessage() const override;
};



#endif //UNTITLED3_BULKY_H
