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

The combat loop is functional and supports player-controlled and AI-controlled characters, skills, cooldowns, turn progression, character selection, rematches, and persistent gameplay effects.

The game currently supports selecting a player character before a match, resetting the match after it ends, choosing between a rematch and exiting, and selecting a character again when starting a new match.

The project also includes an event-driven system for gameplay events such as character deaths and match completion.

Abilities are now built through composable operations rather than ability-specific logic inside `Game` or `ActionExecutor`.

The current ability architecture supports:

* Direct damage
* Direct healing
* Persistent damage
* Persistent healing
* Abilities composed from multiple operations

The project also includes an `EffectSystem` responsible for owning and updating active persistent effects.

## Current Systems

* Character system
* Character stats
* Health and mana
* Combat actions
* Skills and abilities
* Operation-based ability composition
* Skill cooldowns
* Turn management
* Round progression
* Action execution
* Player and AI turns
* Character selection
* Match reset
* Rematch flow
* Post-game choice
* Persistent gameplay effects
* Event-driven gameplay reactions
* Kill statistics and history
* Kill rewards
* Kill-streak rewards

## Architecture

The project currently keeps the main gameplay coordination inside `Game`.

`Game` is responsible for the overall match flow, character management, turn and round progression, player selection, match completion, and publishing gameplay events.

`ActionExecutor` handles the execution of generic gameplay actions such as attacks and skill usage. It does not contain ability-specific branches for individual skills.

`Character` owns its stats and skills and provides character-level gameplay operations.

`Skill` represents an ability, including its operations, resource requirements, and cooldown state. A skill can contain multiple operations, allowing abilities to be composed from reusable gameplay operations.

`Operation` represents an individual gameplay operation performed by a skill. Operation data is represented through `std::variant`, currently supporting direct damage, direct healing, persistent damage, and persistent healing.

`EffectSystem` owns and updates active persistent effects. Skills create persistent effects through the `EffectRegistrar` interface without depending directly on the internal implementation of `EffectSystem`.

`EffectRegistrar` provides a narrow interface through which skills can register newly created effects. This keeps effect creation separate from effect ownership and management.

`EventBus` provides communication between gameplay systems through events, allowing multiple systems to react to the same gameplay occurrence without requiring `Game` to directly coordinate every reaction.

`Statistics` tracks kill counts and kill history.

`KillRewarder` handles the mana reward given to a character after a kill.

`Bloodlust` tracks kill streaks and provides an additional reward when a character reaches three consecutive kills.

Character objects are owned by `Game` through `std::unique_ptr`. The selected player character is tracked separately through a non-owning pointer, so the player's identity does not depend on the character's position in the turn order.

Active persistent effects are owned by `EffectSystem`. Dead characters are moved from the active character collection to `deadCharacters` and remain alive there until the match ends. This keeps non-owning references held by gameplay systems valid throughout the match.

The architecture is intentionally kept small. New abstractions are introduced when they solve an actual problem rather than simply because they are common game-development patterns.

## Ability Architecture

Abilities are represented as composable `Skill` objects rather than as separate hardcoded cases inside the game flow.

A `Skill` contains one or more `Operation` objects. Each operation contains a specific operation data type.

For example:

* `Fireball` → Damage
* `Heal` → Heal
* `PoisonStrike` → Persistent Damage
* `Regeneration` → Persistent Heal
* `VenomStrike` → Damage + Persistent Damage

This allows a new ability to be created by composing existing operations without requiring a new `ActionType` or a new ability-specific branch inside `ActionExecutor`.

The architecture separates the general concept of using a skill from the specific gameplay operations performed by that skill.

## Operation-Based Ability Composition

Operation data is represented using `std::variant`.

Current operation data types include:

* `DamageData`
* `HealData`
* `PersistentDamageData`
* `PersistentHealData`

### Operation Targeting Rules

Each operation type has a defined target based on its gameplay semantics:

* `DamageData` → Target
* `PersistentDamageData` → Target
* `HealData` → Caster
* `PersistentHealData` → Caster

This allows composite abilities to combine effects with different recipients.

For example, an ability can deal damage to an enemy while healing its caster without requiring a separate target system:

```text
Vampiric Strike
    → Damage → Target
    → Heal   → Caster
```

A skill can contain multiple operations, and the operations are executed sequentially when the skill is used.

This allows composite abilities to be represented through data composition instead of creating a separate class or execution path for every ability.

For example:

```cpp
Skill venomStrike(
    "VenomStrike",
    {
        {DamageData{40}},
        {PersistentDamageData{5, 2}}
    },
    20,
    2
);
```

The ability therefore applies immediate damage and also creates a persistent damage effect without requiring special handling in `Game` or `ActionExecutor`.

### Data-Driven Ability Definitions

Abilities are defined through composable operation data rather than separate classes or hardcoded execution branches for each ability.

Each `Skill` contains a sequence of `Operation` objects, and each operation stores one of the supported operation data types:

* `DamageData`
* `HealData`
* `PersistentDamageData`
* `PersistentHealData`

This allows different abilities to be created by combining existing operations.

For example:

```text
Fireball
    → Damage

Heal
    → Heal

PoisonStrike
    → Persistent Damage

Regeneration
    → Persistent Heal

VenomStrike
    → Damage + Persistent Damage
```

The execution logic is handled by `Skill::use()`, which validates the skill usage, applies its resource and cooldown changes, and executes the composed operations.

This keeps ability-specific content inside the ability definitions instead of requiring changes to `Game` or `ActionExecutor` whenever a new combination of existing operations is introduced.

### Skill Definition Registry

Skill definitions are stored in an internal registry managed by `Skill`.

```cpp
Skill::skillCreator();
Skill::findSkill("Fireball");
```

The registry stores reusable skill definitions. When a character receives a skill, the definition is copied into the character's own skill collection.

This creates a clear distinction between:

* **Skill Definition:** reusable ability data and behavior.
* **Character Skill Instance:** runtime state such as cooldown.

Because each character owns its own `Skill` instance, cooldown state remains independent between characters while the underlying ability definition can be reused.

## Effect System

Persistent gameplay effects are separated from direct skill execution.

`Skill` creates persistent effects when required, while `EffectSystem` owns and updates the active effects.

Current persistent effects include:

* Persistent damage
* Persistent healing

The `EffectSystem` updates active effects during round progression and removes effects when they expire or their targets are no longer valid.

Effects store their relevant caster and target references and maintain their own duration and operation data.

The system is designed so that a skill does not need to know how active effects are stored, updated, or cleaned up.

## Ownership and Lifetime

The current ownership model is:

* `Game` owns `Character` objects.
* `Character` owns its `Skill` objects.
* `EffectSystem` owns active `Effect` objects.
* `Skill` creates effects but does not own them.
* `EffectRegistrar` provides a narrow interface for effect registration.
* Dead characters are moved to `deadCharacters` and remain alive until the match ends.

This separates creation, ownership, and lifecycle responsibilities between the gameplay systems.

## Action Execution

Action types represent generic categories of player actions rather than specific abilities.

Current actions include:

* `Attack`
* `UseSkill`

The selected skill is represented separately by its skill index.

This means `ActionExecutor` does not need to know whether a `UseSkill` action represents `Fireball`, `Heal`, `PoisonStrike`, or another ability.

Adding a new ability therefore does not require adding a new `ActionType` or modifying the generic action execution logic.

## Combat Architecture

The combat flow is coordinated by `Game` as the combat orchestrator.

A turn follows this general lifecycle:

1. Action Selection
2. Target Selection
3. Action Execution
4. Immediate Resolution
5. Character Death Detection
6. Turn Advancement
7. Round-End Processing
8. Cooldown Updates
9. Persistent Effect Updates
10. Match Resolution

`ActionExecutor` provides the execution boundary for generic combat actions, while `Skill` handles the execution of the operations that make up an ability.

When a character dies during direct action resolution, `Game` publishes a `CharacterKilled` event and then moves the character from the active character collection to `deadCharacters`.

At the end of a round, `Game` advances cooldowns and asks `EffectSystem` to update persistent effects. If an effect causes a character to die, `EffectSystem` reports the death back to `Game`, which handles the character's lifecycle and publishes the corresponding `CharacterKilled` event.

This keeps combat orchestration, action execution, ability behavior, effect management, event reactions, and character ownership as separate responsibilities.

The current AI uses a simple rule-based targeting strategy:

* Healing abilities target the AI-controlled character itself.
* Immediate damage that can kill an enemy takes priority when selecting a target.
* If no enemy can be killed immediately, a random enemy is selected.
* Persistent damage is not considered for immediate kill checks.

The AI is intentionally kept simple for the scope of this project. More advanced decision-making is outside the current gameplay systems laboratory.

## Event-Driven Architecture

The project uses an event-driven system for gameplay occurrences that can have multiple independent reactions.

The main example is `CharacterKilled`.

When a character is killed, `Game` publishes a `CharacterKilled` event. Systems interested in the event subscribe independently and react to it without `Game` needing to directly call each system.

Currently, `CharacterKilled` is used by:

* `Statistics` to record kills and kill history
* `KillRewarder` to award mana to the killer
* `Bloodlust` to track kill streaks and apply its reward
* `EffectSystem` to remove effects targeting a killed character

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

Statistics, rewards, kill-streak behavior, and effect cleanup can react independently without requiring `Game` to directly coordinate every reaction.

### State Machine Evaluated but Not Integrated

A state machine was considered for representing different stages of the game flow, with states such as `CharacterSelection`, `PlayerTurn`, `AITurn`, `GameResult`, and `PostGameSelection`.

The concept was implemented and studied separately.

After evaluating it against the RPG itself, a full state machine was not integrated into the project because the current gameplay flow did not have a problem that justified the additional abstraction.

Instead, the game flow is currently handled with the existing loop and functions, which are sufficient for the current scope of the project.

This distinction is intentional. Gameplay architecture patterns are evaluated based on the problems they solve rather than being added simply because they are common in game development.

## Gameplay

The game is a simple turn-based combat prototype.

Each character can perform actions during their turn. The player selects actions and targets manually, while AI-controlled characters make their decisions automatically.

Skills have mana costs, effects, and cooldowns that progress with rounds.

Skills can contain multiple operations, allowing a single ability to combine different gameplay behaviors. Persistent operations create effects that continue across subsequent rounds.

When a character dies, the game publishes a `CharacterKilled` event before removing the character from the match. This allows gameplay systems such as statistics, rewards, kill streaks, and effect cleanup to react to the event independently.

When only one character remains alive, the match ends and a `MatchEnded` event is published. The winner is announced and the player can then choose to start a new match or exit the game.

A rematch creates a new set of characters, resets the match state, and allows the player to select a character again.

## Project Direction

This is an engineering project, not a content-heavy RPG.

The important part is the process of taking a gameplay problem, designing a solution, implementing it in C++, and then evaluating whether the resulting architecture actually makes the code easier to understand and change.

As the project progresses, the codebase will continue to evolve toward more modular gameplay systems while avoiding unnecessary abstractions.
