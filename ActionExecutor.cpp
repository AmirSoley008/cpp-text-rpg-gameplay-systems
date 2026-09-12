//
// Created by parsian on 9/7/2026.
//

#include "ActionExecutor.h"
#include "Character.h"

Result ActionExecutor::execute(ActionType action, Character &caster, Character &target) {
    if (action == ActionType::Attack) {
        caster.attack(target);
        return Result::Success;
    } else if (action == ActionType::Fireball) {
        if (caster.useSkill(0 ,target)) return Result::Success;
        else return Result::Failed;
    }else if (action == ActionType::Heal) {
        if (caster.useSkill(1 ,target)) return Result::Success;
        else return Result::Failed;
    } else return Result::Failed;
}
