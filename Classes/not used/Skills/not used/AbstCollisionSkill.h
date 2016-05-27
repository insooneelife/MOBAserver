#ifndef ABST_COLLISION_EFFECT_H
#define ABST_COLLISION_EFFECT_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   AbstCollisionSkill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Base class to define a collision skill type. 
//			This type of skills can be explosed when collision occurs.
//
//-----------------------------------------------------------------------------

#include "AbstSkill.h"
#include "cocos2d.h"
#include "timer/CrudeTimer.h"
#include <list>

class GameWorld;
class AbstCharacter;

class AbstCollisionSkill : public AbstSkill
{
public:

	AbstCollisionSkill(
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

	virtual ~AbstCollisionSkill();

	//must be implemented
	virtual void				update(double time_elapsed) = 0;

	virtual void				render() = 0;

	virtual bool				handleMessage(const Telegram& telegram) = 0;

	//true if the projectile has impacted but is not yet dead (because it
	//may be exploding outwards from the point of impact for example)
	bool hasImpacted() const { return _impacted; }

protected:

	//this is set to true as soon as a collision effect hits something
	bool				_impacted;

	//the position where this collision effect impacts an object
	cocos2d::Vec2		_impact_pos;

private:
	//tests the trajectory of the shell for an impact
	virtual void testForImpact() = 0;
};



#endif