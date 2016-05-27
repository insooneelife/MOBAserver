#ifndef PENETRATE_SHOOTING_SKILL_H
#define PENETRATE_SHOOTING_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   PenetrateShootingSkill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   class to implement penetrating skill.
//
//-----------------------------------------------------------------------------

#include <set>
#include "cocos2d.h"
#include "AbstNonTargetSkill.h"

class AbstCharacter;

class PenetrateShootingSkill : public AbstNonTargetSkill
{
public:

	PenetrateShootingSkill(
		AbstCharacter* const shooter,
		cocos2d::Vec2 target_pos,
		int damage,
		int radius,
		int max_speed,
		double survive_time,
		const std::string& name,
		int team);

	virtual ~PenetrateShootingSkill() override 
	{}

	virtual void update(double time_elapsed) override;

	virtual void updateMovement(double time_elapsed) override;

	virtual void render() override;

	virtual bool handleMessage(const Telegram& telegram) override
	{
		return false;
	}
};


#endif