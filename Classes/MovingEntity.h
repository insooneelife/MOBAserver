#ifndef MOVING_ENTITY
#define MOVING_ENTITY
//------------------------------------------------------------------------
//
//  Name:   MovingEntity.h
//
//  Desc:   A base class defining an entity that moves. The entity has 
//          a local coordinate system and members for defining its
//          mass and velocity.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <cassert>
#include <memory>
#include "BaseGameEntity.h"
#include "cocos2d.h"
#include "Smoother.h"
#include "GameWorld.h"

class GameWorld;

class MovingEntity : public BaseGameEntity {
public:
	MovingEntity(
		GameWorld* const		world,
		geometry::Shape* const	bounding_shape,
		int						type,
		const std::string&		name,
		const cocos2d::Vec2&	position,
		double					radius,
		const cocos2d::Vec2&	velocity,
		double					max_speed,
		const cocos2d::Vec2&	heading,
		double					mass,
		double					turn_rate,
		double					max_force,
		bool					penetrate);

	virtual ~MovingEntity()
	{}
	//getters && setters
	bool						isSpeedMaxedOut() const;
	double						getSpeed() const;
	double						getSpeedSq() const;
	bool						rotateHeadingToFacePosition(const cocos2d::Vec2& target);

	const cocos2d::Vec2&		getVelocity() const;
	void						setVelocity(const cocos2d::Vec2& velocity);

	double						getMass() const;
	void						setMass(double mass);

	const cocos2d::Vec2&		getSide() const;
	void						setSide(const cocos2d::Vec2& side);

	double						getMaxSpeed() const;
	void						setMaxSpeed(double max_speed);

	double						getMaxForce() const;
	void						setMaxForce(double max_force);

	const cocos2d::Vec2&		getHeading() const;
	void						setHeading(const cocos2d::Vec2& new_heading);

	double						getMaxTurnRate() const;
	void						setMaxTurnRate(double max_turn_rate);
	
	double						getTimeElapsed();
	void						setTimeElapsed(double time_elapsed);

	bool						getPenetrate();
	void						setPenetrate(bool penetrate);

	//virtual functions
	virtual void				update(double time_elapsed) = 0;

	virtual void				updateMovement(double time_elapsed) = 0;

	virtual void				render() = 0;

	virtual bool				handleMessage(const Telegram& telegram) = 0;

protected:

	cocos2d::Vec2			_velocity;

	//a normalized vector pointing in the direction the entity is heading. 
	cocos2d::Vec2			_heading;

	//a vector perpendicular to the heading vector
	cocos2d::Vec2			_side;

	double					_mass;

	//the maximum speed this entity may travel at.
	double					_max_speed;

	//the maximum force this entity can produce to power itself 
	//(think rockets and thrust)
	double					_max_force;

	//the maximum rate (radians per second)this vehicle can rotate         
	double					_max_turn_rate;

	double					_time_elapsed;

	//if true, this entity can penetrate other entity
	bool					_penetrate;

	//The following members are used to smooth the vehicle's heading
	Smoother<cocos2d::Vec2>	_heading_smoother;

private:
	MovingEntity(const MovingEntity&) = delete; // no copies
	MovingEntity& operator=(const MovingEntity&) = delete; // no self-assignments
	MovingEntity() = delete;
};



#endif