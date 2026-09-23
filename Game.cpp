//
// Created by parsian on 7/21/2026.
//

#include <iostream>
#include "Game.h"
#include "Character.h"
#include "Skill.h"
#include <limits>

void Game::start() {
    std::cout << "Round " << round << " Starts" << std::endl;
    selectPlayerCharacter();
    while (gameContinue){
        while (true){
            action = chooseAction();
            int targetIndex = chooseTarget();
            if (executor.execute(action, currentCharacter(), *characters[targetIndex]) == Result::Success){
                if (!characters[targetIndex]->isAlive()) {
                    eventBus.publish(CharacterKilled(&currentCharacter() , &*characters[targetIndex]));
                    characters.erase(characters.begin() + targetIndex);
                }
                break;
            } else std::cout << "choose Action again!" << std::endl;
        } if (characters.size() == 1){
            std::cout << characters[0]->getName() << " has won the game " << std::endl;
            PostGameChoice choice = postGameChoice();
            if (choice == PostGameChoice::Rematch){
                resetMatch();
                selectPlayerCharacter();
                continue;
            } else{
                gameContinue = false;
            }
        } else {
            activeCharacterIndex ++;
            if (activeCharacterIndex >= characters.size()){
                activeCharacterIndex = 0;
                for (auto& character : characters) {
                    character->onRoundEnd();
                }
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
    characters.push_back(std::make_unique<Warrior>("John snow"));
    characters.push_back(std::make_unique<Archer>("Aragorn"));
}

void Game::skillAdder() {
    Skill fireball("Fireball", 20, 80, 2,10, SkillType::damage);
    Skill heal("Heal", 10, 0, 1,50, SkillType::heal);
    characters[0]->addSkill(fireball);
    characters[0]->addSkill(heal);
    characters[1]->addSkill(fireball);
    characters[1]->addSkill(heal);
    characters[2]->addSkill(fireball);
    characters[2]->addSkill(heal);
    characters[3]->addSkill(fireball);
    characters[3]->addSkill(heal);
    characters[4]->addSkill(fireball);
    characters[4]->addSkill(heal);
}

int Game::chooseTarget() {
    if (&currentCharacter() == playerCharacter) {
        while (true) {
            std::cout << "Who is your target?" << std::endl;
            for (int i = 0; i <= characters.size() - 1; ++i) {
                std::cout << i << "." << characters[i]->getName() << std::endl;
            }
            int choose;
            std::cin >> choose;
            if (action == ActionType::Heal) {
                if (choose >= 0 && choose <= characters.size() - 1) {
                    return choose;
                } else {
                    std::cout << "chosen number is not valid!" << std::endl;
                }
            } else {
                if (choose >= 0 && choose <= characters.size() - 1 && choose != activeCharacterIndex) {
                    return choose;
                } else {
                    std::cout << "chosen number is not valid!" << std::endl;
                }
            }
        }
    }else {
        int choose;
        if (action == ActionType::Heal) {
            choose = activeCharacterIndex;
            return choose;
        }
        else {
            choose = rand() % characters.size();
            while (choose == activeCharacterIndex) choose = rand() % characters.size();
            return choose;
        }
    }
}

ActionType Game::chooseAction() {
    if (&currentCharacter() == playerCharacter) {
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
                    std::cout << "Invalid number is chosen please retry noobie"
                              << std::endl;
            }
        }
    } else{
        if (currentCharacter().getSkill(1).isReady() && currentCharacter().hasEnoughMana(currentCharacter().getSkill(1).getManaCost()) && currentCharacter().getHealth() <= 50 ) return ActionType::Heal;
        else if (currentCharacter().getSkill(0).isReady() && currentCharacter().hasEnoughMana(currentCharacter().getSkill(0).getManaCost())) return ActionType::Fireball;
        else return ActionType::Attack;
    }
}

Character &Game::currentCharacter() {
    return *characters[activeCharacterIndex];
}

void Game::selectPlayerCharacter() {
    std::cout << "Choose your Champion"
              << std::endl
              << std::endl;

    int playerChoice;

    while (true) {
        for (int i = 0; i < characters.size(); ++i) {
            std::cout << i + 1 << ")";
            characters[i]->printInfo();
        }

        std::cin >> playerChoice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid type is chosen please retry noobie"
                      << std::endl
                      << std::endl;
            continue;
        }
        --playerChoice;

        if (0 <= playerChoice && playerChoice < characters.size()) {
            std::cout << "you chose "
                      << characters[playerChoice]->getName()
                      << std::endl;
            break;
        } else {
            std::cout << "Invalid number is chosen please retry noobie"
                      << std::endl
                      << std::endl;
        }
    }

    playerCharacter = characters[playerChoice].get();
}

void Game::resetMatch() {
    activeCharacterIndex = 0;
    round = 1;
    gameContinue = true;
    playerCharacter = nullptr;

    statistics.reset();
    bloodlust.reset();

    characters.clear();
    characterAdder();
    skillAdder();
}

PostGameChoice Game::postGameChoice() {
    while (true){
        std::cout << "do you want a rematch?"
                  <<std::endl
                  << "1.Rematch"
                  <<std::endl
                  << "2.Exit"
                  <<std::endl;

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid type is chosen please retry noobie"
                      << std::endl
                      << std::endl;
            continue;
        }

        if (choice != 1 && choice != 2) {
            std::cout << "Invalid number is chosen please retry noobie"
                      << std::endl
                      << std::endl;
            continue;
        }

        if (choice == 1){
            return PostGameChoice::Rematch;
        } else return PostGameChoice::Exit;
    }
}