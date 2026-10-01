//
// Created by parsian on 7/21/2026.
//

#include <iostream>
#include "Game.h"
#include "Character.h"
#include "Skill.h"
#include <limits>
#include <thread>
#include <chrono>

void Game::start() {
    std::cout << "Round " << round << " Starts" << std::endl;
    selectPlayerCharacter();
    while (gameContinue){
        while (true){
            action = chooseAction();
            int targetIndex = chooseTarget();
            if (executor.execute(action, selectedSkillIndex, currentCharacter(), *characters[targetIndex], effectSystem) == Result::Success){
                if (!characters[targetIndex]->isAlive()) {
                    eventBus.publish(CharacterKilled(&currentCharacter() , &*characters[targetIndex]));
                    deadCharacters.push_back(std::move(characters[targetIndex]));
                    if (targetIndex < activeCharacterIndex) {
                        activeCharacterIndex--;
                    }
                    characters.erase(characters.begin() + targetIndex);
                }
                break;
            } else std::cout << "choose Action again!" << std::endl;
        } if (characters.size() == 1){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            eventBus.publish(MatchEnded(&*characters[0]));
            std::this_thread::sleep_for(std::chrono::milliseconds(700));
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
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            if (activeCharacterIndex >= characters.size()){
                activeCharacterIndex = 0;
                for (auto& character : characters) {
                    character->onRoundEnd();
                }
                auto deathReport = effectSystem.update();
                if (!deathReport.empty()){
                    for (const auto &reaport:deathReport) {
                        for (auto &victim:reaport.second) {
                            auto it = std::find_if(
                                    characters.begin(),
                                    characters.end(),
                                    [victim](const auto& character)
                                    {
                                        return character.get() == victim;
                                    }
                                    );
                            if (it != characters.end()) {
                                int targetIndex =std::distance(characters.begin(),it);
                                if (targetIndex < activeCharacterIndex) {
                                    activeCharacterIndex--;
                                }
                                eventBus.publish(CharacterKilled(reaport.first, victim));
                                deadCharacters.push_back(std::move(*it));
                                characters.erase(it);
                            }
                        }
                    }
                }
                if (characters.size() == 1){
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    eventBus.publish(MatchEnded(&*characters[0]));
                    std::this_thread::sleep_for(std::chrono::milliseconds(700));
                    PostGameChoice choice = postGameChoice();
                    if (choice == PostGameChoice::Rematch){
                        resetMatch();
                        selectPlayerCharacter();
                        continue;
                    } else{
                        gameContinue = false;
                    }
                } else if (characters.empty()){
                    std::this_thread::sleep_for(std::chrono::milliseconds(700));
                    std::cout << "The match ended in a draw!" << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(700));
                    PostGameChoice choice = postGameChoice();
                    if (choice == PostGameChoice::Rematch){
                        resetMatch();
                        selectPlayerCharacter();
                        continue;
                    } else{
                        gameContinue = false;
                    }
                }
                if (gameContinue) {
                    round++;
                    std::cout << "Round " << round << " Starts" << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
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
    Skill::skillCreator();

    characters[0]->addSkill(Skill::findSkill("PoisonStrike"));
    characters[0]->addSkill(Skill::findSkill("Heal"));
    characters[1]->addSkill(Skill::findSkill("Fireball"));
    characters[1]->addSkill(Skill::findSkill("Heal"));
    characters[1]->addSkill(Skill::findSkill("Regeneration"));
    characters[2]->addSkill(Skill::findSkill("Regeneration"));
    characters[2]->addSkill(Skill::findSkill("Heal"));
    characters[2]->addSkill(Skill::findSkill("PoisonStrike"));
    characters[3]->addSkill(Skill::findSkill("Regeneration"));
    characters[3]->addSkill(Skill::findSkill("VenomStrike"));
    characters[3]->addSkill(Skill::findSkill("PoisonStrike"));
    characters[4]->addSkill(Skill::findSkill("Fireball"));
    characters[4]->addSkill(Skill::findSkill("Heal"));
}

int Game::chooseTarget() {
    if (&currentCharacter() == playerCharacter) {
        while (true) {
            std::cout << "Who is your target?" << std::endl;
            for (int i = 0; i < characters.size(); ++i) {
                std::cout << i << "." << characters[i]->getName() << std::endl;
            }
            int choose;
            std::cin >> choose;
            const auto &skills = currentCharacter().getSkillsList();
            if (action == ActionType::UseSkill && skills[selectedSkillIndex].providesHealing()) {
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
        const auto &skills = currentCharacter().getSkillsList();
        if (action == ActionType::UseSkill && skills[selectedSkillIndex].providesHealing()) {
            return activeCharacterIndex;
        }
        else {
            if (action == ActionType::Attack){
                for (int i = 0; i < characters.size(); ++i) {
                    if (activeCharacterIndex == i){
                        continue;
                    } if (currentCharacter().getDamageAmount() >= characters[i]->getHealth()){
                        return i;
                    }
                }
                return getRandomEnemyIndex();
            } else {
                if (auto damage = skills[selectedSkillIndex].getSkillDamage()) {
                    for (int i = 0; i < characters.size(); ++i) {
                        if (activeCharacterIndex == i){
                            continue;
                        } if (*damage >= characters[i]->getHealth()){
                            return i;
                        }
                    }
                    return getRandomEnemyIndex();
                }
                return getRandomEnemyIndex();
            }
        }
    }
}

int Game::getRandomEnemyIndex()
{
    std::uniform_int_distribution<int> distribution(
            0,
            static_cast<int>(characters.size()) - 2
    );

    int randomIndex = distribution(generator);

    if (randomIndex >= activeCharacterIndex) {
        ++randomIndex;
    }

    return randomIndex;
}

ActionType Game::chooseAction() {
    if (&currentCharacter() == playerCharacter) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            std::cout << "what action do you want to perform?"
                      << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            std::cout << "1.Attack!!"
                      << std::endl;
            for (int i = 0; i < playerCharacter->getSkillsList().size(); ++i) {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                std::cout << i + 2
                          << "."
                          << playerCharacter->getSkillsList()[i].getSkillName()
                          << std::endl;
            }
            int chooseAct;
            std::cin >> chooseAct;

            if (chooseAct == 1){
                selectedSkillIndex = -1;
                return ActionType::Attack;
            } else if (chooseAct >= 2 && chooseAct < static_cast<int>(playerCharacter->getSkillsList().size()+2)){
                selectedSkillIndex = chooseAct - 2;
                return ActionType::UseSkill;
            }
            std::cout << "Invalid number is chosen please retry noobie" << std::endl;
        }
    } else{
        if (currentCharacter().getHealth() <=50){
            const auto &skills = currentCharacter().getSkillsList();

            for (int i = 0; i < skills.size(); ++i) {
                if (skills[i].providesHealing() && skills[i].isReady() && currentCharacter().hasEnoughMana(skills[i].getManaCost())){
                    selectedSkillIndex = i;
                    return ActionType::UseSkill;
                }
            }
        } const auto &skills = currentCharacter().getSkillsList();
        for (int i = 0; i < skills.size(); ++i) {
            if (skills[i].providesDamaging() && skills[i].isReady() && currentCharacter().hasEnoughMana(skills[i].getManaCost())){
                selectedSkillIndex = i;
                return ActionType::UseSkill;
            }
        } selectedSkillIndex = -1;
        return ActionType::Attack;
    }
}

Character &Game::currentCharacter() {
    return *characters[activeCharacterIndex];
}

void Game::selectPlayerCharacter() {
    std::cout << "Choose your Champion"
              << std::endl
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    int playerChoice;

    while (true) {
        for (int i = 0; i < characters.size(); ++i) {
            std::cout << i + 1 << ")";
            characters[i]->printInfo();
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
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

    effectSystem.resetActiveEffects();
    deadCharacters.clear();
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