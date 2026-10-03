//
// Created by parsian on 10/3/2026.
//

#ifndef UNTITLED3_VAMPIRE_H
#define UNTITLED3_VAMPIRE_H

#include "Character.h"

class Vampire : public Character {
public:
    Vampire(const std::string& name);

    std::string getAttackMessage() const override;
};


#endif //UNTITLED3_VAMPIRE_H
