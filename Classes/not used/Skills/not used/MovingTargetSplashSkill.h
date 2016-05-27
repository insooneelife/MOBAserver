#ifndef PENETRATE_SHOOTING_SKILL_H
#define PENETRATE_SHOOTING_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   MovingTargetSplashSkill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   class to implement penetrate skill which is flying.
//
//-----------------------------------------------------------------------------

#include <set>
#include "cocos2d.h"
#include "AbstTargetSkill.h"

class AbstCharacter;

class MovingTargetSplashSkill : public AbstTargetSkill
{
public:

	MovingTargetSplashSkill(
		cocos2d::Layer* const view_layer,
		AbstCharacter* const shooter,
		AbstCharacter* const target,
		cocos2d::Vec2 target_pos);

	virtual ~MovingTargetSplashSkill() override {}

	virtual void update(double time_elapsed) override;

	virtual void render() override;

	virtual bool handleMessage(const Telegram& telegram) override
	{
		return false;
	}

private:

	virtual void testForImpact() override;

	std::set<int> _already_hit_agent;
};


#endif