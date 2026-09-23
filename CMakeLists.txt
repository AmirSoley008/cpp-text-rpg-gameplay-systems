cmake_minimum_required(VERSION 3.23)
project(untitled3)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled3 main.cpp Character.cpp Character.h Warrior.cpp Warrior.h Mage.cpp Mage.h Archer.cpp Archer.h Skill.cpp Skill.h Game.cpp Game.h ActionExecutor.cpp ActionExecutor.h ActionType.h EventBus.cpp EventBus.h CharacterKilled.h Statistics.cpp Statistics.h KillRewarder.cpp KillRewarder.h Bloodlust.cpp Bloodlust.h)
