# C++ Text RPG Gameplay Systems

A small turn-based RPG project built with C++.

The project started as a simple RPG prototype and is being used as a practical space for learning C++ gameplay programming, system design, and gameplay architecture.

The goal is not to build a large RPG. The goal is to make the codebase progressively better as the gameplay systems become more complex.

## Current Status

The combat loop is functional and supports player-controlled and AI-controlled characters, skills, cooldowns, turn progression, character selection, and rematches.

The game now supports selecting a player character before a match, resetting the match after it ends, choosing between a rematch and exiting, and selecting a character again when starting a new match.

## Current Systems

* Character system
* Character stats
* Health and mana
* Combat actions
* Skills
* Skill cooldowns
* Turn management
* Round progression
* Action execution
* Player and AI turns
* Character selection
* Match reset
* Rematch flow
* Post-game choice

## Architecture

The project currently keeps the main gameplay coordination inside `Game`.

`Game` is responsible for the overall match flow, character management, turn and round progression, player selection, and deciding when a match ends.

`ActionExecutor` handles the execution of gameplay actions such as attacks and skills.

`Character` owns its stats and skills and provides character-level gameplay operations.

`Skill` contains skill data and cooldown state.

Character objects are owned by `Game` through `std::unique_ptr`. The selected player character is tracked separately through a non-owning pointer, so the player's identity does not depend on the character's position in the turn order.

The architecture is intentionally kept small. New abstractions are introduced when they solve an actual problem rather than simply because they are common game-development patterns.

## Game Flow and State Machines

The game flow now includes a clear lifecycle around each match.

The player first selects a character, then enters the turn-based combat loop. When the match ends, the player can choose to start a rematch or exit the game.

A rematch creates a new set of characters, resets the match state, and allows the player to select a character again.

A state machine was considered for representing different stages of the game flow, with states such as `CharacterSelection`, `PlayerTurn`, `AITurn`, `GameResult`, and `PostGameSelection`.

The concept was evaluated against the RPG itself. For the current size and structure of this project, introducing a full state machine into the combat loop would add structure without solving a significant problem. Because of that, the RPG does not currently use a state machine.

Instead, the game flow is currently handled with the existing loop and functions, which are sufficient for the current scope of the project.

This distinction is intentional. Gameplay architecture patterns are evaluated based on the problems they solve rather than being added simply because they are common in game development.

## Gameplay

The game is a simple turn-based combat prototype.

Each character can perform actions during their turn. The player selects actions and targets manually, while AI-controlled characters make their decisions automatically.

Skills have cooldowns that progress with rounds.

When only one character remains alive, the match ends. The player can then start a new match or exit the game.

## Goals

The project is being developed as a foundation for gameplay programming with C++.

Areas currently being explored include:

* Modern C++
* Memory ownership and lifetime
* STL and algorithms
* Object-oriented design
* Composition
* Gameplay architecture
* State machines
* Event-driven design
* Ability systems
* Combat systems
* Testing and debugging
* Performance and code quality

Not every topic is implemented immediately. Some are explored through isolated exercises before deciding whether they belong in the RPG itself.

## Project Direction

This is an engineering project, not a content-heavy RPG.

The important part is the process of taking a gameplay problem, designing a solution, implementing it in C++, and then evaluating whether the resulting architecture actually makes the code easier to understand and change.

As the project progresses, the codebase will continue to evolve toward more modular gameplay systems while avoiding unnecessary abstractions.
