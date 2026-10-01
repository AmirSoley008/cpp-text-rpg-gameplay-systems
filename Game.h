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
#include "MatchEnded.h"
#include "EffectSystem.h"
#include <random>

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

    std::vector<std::unique_ptr<Character>> deadCharacters;

    ActionExecutor executor;

    EventBus eventBus;

    Statistics statistics;

    KillRewarder killRewarder;

    Bloodlust bloodlust;

    EffectSystem effectSystem;

    Character* playerCharacter = nullptr;

    std::mt19937 generator{std::random_device{}()};
public:
    ActionType action;

    int selectedSkillIndex = -1;

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
        eventBus.subscribe<CharacterKilled>(
                [&](const CharacterKilled& event)
                {
                    effectSystem.deleteDeadTargetEffect(event);
                }
        );
        eventBus.subscribe<MatchEnded>(
                [&](const MatchEnded& event)
                {
                    statistics.showSummary();
                }
                );
        eventBus.subscribe<MatchEnded>(
                [&](const MatchEnded& event)
                {
                    std::cout << std::endl
                              << event.winner->getName()
                              << " has won the game "
                              << std::endl;
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

    int getRandomEnemyIndex();
};


#endif //UNTITLED3_GAME_H
