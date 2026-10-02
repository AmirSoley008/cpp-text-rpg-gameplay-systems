# W10 Testing & Debugging

## ACT-01 | Attack → Damage → Next Turn

**Priority:** 🔴 Core

### Setup

- Two characters are alive.
- Character A is the active character.
- Character B is an enemy.
- Character A has enough conditions to perform a normal Attack.

### Action

1. Character A selects `Attack`.
2. Character A selects Character B as the target.
3. Execute the attack.

### Expected Result

- Character B's HP decreases by the correct damage amount.
- Character A's HP remains unchanged.
- The turn advances to the correct next character.
- No unrelated state changes occur.
- No character is incorrectly removed.
- No `CharacterKilled` event is published unless the attack actually kills Character B.

### Actual Result

- Yen's HP decreased from 200 to 150.
- Geralt's damage was 50, so the damage was applied correctly.
- Geralt's HP remained unchanged.
- The turn advanced correctly to Yen.
- No character was removed.
- No `CharacterKilled` event occurred.

### Status

PASS

### Notes

The combat output displayed Yen's HP as `150/250` even though
Yen was initially shown with 200 HP. The actual damage calculation
was correct, so this was not considered a failure for ACT-01.

## ACT-02 | Skill → Mana + Cooldown

**Priority:** 🔴 Core

### Setup

* Geralt is the active character.
* Geralt has 30 Mana.
* `PoisonStrike` costs 15 Mana.
* `PoisonStrike` has a cooldown greater than 0.
* At least one enemy is alive and can be targeted.

### Action

1. Geralt selects `PoisonStrike`.
2. Geralt selects an enemy target.
3. Execute the skill.
4. Inspect Geralt's Mana using the debugger.
5. Attempt to use `PoisonStrike` again before its cooldown expires.
6. Allow the cooldown to expire.
7. Use `PoisonStrike` again.
8. Attempt to use `PoisonStrike` once more after Mana becomes insufficient.

### Expected Result

* `PoisonStrike` executes successfully when Mana and cooldown requirements are satisfied.
* Geralt's Mana decreases from 30 to 15 after the first use.
* `PoisonStrike` enters cooldown after use.
* Attempting to use the skill during cooldown fails.
* After the cooldown expires, the skill becomes available again.
* The second successful use consumes the remaining 15 Mana.
* A further attempt fails because Geralt does not have enough Mana.
* A failed skill execution must not apply its gameplay effects.

### Actual Result

* `PoisonStrike` executed successfully on the first attempt.
* Debugger showed Geralt's Mana changing from 30 to 15.
* `PoisonStrike` entered cooldown after the first use.
* Attempting to use `PoisonStrike` during cooldown produced:
  `PoisonStrike cooldown is not ready yet! wait`
* After the cooldown expired, `PoisonStrike` was successfully used again.
* The remaining 15 Mana was consumed.
* A subsequent attempt produced:
  `not enough mana`
* The skill was rejected instead of executing without sufficient Mana.

### Status

PASS

## ACT-03 | Pure Heal → Caster

**Priority:** 🔴 Core

### Setup

* Geralt is the active character.
* Geralt is below maximum HP.
* Geralt has enough Mana to use `Heal`.
* `Heal` is ready and not on cooldown.
* `Heal` contains `HealData` only.

### Action

1. Select `Heal`.
2. Execute the skill.
3. Inspect Geralt's HP using the debugger.
4. Observe whether the game requests a target.

### Expected Result

* `Heal` executes successfully.
* `HealData` applies its healing amount to the caster.
* Geralt's HP increases by the correct amount.
* No enemy target selection is requested.
* No other character's HP is modified.

### Actual Result

* `Heal` executed successfully.
* Geralt's HP increased from 250 to 300.
* No target selection was requested.
* The skill correctly treated Geralt as the recipient of the healing effect.

### Status

PASS

## ACT-04 | Persistent Heal → Caster

**Priority:** 🔴 Core

### Setup

* Yen is the active character.
* Yen is below maximum HP.
* Yen has enough Mana to use `Regeneration`.
* `Regeneration` is ready and not on cooldown.
* `Regeneration` contains `PersistentHealData`.
* The effect has a defined healing amount and duration.

### Action

1. Select `Regeneration`.
2. Execute the skill.
3. Verify that no target selection is requested.
4. Inspect Yen's HP before the Round End.
5. Allow the current round to end.
6. Inspect Yen's HP after the first effect tick.
7. Allow another round to end.
8. Inspect Yen's HP after the second effect tick.
9. Continue until the effect duration expires.
10. Verify that no additional healing occurs after expiration.

### Expected Result

* `Regeneration` executes successfully.
* No enemy target selection is requested.
* The persistent healing effect is registered with `EffectSystem`.
* The healing is applied to the caster.
* Healing occurs at the correct Round-End lifecycle point.
* The effect applies the correct number of healing ticks.
* The effect expires after its defined duration.
* No additional healing occurs after expiration.
* No other character receives the healing effect.

### Actual Result

* `Regeneration` executed successfully.
* No target selection was requested.
* The persistent healing effect was applied correctly.
* Yen received the healing at the correct Round-End timing.
* The effect applied its healing ticks correctly.
* The effect expired after its defined duration.
* No additional healing occurred after expiration.
* All tested behavior matched the expected result.

### Status

PASS

## ACT-05 | Composite Damage + Heal

**Priority:** 🔴 Core

### Setup

* A character is the active caster.
* The caster is below maximum HP.
* An enemy target is alive.
* The caster has enough Mana to use the composite skill.
* The composite skill contains both `DamageData` and `HealData`.

### Action

1. Select the composite damage + heal skill.
2. Select an enemy target.
3. Execute the skill.
4. Inspect the caster's HP before and after execution.
5. Inspect the target's HP before and after execution.

### Expected Result

* The skill executes successfully.
* `DamageData` applies its damage to the selected enemy target.
* `HealData` applies its healing to the caster.
* The target loses the correct amount of HP.
* The caster gains the correct amount of HP.
* The healing is not applied to the target.
* The damage is not applied to the caster.

### Actual Result

* The composite skill executed successfully.
* The target's HP decreased by 40.
* The caster's HP increased by 20.
* Damage was correctly applied to the target.
* Healing was correctly applied to the caster.
* The two operations correctly used their defined recipients.

### Status

PASS

## ACT-06 | Insufficient Mana → Failure

**Priority:** 🔴 Core

### Setup

* A character is the active caster.
* The caster has less Mana than the selected skill's Mana cost.
* The selected skill is otherwise ready and available.
* A valid target exists.

### Action

1. Select a skill whose Mana cost is greater than the caster's current Mana.
2. Select a valid target if the action requires one.
3. Attempt to execute the skill.
4. Inspect the relevant character state using the debugger.

### Expected Result

* Skill execution fails because the caster does not have enough Mana.
* No gameplay effect is applied.
* Mana remains unchanged.
* Caster HP remains unchanged.
* Target HP remains unchanged.
* The skill does not enter or modify its cooldown because execution did not succeed.
* The turn is not incorrectly advanced as a successful action.

### Actual Result

* The skill execution was rejected because the caster did not have enough Mana.
* Debugger inspection confirmed that the skill did not execute.
* No gameplay effect was applied.
* The insufficient-Mana failure path behaved as expected.

### Status

PASS

## ACT-07 | Skill in Cooldown → Failure

**Priority:** 🔴 Core

### Setup

* A character is the active caster.
* The selected skill is ready and its cooldown is 0.
* The caster has enough Mana to use the skill.
* A valid target exists.

### Action

1. Execute the skill successfully.
2. Verify that the skill enters cooldown.
3. Attempt to use the same skill again before its cooldown reaches 0.
4. Inspect the skill state and relevant character state using the debugger.

### Expected Result

* The first skill execution succeeds.
* The skill enters its defined cooldown.
* The second execution fails because the skill is still on cooldown.
* No damage or healing is applied by the failed execution.
* Mana is not consumed by the failed execution.
* The existing cooldown is not reset or corrupted by the failed execution.
* The failed attempt does not count as a successful gameplay action.

### Actual Result

* The first skill execution succeeded.
* The skill entered cooldown.
* The second execution was rejected because the cooldown was not 0.
* Debugger inspection confirmed that the second skill execution did not occur.
* The cooldown restriction behaved as expected.

### Status

PASS

## CD-01 | Cooldown → Round End → Ready

**Priority:** 🔴 Core

### Setup

* Yen is the active character.
* `Fireball` is ready and its cooldown is 0.
* Yen has enough Mana to use `Fireball`.
* `Fireball` has a cooldown of 2 rounds.

### Action

1. Use `Fireball` successfully during Round 1.
2. Inspect `Fireball.currentCooldown` using the debugger.
3. Allow Round 1 to end.
4. Inspect the cooldown again.
5. Allow Round 2 to end.
6. Inspect the cooldown again.
7. Attempt to use `Fireball` after the cooldown reaches 0.

### Expected Result

* `Fireball` executes successfully during Round 1.
* The cooldown becomes active after use.
* The cooldown decreases by 1 at each Round-End.
* The cooldown reaches 0 after the defined number of rounds.
* `Fireball` cannot be used while its cooldown is greater than 0.
* `Fireball` becomes available again when the cooldown reaches 0.

### Actual Result

* Yen successfully used `Fireball` during Round 1.
* Debugger inspection showed the cooldown decreasing as rounds progressed.
* `Fireball` remained unavailable while its cooldown was active.
* By Round 3, the cooldown had reached 0.
* `Fireball` became available for use again.

### Status

PASS

## CD-02 | Independent Cooldowns

**Priority:** 🔴 Core

### Setup

* At least two characters own the same skill, such as `Fireball`.
* Both characters initially have the skill ready.
* Both characters have enough Mana to use the skill.

### Action

1. Have Yen use `Fireball` successfully.
2. Inspect Yen's `Fireball.currentCooldown` using the debugger.
3. Allow Aragorn to reach his turn.
4. Inspect Aragorn's `Fireball.currentCooldown`.
5. Verify that Aragorn's skill remains available independently of Yen's cooldown.

### Expected Result

* Yen's `Fireball` enters its cooldown after use.
* Aragorn's `Fireball` cooldown remains unchanged.
* Yen's cooldown does not affect Aragorn's skill.
* Each character maintains an independent cooldown state for the same skill definition.

### Actual Result

* Yen used `Fireball` successfully.
* Yen's `Fireball.currentCooldown` became 2.
* Aragorn also owned `Fireball`.
* Debugger inspection showed Aragorn's `Fireball.currentCooldown` remained 0.
* Aragorn's skill remained independent of Yen's cooldown.

### Status

PASS

## EFF-01 | Persistent Damage → Tick → Expiration

**Priority:** 🔴 Core

### Setup

* Geralt is the active character.
* Geralt has enough Mana to use `PoisonStrike`.
* `PoisonStrike` applies `PersistentDamageData`.
* Damage per round is 10.
* Duration is 3 rounds.
* Legolas is alive and can be targeted.

### Action

1. Geralt uses `PoisonStrike` on Legolas.
2. Allow the first Round-End to occur.
3. Inspect Legolas's HP and the active effects.
4. Allow the second Round-End to occur.
5. Inspect Legolas's HP and the active effects.
6. Allow the third Round-End to occur.
7. Inspect Legolas's HP and the active effects.
8. Allow another Round-End to occur after expiration.

### Expected Result

* Legolas loses 10 HP at each of the first three Round-Ends.
* The persistent damage effect remains active while its duration has not expired.
* After the third tick, the effect expires.
* The expired effect is removed from `activeEffects`.
* No additional damage is applied after expiration.

### Actual Result

* Legolas lost 10 HP at each of the first three Round-Ends.
* The persistent damage effect remained active during its duration.
* After the third tick, the effect expired.
* The effect was removed from `activeEffects`.
* No additional damage was applied after expiration.

### Status

PASS

## EFF-02 | Multiple Effects → Independent Execution

**Priority:** 🔴 Core

### Setup

* Geralt is the active character.
* Geralt has enough Mana to use skills that create persistent effects.
* Legolas is alive and can be targeted.
* At least two persistent effects can be active at the same time.
* The two effects have different damage values and/or durations.

### Action

1. Apply one persistent damage effect to Legolas.
2. Apply a second persistent damage effect to Legolas before the first effect expires.
3. Inspect `activeEffects` using the debugger.
4. Allow the next Round-End to occur.
5. Inspect Legolas's HP and both effects.
6. Continue through the remaining durations of both effects.
7. Observe when each effect expires.

### Expected Result

* Both effects are registered independently in `activeEffects`.
* At each Round-End, both active effects execute their own damage.
* One effect does not modify, reset, or remove the other effect's duration.
* Each effect expires according to its own duration.
* When one effect expires, the other remains active if its duration has not expired.
* After both effects expire, neither remains in `activeEffects`.

### Actual Result

* Both effects were registered independently in `activeEffects`.
* At each Round-End, both active effects executed their own damage.
* Neither effect modified, reset, or removed the other's duration.
* Each effect expired according to its own duration.
* When one effect expired, the other remained active until its own duration expired.
* After both effects expired, neither remained in `activeEffects`.

### Status

PASS

## EFF-03 | Caster Dies → Effect Continues

**Priority:** 🔴 Core

### Setup

* Geralt applies a persistent effect to Legolas.
* The effect is active and has remaining duration.
* Geralt and Legolas are both alive when the effect is created.

### Action

1. Geralt applies the persistent effect to Legolas.
2. Verify that the effect is registered in `activeEffects`.
3. Cause Geralt to die while the effect is still active.
4. Allow subsequent Round-Ends to occur.
5. Observe the effect's remaining duration and its damage application.
6. Continue until the effect expires.

### Expected Result

* Geralt's death does not remove the active effect.
* The effect continues applying its damage to Legolas at the expected Round-End intervals.
* The effect's duration continues progressing normally after Geralt's death.
* The effect expires according to its original duration.
* After expiration, the effect is removed from `activeEffects`.

### Actual Result

* Geralt died while the effect was still active.
* Geralt's death did not remove the effect.
* The effect continued applying its damage to Legolas at the expected Round-End intervals.
* The effect's duration continued progressing normally after Geralt's death.
* The effect reached the end of its original duration.
* After expiration, the effect was removed from `activeEffects`.

### Status

PASS

## EFF-04 | Target Dies → Effect Cleanup

**Priority:** 🔴 Core

### Setup

* Aragorn is alive and can be targeted.
* Aragorn has multiple active persistent effects applied to him.
* The effects were created by different casters, such as Legolas and Geralt.
* Both effects are present in `activeEffects`.

### Action

1. Apply `PoisonStrike` from Legolas to Aragorn.
2. Apply another `PoisonStrike` from Geralt to Aragorn.
3. Verify that both effects are present in `activeEffects`.
4. Cause Aragorn to die while both effects are still active.
5. Inspect `activeEffects` after Aragorn's death.
6. Allow the next Round-End to occur.

### Expected Result

* Both persistent effects are active before Aragorn's death.
* Aragorn's death triggers the appropriate `CharacterKilled` event.
* Both effects targeting Aragorn are removed from `activeEffects`.
* Effects targeting Aragorn do not execute after his death.
* No persistent damage is applied to Aragorn after the cleanup.

### Actual Result

* Two `PoisonStrike` effects were active on Aragorn.
* One effect had been created by Legolas and the other by Geralt.
* Aragorn died while both effects were active.
* Both effects were removed from `activeEffects`.
* Neither effect remained active after Aragorn's death.
* No further effect execution occurred on Aragorn.

### Status

PASS

## DEAD-01 | Direct Action → Death → Removal

**Priority:** 🔴 Core

### Setup

* Geralt and Aragorn are alive.
* Geralt is the active character.
* Aragorn has low enough HP to be killed by a single Attack.
* Geralt has enough Damage to kill Aragorn.

### Action

1. Geralt performs an `Attack` on Aragorn.
2. Inspect Aragorn's HP after the attack.
3. Observe the gameplay systems that react to the death.
4. Inspect the `characters` container after Aragorn's death.
5. Inspect the `deadCharacters` container after Aragorn's death.
6. Continue the Turn Loop and observe the next turn.

### Expected Result

* Aragorn is killed by the Attack.
* `CharacterKilled` is published exactly once.
* Aragorn is removed from `characters`.
* Aragorn is moved to `deadCharacters`.
* Other characters are not incorrectly affected by Aragorn's removal.
* The Turn Loop does not skip a character or produce an index error.
* Aragorn remains alive in terms of object lifetime, but is no longer present in `characters`.

### Actual Result

* Aragorn was killed by Geralt's Attack.
* The effects of the `CharacterKilled` event were observed through the reacting systems.
* Statistics, Bloodlust, and KillRewarder updated correctly after Aragorn's death.
* Aragorn was removed from `characters`.
* Aragorn was added to `deadCharacters`.
* Other characters were not incorrectly affected.
* The Turn Loop continued correctly without skipping a character or producing an index error.
* Aragorn remained alive through `deadCharacters`, while no longer being present in `characters`.
* The debugger could not directly inspect the `CharacterKilled` event object and reported `unable to create variable object`; this was a debugger inspection limitation, not a gameplay failure.

### Status

PASS

## DEAD-02 | Effect → Death → Removal

**Priority:** 🔴 Core

### Setup

* A persistent damage effect is active on a living character.
* The target has enough remaining HP for the persistent effect to kill it.
* The effect has not yet expired.

### Action

1. Apply a persistent damage effect to the target.
2. Allow the effect to execute at Round-End.
3. Continue until the effect reduces the target's HP to zero.
4. Observe the target's death and removal.
5. Observe the Turn Loop and match state after the death.

### Expected Result

* The persistent effect reduces the target's HP to zero.
* The target is detected as dead.
* `CharacterKilled` is published through the normal death lifecycle.
* The target is removed from `characters`.
* The target is moved to `deadCharacters`.
* The effect is cleaned up appropriately.
* The Turn Loop remains valid after the removal.
* The match reaches the correct end state when the death satisfies the match-ending condition.

### Actual Result

* The persistent effect reduced the target's HP to zero.
* The target was correctly detected as dead.
* The normal death lifecycle was executed.
* The target was removed from `characters` and handled as a dead character.
* The effect was cleaned up correctly.
* The game continued through the expected death/removal lifecycle.
* The match ended correctly after the death.

### Status

PASS

## DEAD-03 | Death Before Active Index → No Skipped Turn

**Priority:** 🔴 Core

### Setup

* Multiple characters are alive in `characters`.
* The active character has an `activeCharacterIndex` greater than 0.
* A character located before `activeCharacterIndex` can be killed.
* The Turn Loop is active.

### Action

1. Kill a character whose index is lower than `activeCharacterIndex`.
2. Observe the removal of the dead character from `characters`.
3. Inspect `activeCharacterIndex` after the removal.
4. Continue the Turn Loop.
5. Observe the following turns.

### Expected Result

* The selected character is correctly killed and removed.
* `activeCharacterIndex` remains consistent with the new `characters` layout.
* No living character is skipped because of the index shift.
* The next valid character receives its turn exactly once.
* The Turn Loop continues without an index error.

### Actual Result

* The selected character was killed and removed correctly.
* The active index remained consistent with the updated `characters` layout.
* No turn was skipped after the removal.
* The next valid character received its turn correctly.
* The Turn Loop continued without errors.

### Status

PASS

## DEAD-04 | Dead Character Lifetime

**Priority:** 🔴 Core

### Setup

* Multiple characters are alive in `characters`.
* At least one character can be killed during the match.
* `deadCharacters` is initially empty.

### Action

1. Kill one character during the match.
2. Verify that the dead character is removed from `characters`.
3. Verify that the same character is added to `deadCharacters`.
4. Continue the match through subsequent turns and rounds.
5. Allow the match to reach its end.
6. Inspect `deadCharacters` during and after the match.

### Expected Result

* The dead character is removed from `characters`.
* The dead character is added to `deadCharacters`.
* The character remains alive in terms of object lifetime while stored in `deadCharacters`.
* The character remains in `deadCharacters` for the remainder of the match.
* No dangling lifetime issue occurs as a result of removing the character from `characters`.
* The character is not destroyed prematurely before the match ends.

### Actual Result

* The dead character was removed from `characters`.
* The dead character was added to `deadCharacters`.
* The character remained stored in `deadCharacters` throughout the remainder of the match.
* The character was retained until the match ended.
* No premature destruction or lifetime-related error occurred.

### Status

PASS

## DEAD-05 | Multiple Effects → Single Death Event

**Priority:** 🔴 Core

### Setup

* A target character has multiple active persistent effects.
* The effects can execute during the same Round-End.
* The target has low enough HP that the effects can result in death.
* The effects originate from one or more characters.

### Action

1. Apply multiple persistent effects to the same target.
2. Allow the effects to execute at Round-End.
3. Cause the target to die as a result of the effect resolution.
4. Observe the `CharacterKilled` event reactions.
5. Inspect the KillRewarder and other death-related systems.

### Expected Result

* The target is detected as dead only once.
* Only one `CharacterKilled` event is published for the target's death.
* Exactly one killer is associated with the death event.
* Kill rewards are granted only once to the registered killer.
* Bloodlust and Statistics do not process the same death multiple times.
* The target is removed from `characters` only once.
* The target is added to `deadCharacters` only once.

### Actual Result

* The target was detected as dead once.
* Only one killer was associated with the target's death.
* The death reward was granted only to that killer.
* No duplicate kill reward was observed.
* The death was handled as a single kill rather than multiple kills from the active effects.

### Status

PASS

## EVT-01 | CharacterKilled → Event Reactions

**Priority:** 🔴 Core

### Setup

* Multiple systems are subscribed to `CharacterKilled`.
* The subscribed systems include:

  * `Statistics`
  * `KillRewarder`
  * `Bloodlust`
  * `EffectSystem`
* A valid character death can be triggered during the match.

### Action

1. Cause one character to die.
2. Observe the `CharacterKilled` event reactions.
3. Inspect the state of each subscribed system after the event.
4. Verify that each system performs only its own responsibility.

### Expected Result

* `CharacterKilled` is published when the character dies.
* `Statistics` records the kill correctly.
* `KillRewarder` grants the appropriate reward.
* `Bloodlust` updates the killer's kill streak correctly.
* `EffectSystem` performs the required cleanup for effects targeting the dead character.
* All subscribers react correctly without interfering with each other's responsibilities.

### Actual Result

* `CharacterKilled` was published correctly.
* `Statistics` reacted correctly and recorded the kill.
* `KillRewarder` reacted correctly and granted the expected reward.
* `Bloodlust` reacted correctly and updated the killer's streak.
* `EffectSystem` reacted correctly and performed the required cleanup.
* All subscribers performed their expected responsibilities correctly.

### Status

PASS

## EVT-02 | Kill → Reward + Bloodlust

**Priority:** 🔴 Core

### Setup

* `KillRewarder` and `Bloodlust` are subscribed to `CharacterKilled`.
* A character can successfully kill an enemy.
* The killer can accumulate multiple consecutive kills.

### Action

1. Perform a successful kill.
2. Observe the reward granted by `KillRewarder`.
3. Inspect the killer's Bloodlust state.
4. Perform another successful kill.
5. Inspect the Bloodlust state again.
6. Continue until the required kill-streak threshold is reached.
7. Observe the reward and Bloodlust response at the threshold.

### Expected Result

* Each successful kill triggers `CharacterKilled`.
* `KillRewarder` grants the appropriate reward for each kill.
* `Bloodlust` increments the killer's kill streak correctly.
* The kill streak does not increment for unrelated actions.
* When the configured streak threshold is reached, the corresponding Bloodlust reward is granted.
* Reward processing occurs only once per kill.

### Actual Result

* Each successful kill triggered the expected event reaction.
* `KillRewarder` granted the appropriate rewards.
* `Bloodlust` updated the killer's kill streak correctly.
* The configured streak reward was granted when the required threshold was reached.
* Rewards were granted correctly without duplicate processing.

### Status

PASS

## EVT-03 | Effect Death → Event Propagation

**Priority:** 🔴 Core

### Setup

* A persistent damage effect is active on a character.
* The target has enough remaining HP for the effect to cause death.
* `Statistics`, `KillRewarder`, `Bloodlust`, and `EffectSystem` are subscribed to `CharacterKilled`.

### Action

1. Allow the persistent effect to execute at Round-End.
2. Allow the effect to reduce the target's HP to zero.
3. Observe the target's death.
4. Observe the `CharacterKilled` event reactions.
5. Inspect the state changes produced by the subscribed systems.

### Expected Result

* The persistent effect causes the target's death.
* The death is detected correctly.
* `CharacterKilled` is published.
* `Statistics` records the kill.
* `KillRewarder` grants the appropriate reward.
* `Bloodlust` updates correctly.
* `EffectSystem` performs the required cleanup.
* The event propagation follows the same pipeline as a direct-action death.

### Actual Result

* The persistent effect caused the target's death.
* The death was detected correctly.
* `CharacterKilled` was propagated.
* `Statistics` reacted correctly.
* `KillRewarder` granted the appropriate reward.
* `Bloodlust` updated correctly.
* `EffectSystem` performed the required cleanup.
* The same expected event reactions occurred as in the direct-action death path.

### Status

PASS

## MATCH-01 | One Survivor → Match End

**Priority:** 🔴 Core

### Setup

* Multiple characters are alive in `characters`.
* The match is in progress.
* Characters can perform valid actions against each other.
* A winner can be determined when only one character remains alive.

### Action

1. Continue the match until all but one character are dead.
2. Observe the removal of defeated characters from `characters`.
3. Inspect `characters` when only one character remains.
4. Observe the match state and end condition.
5. Observe the winner reported by the game.

### Expected Result

* Defeated characters are removed from `characters`.
* Exactly one living character remains.
* The match end condition is triggered.
* The game stops the combat loop.
* `MatchEnded` is published.
* The remaining character is identified as the winner.
* The winner and final match state are reported correctly.

### Actual Result

* Defeated characters were removed from `characters`.
* Exactly one living character remained.
* The match end condition was triggered correctly.
* The combat loop ended.
* `MatchEnded` was published.
* The remaining character was correctly identified as the winner.
* The match ended with the expected final state.

### Status

PASS

## MATCH-02 | Effect Resolution → Draw

**Priority:** 🔴 Core

### Setup

* The match has multiple characters alive.
* Persistent effects are active on the remaining characters.
* The effects can cause the final characters to die during Round-End resolution.
* No character should remain alive after the effect resolution.

### Action

1. Allow the active persistent effects to execute at Round-End.
2. Allow the effects to kill the remaining characters.
3. Observe the death and removal of the affected characters.
4. Inspect `characters` after the effect resolution.
5. Observe the match end condition and final result.

### Expected Result

* The persistent effects execute correctly.
* All affected characters that reach zero HP are detected as dead.
* Dead characters are removed from `characters`.
* No living character remains after the effect resolution.
* The match end condition is triggered correctly.
* The game does not attempt to select a winner when there is no survivor.
* The match ends with the correct Draw result.

### Actual Result

* The persistent effects executed correctly.
* The affected characters were correctly detected as dead.
* Dead characters were removed from `characters`.
* No living character remained after the effect resolution.
* The match end condition was triggered correctly.
* No incorrect winner was selected.
* The match ended with the expected Draw result.

### Status

PASS

## AI-01 | AI Targeting Rules

**Priority:** 🔴 Core

### Setup

* Multiple characters are alive.
* An AI-controlled character can perform `Attack` and `UseSkill`.
* The AI has access to the relevant skills and enough resources when required.
* Multiple enemy targets are available for selection.

### Action

1. **Scenario A:** Give the AI access to a pure healing skill and observe its target selection.
2. **Scenario B:** Create a situation where one enemy can be killed by a normal `Attack`.
3. **Scenario C:** Create a situation where no enemy can be killed by a normal `Attack`.
4. **Scenario D:** Create a situation where an immediate-damage skill can kill an enemy.
5. **Scenario E:** Give the AI a persistent-damage-only skill and observe its target selection.

### Expected Result

* **A. Pure Heal:** The AI targets itself.
* **B. Attack Kill:** The AI selects the first enemy that can be killed by the attack.
* **C. No Kill:** The AI selects a random enemy.
* **D. Skill Damage Kill:** The AI selects an enemy that can be killed by the skill's immediate damage.
* **E. Persistent-only Skill:** The AI does not treat future persistent damage as immediate kill potential and falls back to random target selection.

### Actual Result

* **A. Pure Heal:** The AI correctly selected itself.
* **B. Attack Kill:** The AI correctly selected the first killable enemy.
* **C. No Kill:** The AI correctly selected a random enemy.
* **D. Skill Damage Kill:** The AI correctly selected an enemy that could be killed by the skill's immediate damage.
* **E. Persistent-only Skill:** The AI correctly fell back to random target selection instead of treating future persistent damage as an immediate kill.

### Status

PASS

## REG-01 | Full Combat Chain

**Priority:** 🔴 Core

### Setup

* A complete playable match is started with multiple characters.
* All major gameplay systems are active.
* Characters can perform normal attacks and skills.
* Persistent effects, cooldowns, events, AI, rewards, and death handling are enabled.

### Action

1. Start a complete match.
2. Play the match normally from the first turn until the match ends.
3. Allow characters to use attacks and skills.
4. Allow cooldowns and persistent effects to progress through Round-End.
5. Allow characters to die through direct actions and/or effects.
6. Observe event reactions, rewards, Bloodlust, effect cleanup, and character removal.
7. Continue until the match reaches its final state.
8. Observe the final match result.

### Expected Result

* The complete combat chain executes without unexpected behavior.
* Actions execute correctly.
* Skills correctly manage Mana and cooldowns.
* Persistent effects execute according to their lifecycle.
* Death detection and character removal remain correct.
* `CharacterKilled` propagates to the relevant systems.
* Rewards, Bloodlust, Statistics, and effect cleanup behave correctly.
* AI targeting remains valid throughout the match.
* No turn is incorrectly skipped.
* No index, lifetime, or state-management errors occur.
* The match reaches the correct final state.

### Actual Result

* The complete match executed correctly from start to finish.
* Actions and skills behaved as expected.
* Mana and cooldown lifecycles remained correct.
* Persistent effects executed and expired correctly.
* Character deaths and removals behaved correctly.
* `CharacterKilled` propagated correctly to the relevant systems.
* Rewards, Bloodlust, Statistics, and effect cleanup behaved correctly.
* AI targeting remained valid throughout the match.
* No turn was skipped.
* No unexpected index, lifetime, or state-management errors occurred.
* The match reached the expected final state without abnormal behavior.

### Status

PASS
