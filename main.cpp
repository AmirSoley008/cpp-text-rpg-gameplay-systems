#include <iostream>
#include "Character.h"
#include "Warrior.h"
#include "Mage.h"
#include "Archer.h"
#include "Skill.h"
#include "Game.h"


int main() {

    Warrior geralt("Geralt");
    Mage yen("Yennifer");
    Archer legolas("Legolas");

    legolas.printInfo();
    geralt.printInfo();
    yen.printInfo();

    Game game;
    game.start();

    return 0;
}
