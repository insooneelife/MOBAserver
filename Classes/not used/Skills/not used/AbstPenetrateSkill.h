#ifndef ABST_PENETRATE_SKILL_H
#define ABST_PENETRATE_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   AbstPenetrateSkill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Base class to define a penetrate skill type.
//			This type of skills can impact to multiple entities.
//
//-----------------------------------------------------------------------------

#include "AbstSkill.h"
#include "cocos2d.h"
#include "timer/CrudeTimer.h"
#include <list>

class GameWorld;
class AbstCharacter;

class AbstPenetrateSkill : public AbstSkill
{
public:

	AbstPenetrateSkill(
		GameWorld* const		world,			//a pointer to the world data
		cocos2d::Layer*	const	view_layer,
		cocos2d::Vec2			target,			//the target's position
		int						shooter_id,		//the ID of the bot that fired this shot
		cocos2d::Vec2			origin,			//the start position of the projectile
		cocos2d::Vec2			heading,		//the heading of the projectile
		int						damage,			//how much damage it inflicts
		double					radius,
		double					max_speed,
		double					mass,
		double					max_force,
		std::string				name,
		int						type,
		double					survive_time);

	virtual ~AbstPenetrateSkill();

	//must be implemented
	virtual void				update(double time_elapsed) = 0;

	virtual void				render() = 0;

	virtual bool				handleMessage(const Telegram& telegram) = 0;

private:

	virtual void				testForImpact() = 0;
};



#endif