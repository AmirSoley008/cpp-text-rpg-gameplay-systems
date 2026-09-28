//
// Created by parsian on 9/26/2026.
//

#include "Effect.h"
#include "Character.h"

void Effect::updateEffect() {
    if (target->isAlive()){
        std::visit([&](auto& data){

            using T = std::decay_t<decltype(data)>;

            if constexpr (std::is_same_v<T, PersistentDamageData>){
                target->takeDamage(data.damagePerRound);
                data.duration -= 1 ;
            }
            if constexpr (std::is_same_v<T, PersistentHealData>){
                target->heal(data.healPerRound);
                data.duration -= 1 ;
            }
            },data);
    } else return;
}

bool Effect::isExpired() const {
    return std::visit([](const auto& data){
        return data.duration == 0;
        },data);
}

Character* Effect::getTarget() const {
    return target;
}

Character* Effect::getCaster() const {
    return target;
}