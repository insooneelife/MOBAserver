#ifndef ABST_NONE_TARGET_SKILL_H
#define ABST_NONE_TARGET_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   AbstNonTargetSkill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Base class to define skill which has targeted character.
//
//-----------------------------------------------------------------------------

#include "AbstSkill.h"
#include "cocos2d.h"
#include "timer/CrudeTimer.h"

class GameWorld;

class AbstNonTargetSkill : public AbstSkill
{
public:

	AbstNonTargetSkill::AbstNonTargetSkill(
		GameWorld* const		world,
		cocos2d::Vec2			target_pos,
		int						shooter_id,
		cocos2d::Vec2			origin,
		cocos2d::Vec2			heading,
		int						damage,
		double					radius,
		double					max_speed,
		double					mass,
		double					max_force,
		std::string				name,
		int						type,
		double					survive_time);

	virtual ~AbstNonTargetSkill();

	//must be implemented
	virtual void				update(double time_elapsed) = 0;

	virtual void				updateMovement(double time_elapsed) = 0;

	virtual void				render() = 0;

	virtual bool				handleMessage(const Telegram& telegram) = 0;

};



#endif