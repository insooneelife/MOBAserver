#ifndef TARGET_SPLASH_SKILL_H
#define TARGET_SPLASH_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   TargetSplashSkill.h
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

class TargetSplashSkill : public AbstTargetSkill
{
public:

	TargetSplashSkill(
		AbstCharacter* const shooter,
		AbstCharacter* const target,
		float splash_radius,
		int damage,
		int radius,
		int max_speed,
		double survive_time,
		const std::string& name,
		int team);

	virtual ~TargetSplashSkill() override {}

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