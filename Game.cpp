//
// Created by parsian on 7/21/2026.
//

#include <iostream>
#include "Game.h"
#include "Character.h"
#include "Skill.h"

void Game::start() {
    std::cout << "Round " << round << " Starts" << std::endl;
    while (gameContinue){
        if (characters.size() == 1){
            std::cout << characters[0]->getName() << " has won the game " << std::endl;
            break;
        } else {
            while (true){
                action = chooseAction();
                if (action == ActionType::Attack) {
                    int targetIndex = chooseTarget();
                    currentCharacter().attack(*characters[targetIndex]);
                    if (!characters[targetIndex]->isAlive()) characters.erase(characters.begin() + targetIndex);
                    break;
                }
                if (action == ActionType::Fireball) {
                    int targetIndex = chooseTarget();
                    if (currentCharacter().useSkill(0 ,*characters[targetIndex])) {
                        if (!characters[targetIndex]->isAlive()) characters.erase(characters.begin() + targetIndex);
                        break;
                    } else std::cout << "choose Action again!" << std::endl;
                }
                if (action == ActionType::Heal) {
                    int targetIndex = chooseTarget();
                    if (currentCharacter().useSkill(1, *characters[targetIndex])) {
                        if (!characters[targetIndex]->isAlive()) characters.erase(characters.begin() + targetIndex);
                        break;
                    } else {
                        std::cout << "choose Action again!" << std::endl;
                    }
                }
            } activeCharacterIndex ++;
            if (activeCharacterIndex >= characters.size()){
                activeCharacterIndex = 0;
                round ++;
                std::cout << "Round " << round << " Starts" << std::endl;
            }
        }

    }
}


void Game::characterAdder() {
    characters.push_back(std::make_unique<Warrior>("geralt"));
    characters.push_back(std::make_unique<Mage>("yen"));
    characters.push_back(std::make_unique<Archer>("legolas"));
}

int Game::chooseTarget() {
    if (activeCharacterIndex == 0) {
        while (true) {
            std::cout << "Who is your target?" << std::endl;
            if (action == ActionType::Heal) {
                for (int i = 0; i <= characters.size() - 1; ++i) {
                    std::cout << i << "." << characters[i]->getName() << std::endl;
                }
                int choose;
                std::cin >> choose;
                if (choose >= 0 && choose <= characters.size() - 1) return choose;
                else std::cout << "chosen number is not valid!" << std::endl;
            } else{
                for (int i = 1; i <= characters.size() - 1; ++i) {
                    std::cout << i << "." << characters[i]->getName() << std::endl;
                }
                int choose;
                std::cin >> choose;
                if (choose >= 1 && choose <= characters.size() - 1) return choose;
                else std::cout << "chosen number is not valid!" << std::endl;
            }
        }
    }else {
        int choose;
        if (action == ActionType::Heal) return choose = activeCharacterIndex;
        else {
            choose = rand() % characters.size();
            while (choose == activeCharacterIndex) choose = rand() % characters.size();
            return choose;
        }
    }
}

ActionType Game::chooseAction() {
    if (activeCharacterIndex == 0) {
        while (true) {
            std::cout << "what action do you want to perform?"
                      << std::endl
                      << "1.Attack!!"
                      << std::endl
                      << "2.Fireball"
                      << std::endl
                      << "3.Heal"
                      << std::endl;
            int chooseAct;
            std::cin >> chooseAct;
            switch (chooseAct) {
                case 1:
                    return ActionType::Attack;
                case 2:
                    return ActionType::Fireball;
                case 3:
                    return ActionType::Heal;
                default:
                    std::cout << "Invalid number is chosen please retry noobie" << std::endl;
            }
        }
    } else{
        if (currentCharacter().getSkill(1).isReady() && currentCharacter().getHealth() <= 50) return ActionType::Heal;
        else if (currentCharacter().getSkill(0).isReady() && currentCharacter().hasEnoughMana(currentCharacter().getSkill(0).getManaCost())) return ActionType::Fireball;
        else return ActionType::Attack;
    }
}

Character &Game::currentCharacter() {
    return *characters[activeCharacterIndex];
}