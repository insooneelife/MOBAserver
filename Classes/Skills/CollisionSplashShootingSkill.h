#ifndef COLLISION_SPLASH_SHOOTING_SKILL_H
#define COLLISION_SPLASH_SHOOTING_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   CollisionSplashShootingSkill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Class to implement skill that collide to others and gives splash
//			impact.
//
//-----------------------------------------------------------------------------

#include "cocos2d.h"
#include "AbstNonTargetSkill.h"

class AbstCharacter;

class CollisionSplashShootingSkill : public AbstNonTargetSkill
{
public:

	CollisionSplashShootingSkill(
		AbstCharacter* const shooter,
		cocos2d::Vec2 target,
		float splash_radius,
		int damage,
		int radius,
		int max_speed,
		double survive_time,
		const std::string& name,
		int team);

	virtual ~CollisionSplashShootingSkill() override {}

	virtual void update(double time_elapsed) override;

	virtual void updateMovement(double time_elapsed) override;

	virtual void render() override;

	virtual bool handleMessage(const Telegram& telegram) override
	{
		return false;
	}
};


#endif