#ifndef ABST_TARGET_SKILL_H
#define ABST_TARGET_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   AbstTargetSkill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Base class to define skill which has targeted character.
//
//-----------------------------------------------------------------------------

#include "AbstSkill.h"
#include "cocos2d.h"
#include "timer/CrudeTimer.h"
#include <list>

class GameWorld;
class AbstCharacter;

class AbstTargetSkill : public AbstSkill
{
public:

	AbstTargetSkill::AbstTargetSkill(
		GameWorld* const		world,
		AbstCharacter* const	target,
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

	virtual ~AbstTargetSkill()
	{}

	AbstCharacter* const		getTarget() const;

	//must be implemented
	virtual void				update(double time_elapsed) = 0;

	virtual void				updateMovement(double time_elapsed) = 0;

	virtual void				render() = 0;

	virtual bool				handleMessage(const Telegram& telegram) = 0;

protected:

	AbstCharacter* const		_target;
};



#endif