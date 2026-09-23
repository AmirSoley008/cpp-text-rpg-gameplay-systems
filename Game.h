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
#include "EventBus.h"
#include "Statistics.h"
#include "KillRewarder.h"
#include "Bloodlust.h"

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

    EventBus eventBus;

    Statistics statistics;

    KillRewarder killRewarder;

    Bloodlust bloodlust;

    Character* playerCharacter = nullptr;
public:
    ActionType action;

    Game(){
        characterAdder();
        skillAdder();
        eventBus.subscribe<CharacterKilled>(
                [&](const CharacterKilled& event)
                {
                    statistics.registerKill(event);
                }
        );
        eventBus.subscribe<CharacterKilled>(
                [&](const CharacterKilled& event)
                {
                    killRewarder.onCharacterKilled(event);
                }
        );
        eventBus.subscribe<CharacterKilled>(
                [&](const CharacterKilled& event)
                {
                    bloodlust.onCharacterKilled(event);
                }
        );
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
