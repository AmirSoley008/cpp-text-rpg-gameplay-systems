//
// Created by parsian on 9/7/2026.
//

#include "ActionExecutor.h"
#include "Character.h"

Result ActionExecutor::execute(ActionType action, int skillIndex, Character &caster, Character &target, EffectRegistrar& effectRegistrar) {
    if (action == ActionType::Attack) {
        caster.attack(target);
        return Result::Success;
    } else if (action == ActionType::UseSkill) {
        if (caster.useSkill(skillIndex ,target, effectRegistrar)) return Result::Success;
        else return Result::Failed;
    } else return Result::Failed;
}
