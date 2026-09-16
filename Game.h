//
// Created by parsian on 7/21/2026.
//

#ifndef UNTITLED3_GAME_H
#define UNTITLED3_GAME_H

#include "Character.h"
#include "Mage.h"
#include "Warrior.h"
#include "Archer.h"
#include "ActionType.h"
#include "ActionExecutor.h"

enum class PostGameChoice {
    Rematch,
    Exit
};

class Game {
private:
    int activeCharacterIndex = 0;

    int round = 1;

    bool gameContinue = true;

    std::vector<std::unique_ptr<Character>> characters;

    ActionExecutor executor;

    Character* playerCharacter = nullptr;
public:
    ActionType action;

    Game(){
        characterAdder();
        skillAdder();
    }

    void start();

    void characterAdder();

    void skillAdder();

    int chooseTarget();

    ActionType chooseAction();

    Character& currentCharacter();

    void selectPlayerCharacter();

    void resetMatch();

    PostGameChoice postGameChoice();
};


#endif //UNTITLED3_GAME_H
