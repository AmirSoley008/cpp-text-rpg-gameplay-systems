//
// Created by parsian on 7/1/2026.
//

#ifndef UNTITLED3_SKILL_H
#define UNTITLED3_SKILL_H

#include <string>
#include <variant>
#include <vector>
#include "EffectRegistrar.h"
#include "OperationData.h"
#include <optional>
class Character;

struct SkillStats{
    int manaCost;

    int coolDown;

    SkillStats(int mc, int cd)
            :manaCost(mc),
             coolDown(cd){}
};

struct Operation{
    std::variant<
        DamageData,
        HealData,
        PersistentDamageData,
        PersistentHealData> data;
};
class Skill{
protected:
    std::string name;

    std::vector<Operation> operations;

    SkillStats skillStats;

    int currentCoolDown;

    static std::vector<Skill> skillDefinitions;
public:


    Skill(const std::string& name, const std::vector<Operation>& operations, int manaCost, int cooldown)
        :name(name),
         operations(operations),
         skillStats(manaCost,cooldown),
         currentCoolDown(0)
    {
    }
    bool use(Character& caster,Character& target, EffectRegistrar& effectRegistrar);

    const std::string& getSkillName() const;

    int getManaCost() const;

    bool isReady() const;

    void tickCooldown();

    bool providesHealing() const;

    bool providesDamaging() const;

    static void skillCreator();

    static Skill& findSkill(const std::string& name);

    std::optional<int> getSkillDamage() const;

    void printSkillsInfo() const;
};


#endif //UNTITLED3_SKILL_H
