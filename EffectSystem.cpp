//
// Created by parsian on 9/27/2026.
//

#include "EffectSystem.h"
#include "Effect.h"

void EffectSystem::registerEffect(std::unique_ptr<Effect> effect) {
    activeEffects.push_back(std::move(effect));
}

std::unordered_map<Character* , std::vector<Character*>> EffectSystem::update() {
    std::unordered_map<Character* , std::vector<Character*>>deathReport;
    for (auto it = activeEffects.begin(); it != activeEffects.end();) {
        Character* caster = (*it)->getCaster();
        Character* target = (*it)->getTarget();

        if (target->isAlive()) {
            (*it)->updateEffect();

            if (!target->isAlive()) {
                deathReport[caster].push_back(target);
            }
            if ((*it)->isExpired() || !target->isAlive()) {
                it = activeEffects.erase(it);
            } else ++it;
        } else{
            it = activeEffects.erase(it);
        }
    }
    return deathReport;
}

void EffectSystem::resetActiveEffects() {
    activeEffects.clear();
}

void EffectSystem::deleteDeadTargetEffect(const CharacterKilled &event) {
    for (auto it = activeEffects.begin(); it != activeEffects.end();){
        if (event.victim == (*it)->getTarget()){
            it = activeEffects.erase(it);
        } else ++it;
    }
}