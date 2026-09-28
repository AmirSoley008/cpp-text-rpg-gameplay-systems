//
// Created by parsian on 9/26/2026.
//

#ifndef UNTITLED3_EFFECTREGISTRAR_H
#define UNTITLED3_EFFECTREGISTRAR_H

#include <memory>
class Effect;

class EffectRegistrar{
public:
    virtual void registerEffect(std::unique_ptr<Effect> effect) = 0;
    virtual ~EffectRegistrar() = default;
};
#endif //UNTITLED3_EFFECTREGISTRAR_H
