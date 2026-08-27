//
// Created by parsian on 7/21/2026.
//

#ifndef UNTITLED3_GAME_H
#define UNTITLED3_GAME_H

#include "Character.h"
#include "Mage.h"
#include "Warrior.h"
#include "Archer.h"

enum class ActionType {
    Attack,
    Fireball,
    Heal
};

class Game {
private:
    int activeCharacterIndex = 0;

    int round = 1;

    bool gameContinue = true;

    std::vector<std::unique_ptr<Character>> characters;

public:
    ActionType action;

    Game(){
        characterAdder();
        Skill fireball("Fireball", 10, 80, 2,10, SkillType::damage);
        Skill heal("Heal", 20, 0, 1,50, SkillType::heal);
        characters[0]->addSkill(fireball);
        characters[0]->addSkill(heal);
        characters[1]->addSkill(fireball);
        characters[1]->addSkill(heal);
        characters[2]->addSkill(fireball);
        characters[2]->addSkill(heal);
    }

    void start();

    void characterAdder();

    int chooseTarget();

    ActionType chooseAction();

    Character& currentCharacter();
};


#endif //UNTITLED3_GAME_H
