#include "AbstWeapon.h"


AbstWeapon::AbstWeapon(
	double        RateOfFire,
	double        IdealRange,
	double        ProjectileSpeed,
	AbstCharacter*   OwnerOfGun)
	:
	_owner(OwnerOfGun),
	_rate_of_fire(RateOfFire),
	_last_desirability_score(0),
	_ideal_range(IdealRange),
	_max_skill_speed(ProjectileSpeed)
{
	_time_next_available = Clock.getCurrentTime();
}

AbstWeapon::~AbstWeapon() 
{}

//------------------------ ReadyForNextShot -----------------------------------
//
//  returns true if the weapon is ready to be discharged
//-----------------------------------------------------------------------------
bool AbstWeapon::isReadyForNextShot()
{
	if (Clock.getCurrentTime() > _time_next_available)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
void AbstWeapon::updateTimeWeaponIsNextAvailable()
{
	_time_next_available = Clock.getCurrentTime() + 1.0 / _rate_of_fire;
}

//-----------------------------------------------------------------------------
bool AbstWeapon::aimAt(cocos2d::Vec2 target)const
{
	return _owner->rotateFacingTowardPosition(target);
}

double AbstWeapon::getMaxProjectileSpeed() const
{
	return _max_skill_speed; 
}

double AbstWeapon::getIdealRange() const
{
	return _ideal_range; 
}
