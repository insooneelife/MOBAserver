#include "MovingEntity.h"
#include "ParameterLoader.h"
#include "Util.h"
#include "GameMap.h"
#include "Geometry\C2DMatrix.h"

USING_NS_CC;

MovingEntity::MovingEntity(
		GameWorld* const		world,
		geometry::Shape* const	bounding_shape,
		int						type,
		const std::string&		name,
		const Vec2&				position,
		double					radius,
		const Vec2&				velocity,
		double					max_speed,
		const Vec2&				heading,
		double					mass,
		double					turn_rate,
		double					max_force,
		bool					penetrate)
		:
		BaseGameEntity(world, bounding_shape, type, name, radius, position),
		_heading(heading),
		_velocity(velocity),
		_mass(mass),
		_side(_heading.getPerp()),
		_max_speed(max_speed),
		_max_turn_rate(turn_rate),
		_max_force(max_force),
		_heading_smoother(
			Smoother<Vec2>(Prm.NumSamplesForSmoothing, Vec2(0.0, 0.0))),
		_penetrate(penetrate)
{
}

//--------------------------- rotateHeadingToFacePosition ---------------------
//
//  given a target position, this method rotates the entity's heading and
//  side vectors by an amount not greater than m_dMaxTurnRate until it
//  directly faces the target.
//
//  returns true when the heading is facing in the desired direction
//-----------------------------------------------------------------------------
bool MovingEntity::rotateHeadingToFacePosition(const Vec2& target)
{
	Vec2 toTarget = target - _pos;
	toTarget.normalize();

	double dot = _heading.dot(toTarget);

	//some compilers lose acurracy so the value is clamped to ensure it
	//remains valid for the acos
	util::clamp(dot, -1, 1);

	//first determine the angle between the heading vector and the target
	double angle = acos(dot);

	//return true if the player is facing the target
	if (angle < 0.00001) return true;

	//clamp the amount to turn to the max turn rate
	if (angle > _max_turn_rate) angle = _max_turn_rate;

	//The next few lines use a rotation matrix to rotate the player's heading
	//vector accordingly

	C2DMatrix rotation_matrix;

	//notice how the direction of rotation has to be determined when creating
	//the rotation matrix
	rotation_matrix.Rotate(angle * util::sign(_heading, toTarget));
	rotation_matrix.Rotate(angle * util::sign(_heading, toTarget));
	rotation_matrix.TransformVector2Ds(_heading);
	rotation_matrix.TransformVector2Ds(_velocity);

	//finally recreate m_vSide
	_side = _heading.getPerp();

	return false;
}

//------------------------- SetHeading ----------------------------------------
//
//  first checks that the given heading is not a vector of zero length. If the
//  new heading is valid this fumction sets the entity's heading and side 
//  vectors accordingly
//-----------------------------------------------------------------------------
void MovingEntity::setHeading(const Vec2& new_heading)
{
	CC_ASSERT((new_heading.getLengthSq() - 1.0) < 0.00001);

	_heading = new_heading;

	//the side vector must always be perpendicular to the heading
	_side = _heading.getPerp();
}


bool MovingEntity::isSpeedMaxedOut() const
{
	return _max_speed*_max_speed >= _velocity.getLengthSq();
}

double MovingEntity::getSpeed() const 
{
	return _velocity.getLength();
}

double MovingEntity::getSpeedSq() const 
{
	return _velocity.getLengthSq(); 
}

const Vec2& MovingEntity::getVelocity() const 
{
	return _velocity; 
}

void MovingEntity::setVelocity(const Vec2& velocity) 
{
	_velocity = velocity; 
}

double MovingEntity::getMass() const 
{ 
	return _mass; 
}

void MovingEntity::setMass(double mass)
{
	_mass = mass; 
}

const Vec2& MovingEntity::getSide() const 
{
	return _side; 
}

void MovingEntity::setSide(const Vec2& side) 
{
	_side = side; 
}

double MovingEntity::getMaxSpeed() const
{
	return _max_speed; 
}

void MovingEntity::setMaxSpeed(double max_speed) 
{
	_max_speed = max_speed; 
}

double MovingEntity::getMaxForce() const 
{
	return _max_force; 
}

void MovingEntity::setMaxForce(double max_force) 
{
	_max_force = max_force; 
}

const Vec2& MovingEntity::getHeading() const 
{
	return _heading; 
}

double MovingEntity::getMaxTurnRate() const 
{
	return _max_turn_rate; 
}

void MovingEntity::setMaxTurnRate(double max_turn_rate) 
{
	_max_turn_rate = max_turn_rate; 
}

double MovingEntity::getTimeElapsed() {
	return _time_elapsed;
}

void MovingEntity::setTimeElapsed(double time_elapsed)
{
	this->_time_elapsed = time_elapsed;
}

bool MovingEntity::getPenetrate() {
	return _penetrate;
}

void MovingEntity::setPenetrate(bool penetrate) {
	_penetrate = penetrate;
}
