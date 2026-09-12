# C++ Text RPG Gameplay Systems

A C++ project focused on learning and implementing gameplay systems, Modern C++, memory management, and gameplay architecture.

## Current Status

The project started as a small object-oriented RPG prototype and is being gradually evolved into a more structured Gameplay Systems project.

**Current milestone: Week 4 - Turn System / Gameplay Loop Architecture**

The current prototype has a structured turn-based gameplay flow with separate responsibilities for decision-making, action execution, action resolution, turn advancement, and round progression.

## Current Systems

* Character System
* Character Stats
* Health and Mana
* Combat
* Skills
* Skill Cooldowns
* Turn Management
* Action Execution
* Round-based Cooldown Progression
* Combat End Conditions
* Player and AI Decision Flow

## Gameplay Flow

The current gameplay loop follows this general flow:

```text
Player / AI Decision
        ↓
Action Selection
        ↓
ActionExecutor
        ↓
Character / Skill
        ↓
Action Resolution
        ↓
Game State Update
        ↓
Turn Advancement
        ↓
Round Progression
```

A successful action ends the current turn. Failed actions do not consume the turn and allow the current character to choose another action.

At the end of a round, character skills update their cooldown state.

## Architecture

The project is being developed incrementally, with each stage focused on a specific gameplay engineering problem rather than feature quantity.

Current responsibilities include:

* **Game** - coordinates gameplay flow, turn and round progression, character management, and combat end conditions.
* **ActionExecutor** - executes the selected action and reports whether the action succeeded.
* **Character** - owns character state, stats, skills, and character-level gameplay operations.
* **Skill** - owns skill behavior and cooldown state.

The architecture is intentionally evolving as new gameplay systems are introduced.

## Goals

The long-term goal is to transform the initial RPG prototype into a well-structured C++ Gameplay Systems project demonstrating:

* Modern C++
* Memory and Ownership
* STL
* Algorithms
* Gameplay Architecture
* State Machines
* Event-driven Design
* Modular Gameplay Systems
* Testing and Debugging
* Performance and Code Quality

## Project Direction

This project is primarily an engineering and learning project rather than a feature-heavy RPG.

The focus is on understanding how gameplay systems can be designed, separated, tested, and evolved in C++ while keeping the resulting architecture explainable and maintainable.
