#ifndef TARGET_SKILL_H
#define TARGET_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   TargetSkill.h
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

class TargetSkill : public AbstTargetSkill
{
public:

	TargetSkill(
		AbstCharacter* const shooter,
		AbstCharacter* const target,
		float splash_radius,
		int damage,
		int radius,
		int max_speed,
		double survive_time,
		const std::string& name,
		int team);

	virtual ~TargetSkill() override {}

	virtual void update(double time_elapsed) override;

	void updateMovement(double time_elapsed) override;

	virtual void render() override;

	virtual bool handleMessage(const Telegram& telegram) override
	{
		return false;
	}

private:

	bool _activated;
};


#endif