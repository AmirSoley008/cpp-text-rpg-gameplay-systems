//
// Created by parsian on 9/7/2026.
//

#ifndef UNTITLED3_ACTIONEXECUTOR_H
#define UNTITLED3_ACTIONEXECUTOR_H

#include <iostream>
#include "Character.h"
#include "ActionType.h"


enum class Result{
    Success,
    Failed
};

class ActionExecutor {
public:
    Result execute(ActionType action, Character& caster, Character& target);
};


#endif //UNTITLED3_ACTIONEXECUTOR_H
