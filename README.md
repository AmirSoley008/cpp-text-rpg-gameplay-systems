# C++ Text RPG Gameplay Systems

A small turn-based RPG project built with C++.

The project started as a simple RPG prototype and is being used as a practical space for learning C++ gameplay programming, system design, and gameplay architecture.

The goal is not to build a large RPG. The goal is to make the codebase progressively better as the gameplay systems become more complex.

## Project Goals

The primary goal of this project is to develop the engineering skills required for C++ gameplay programming.

The project focuses on:

* Modern C++
* Memory ownership and object lifetime
* STL and algorithms
* Object-oriented design
* Composition
* Gameplay architecture
* Event-driven systems
* Ability and skill architecture
* Combat systems
* State machines
* Testing and debugging
* Performance and code quality
* Architectural decision-making

The emphasis is on **engineering depth rather than feature count**.

## Current Status

The combat loop is functional and supports player-controlled and AI-controlled characters, skills, cooldowns, turn progression, character selection, and rematches.

The game currently supports selecting a player character before a match, resetting the match after it ends, choosing between a rematch and exiting, and selecting a character again when starting a new match.

The project also includes an event-driven system for gameplay events such as character deaths and match completion.

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
* Event-driven gameplay reactions
* Kill statistics and history
* Kill rewards
* Kill-streak rewards

## Architecture

The project currently keeps the main gameplay coordination inside `Game`.

`Game` is responsible for the overall match flow, character management, turn and round progression, player selection, match completion, and publishing gameplay events.

`ActionExecutor` handles the execution of gameplay actions such as attacks and skills.

`Character` owns its stats and skills and provides character-level gameplay operations.

`Skill` contains skill data and cooldown state.

`EventBus` provides communication between gameplay systems through events, allowing multiple systems to react to the same gameplay occurrence without requiring `Game` to directly coordinate every reaction.

`Statistics` tracks kill counts and kill history.

`KillRewarder` handles the mana reward given to a character after a kill.

`Bloodlust` tracks kill streaks and provides an additional reward when a character reaches three consecutive kills.

Character objects are owned by `Game` through `std::unique_ptr`. The selected player character is tracked separately through a non-owning pointer, so the player's identity does not depend on the character's position in the turn order.

The architecture is intentionally kept small. New abstractions are introduced when they solve an actual problem rather than simply because they are common game-development patterns.

## Event-Driven Architecture

The project uses an event-driven system for gameplay occurrences that can have multiple independent reactions.

The main example is `CharacterKilled`.

When a character is killed, `Game` publishes a `CharacterKilled` event. Systems interested in the event subscribe independently and react to it without `Game` needing to directly call each system.

Currently, `CharacterKilled` is used by:

* `Statistics` to record kills and kill history
* `KillRewarder` to award mana to the killer
* `Bloodlust` to track kill streaks and apply its reward

The project also uses a `MatchEnded` event when only one character remains alive. The event carries the winning character and allows interested systems to react to the end of the match.

The event system currently uses:

* `std::function`
* `std::any`
* `std::type_index`
* Lambda callbacks
* Template-based `subscribe` / `publish`

The EventBus is synchronous, so events are dispatched immediately to their registered callbacks.

## Architecture Decisions

### EventBus Instead of Direct System Calls

**Problem**

Multiple systems needed to react to a character death.

**Decision**

`Game` publishes `CharacterKilled` through `EventBus`.

**Result**

Statistics, rewards, and kill-streak behavior can react independently without requiring `Game` to directly coordinate every reaction.

### State Machine Evaluated but Not Integrated

A state machine was considered for representing different stages of the game flow, with states such as `CharacterSelection`, `PlayerTurn`, `AITurn`, `GameResult`, and `PostGameSelection`.

The concept was implemented and studied separately.

After evaluating it against the RPG itself, a full state machine was not integrated into the project because the current gameplay flow did not have a problem that justified the additional abstraction.

Instead, the game flow is currently handled with the existing loop and functions, which are sufficient for the current scope of the project.

This distinction is intentional. Gameplay architecture patterns are evaluated based on the problems they solve rather than being added simply because they are common in game development.

## Gameplay

The game is a simple turn-based combat prototype.

Each character can perform actions during their turn. The player selects actions and targets manually, while AI-controlled characters make their decisions automatically.

Skills have mana costs, effects, types, and cooldowns that progress with rounds.

When a character dies, the game publishes a `CharacterKilled` event before removing the character from the match. This allows gameplay systems such as statistics and rewards to react to the event independently.

When only one character remains alive, the match ends and a `MatchEnded` event is published. The winner is announced and the player can then choose to start a new match or exit the game.

A rematch creates a new set of characters, resets the match state, and allows the player to select a character again.

## Project Direction

This is an engineering project, not a content-heavy RPG.

The important part is the process of taking a gameplay problem, designing a solution, implementing it in C++, and then evaluating whether the resulting architecture actually makes the code easier to understand and change.

As the project progresses, the codebase will continue to evolve toward more modular gameplay systems while avoiding unnecessary abstractions.
