//
// Created by parsian on 9/27/2026.
//

#ifndef UNTITLED3_EFFECTSYSTEM_H
#define UNTITLED3_EFFECTSYSTEM_H


#include <memory>
#include <vector>
#include <unordered_map>
#include "EffectRegistrar.h"
#include "Character.h"
#include "CharacterKilled.h"
#include "Effect.h"

class EffectSystem : public EffectRegistrar{
private:
    std::vector<std::unique_ptr<Effect>> activeEffects;

public:
    ~EffectSystem();

    std::unordered_map<Character* , std::vector<Character*>> update();

    void registerEffect(std::unique_ptr<Effect> effect) override;

    void resetActiveEffects();

    void deleteDeadTargetEffect(const CharacterKilled& event);

    bool hasActiveEffectsFrom(Character* target);
};


#endif //UNTITLED3_EFFECTSYSTEM_H
