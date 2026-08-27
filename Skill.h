//
// Created by parsian on 7/1/2026.
//

#ifndef UNTITLED3_SKILL_H
#define UNTITLED3_SKILL_H

#include <string>
class Character;

enum class SkillType{
    damage,
    heal
};

struct SkillStats{
    int damage;

    int manaCost;

    int coolDown;

    int healAmount;
    SkillStats(int mc, int d, int cd,int ha)
            :damage(d),
             manaCost(mc),
             coolDown(cd),
             healAmount(ha){}
};

class Skill{
protected:
    SkillType skillType;

    std::string name;

    SkillStats skillStats;

    int currentCoolDown;

public:


    Skill(const std::string& name,int manaCost,int damage,int cooldown,int healAmount,SkillType skillType)
        :name(name),
         skillStats(manaCost,damage,cooldown,healAmount),
         skillType(skillType),
         currentCoolDown(0)
    {
    }
    bool use(Character& caster,Character& target);

    const std::string& getSkillName() const;

    int getManaCost() const;

    bool isReady() const;

    void tickCooldown();
};


#endif //UNTITLED3_SKILL_H
