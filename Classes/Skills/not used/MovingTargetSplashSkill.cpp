#include "MovingTargetSplashSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "GlobalValues.h"
#include "DebugRenderer.h"
#include "Message/MessageDispatcher.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
MovingTargetSplashSkill::MovingTargetSplashSkill(
	cocos2d::Layer* const view_layer,
	AbstCharacter* const shooter,
	AbstCharacter* const target,
	Vec2 target_pos)
	:
	AbstTargetSkill(
		shooter->getWorld(),
		view_layer,
		target,
		target_pos,
		shooter->getId(),
		shooter->getPos(),
		shooter->getHeading(),
		5,
		50,
		20,
		2,
		10,
		"MovingTargetSplashSkill",
		SKILL,
		1.5f)
{
	CCASSERT(target_pos != Vec2(), "target != Vec2()");

	movePos(_heading * _bounding_radius * 2);

	_emitter = ParticleSystemQuad::create(Prm.ParticleFolderName + "\\FireSpear.plist");
	_emitter->setPosition(_pos);
	_emitter->setRotation(- util::getAngleFromZero(shooter->getHeading()));
	_emitter->setCameraMask(1 << 2);
	_emitter->setEmissionRate(150);
	_emitter->setDuration(_survive_time);
	_emitter->setGlobalZOrder(SKILL_ZORDER);
	_emitter->setSpeed(10);
	_emitter->setTexture(Director::getInstance()->
		getTextureCache()->addImage(Prm.ParticleFolderName + "\\fire.png"));
	_emitter->setPositionType(ParticleSystem::PositionType::FREE);
	view_layer->addChild(_emitter);
}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void MovingTargetSplashSkill::update(double time_elapsed)
{
	if (Clock.getCurrentTime() > _survive_time + _time_of_creation)
	{
		_dead = true;
	}

	_velocity = getMaxSpeed() * getHeading();

	//make sure vehicle does not exceed maximum velocity
	if (_velocity.getLength() > _max_speed) 
	{
		_velocity.normalize();
		_velocity *= _max_speed;
	}

	movePos(_velocity);
	testForImpact();
	
}


//-------------------------- render -------------------------------------------
//-----------------------------------------------------------------------------
void MovingTargetSplashSkill::render()
{
	_bounding_shape->render(Debug.getBoundingShapePanel());

	Debug.getBoundingCirclePanel()->
		drawCircle(getPos(), getBRadius(), (float)(2 * M_PI), 50, false, Color4F::WHITE);


}

void MovingTargetSplashSkill::testForImpact()
{
	//test to see if the line segment connecting the rocket's current position
	//and previous position intersects with any bots.
	if (!_target->isDead())
	{
		if ((_target->getPos() - _pos).getLength() < _bounding_radius)
		{

		}
	}
	else
	{
		_dead = true;
	}

	
}
