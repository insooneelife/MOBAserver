#ifndef TARGET_DIRECT_CONTINUOUS_SKILL_H
#define TARGET_DIRECT_CONTINUOUS_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   TargetDirectContinuousSkill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   class to implement targeting skill which impacts splash.
//
//-----------------------------------------------------------------------------

#include <set>
#include "cocos2d.h"
#include "AbstTargetSkill.h"

class AbstCharacter;

class TargetDirectContinuousSkill : public AbstTargetSkill
{
public:

	TargetDirectContinuousSkill(
		AbstCharacter* const shooter,
		AbstCharacter* const target,
		double period,
		int damage,
		int radius,
		int max_speed,
		double survive_time,
		float splash_radius,
		const std::string& name,
		int team);

	virtual ~TargetDirectContinuousSkill() override {}

	virtual void update(double time_elapsed) override;

	void updateMovement(double time_elapsed) override;

	virtual void render() override;

	virtual bool handleMessage(const Telegram& telegram) override
	{
		return false;
	}

private:
	double _period;

	bool _activated;
};


#endif