//
// Created by parsian on 9/26/2026.
//

#ifndef UNTITLED3_EFFECT_H
#define UNTITLED3_EFFECT_H

#include <variant>
#include <utility>
#include <vector>
#include "OperationData.h"
class Character;

class Effect {
private:
    Character* caster;
    Character* target;

    std::variant<PersistentDamageData,PersistentHealData> data;
public:
    Effect(Character* caster,Character* target,std::variant<PersistentDamageData, PersistentHealData> data)
    : caster(caster),
    target(target),
    data(std::move(data))
    {
    }

    void updateEffect();

    bool isExpired() const;

    Character* getTarget() const;

    Character* getCaster() const;
};


#endif //UNTITLED3_EFFECT_H
