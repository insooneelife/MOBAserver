#ifndef SPLASH_CONTINUOS_SKILL_H
#define SPLASH_CONTINUOS_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   SplashContinuousSkill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   class to implement targeting skill which impacts splash.
//
//-----------------------------------------------------------------------------

#include <set>
#include "cocos2d.h"
#include "AbstNonTargetSkill.h"

class AbstCharacter;

class SplashContinuousSkill : public AbstNonTargetSkill
{
public:

	SplashContinuousSkill(
		AbstCharacter* const shooter,
		cocos2d::Vec2 target_pos,
		double period,
		int damage,
		int radius,
		int max_speed,
		double survive_time,
		float splash_radius,
		const std::string& name,
		int team);

	virtual ~SplashContinuousSkill() override {}

	virtual void update(double time_elapsed) override;

	void updateMovement(double time_elapsed) override;

	virtual void render() override;

	virtual bool handleMessage(const Telegram& telegram) override
	{
		return false;
	}

private:
	double _period;
	cocos2d::Vec2 _target_pos;
	float _splash_radius;
	bool _activated;
};


#endif