#include <list>
#include <string>
#include "AbstCharacter.h"
#include "AbstWeaponSystem.h"
#include "GlobalValues.h"
#include "GameWorld.h"
#include "GameMap.h"
#include "Util.h"
#include "Message/Telegram.h"
#include "Motion.h"
#include "SteeringBehavior.h"
#include "CollisionArea.h"
#include "EntityFunctionTemplates.h"
#include "SensoryMemory.h"
#include "Goals\GoalThink.h"
#include "Geometry\Geometry.h"
#include "BuffNerf.h"

USING_NS_CC;

AbstCharacter::AbstCharacter(
	const std::string&			name,
	GameWorld* const			world,
	int							type,
	const cocos2d::Vec2&		position,
	int							team,
	int							line,
	double						rotation,
	double						mass,
	double						max_force,
	double						max_turn_rate,
	double						bounding_radius,
	int							max_health,
	int							max_mana,
	int							damage,
	int							mdamage,
	int							defence,
	int							mdefence,
	double						attack_range,
	double						view_range,
	double						move_speed,
	double						attack_speed,
	int							gold_worth,
	int							exp_worth)
	:
	MovingEntity(
		world,
		new geometry::Circle(position, bounding_radius),
		type,
		name,
		position,
		bounding_radius,
		Vec2(),
		move_speed,
		Vec2(sin(rotation), -cos(rotation)),
		mass,
		max_turn_rate,
		max_force,
		false),
	_motion(new Motion(name, Prm.CharacterFolderName, this, position, 0)),
	_buffs_nerfs(new BuffNerf()),
	_direction(0),
	_motion_name("Neutral"),
	_line(line),
	_base_max_hp(max_health),
	_base_max_mp(max_mana),
	_base_damage(damage),
	_base_mdamage(mdamage),
	_base_defence(defence),
	_base_mdefence(mdefence),
	_base_speed(move_speed),
	_attack_range(attack_range),
	_gold_worth(gold_worth),
	_exp_worth(exp_worth),
	_path_number(0)
{
	setTeam(team);
	_max_health = _health = max_health;
	_max_mana = _mana = max_mana;
	_game_world->getGameMap()->getAgentCellSpace()->addEntity(this);
}

AbstCharacter::~AbstCharacter()
{
	//_now_sprite must be deleted after child renderers.
	_sensory_memory.reset();

	//brain must be deleted before _motion, because it uses _motion's function.
	_brain.reset();
	_motion.reset();
	
	_game_world->getGameMap()->getAgentCellSpace()->removeEntity(this);

#ifdef SHOW_DTOR_INFO
	CCLOG("~AbstCharacter()");
#endif
}

void AbstCharacter::update(double time_elapsed) 
{
	_brain->process();

	if (isReadyForGoalArbitration() && isAlive())
	{
		_brain->arbitrate();
	}
	updateMovement(time_elapsed);
}

void AbstCharacter::updateMovement(double time_elapsed)
{
	//update the time elapsed
	_time_elapsed = time_elapsed;

	//keep a record of its old position so we can update its cell later in this method
	Vec2 old_pos = _pos;
	Vec2 steering_force;

	//calculate the combined force from each steering behavior in the  vehicle's list
	steering_force = _steering->calculate();

	Vec2 acceleration = steering_force / _mass;

	//update velocity
	_velocity += acceleration *time_elapsed;

	//make sure vehicle does not exceed maximum velocity
	if (_velocity.getLength() > _max_speed)
	{
		_velocity.normalize();
		_velocity *= _max_speed;
	}

	//update the position
	movePos(_velocity *time_elapsed);

	//update the heading if the vehicle has a non zero velocity
	if (_velocity.getLengthSq() > 0.00000001)
	{
		_heading = _velocity;
		_heading = _heading_smoother.update(_heading);
		_heading.normalize();

		_side = _heading.getPerp();
	}

	int new_dir = util::make_direction(_heading, _motion->isDirectionEight());

	if (new_dir != _direction) {
		setDirection(new_dir);
		this->setMotionDirect(_motion_name);
	}
	_motion->update(_motion_name, _pos, new_dir);
	

	//enforce a non-penetration constraint if desired
	if (!_penetrate)
	{
		enforceNonPenetrationCellSpace(
			this, _game_world->getGameMap()->getAgentCellSpace());

		enforceNonPenetrationCellSpace(
			this, _game_world->getGameMap()->getCollisionAreaCellSpace());
	}

	_game_world->getGameMap()->getAgentCellSpace()->updateEntity(this, old_pos);
}

void AbstCharacter::updateBuffNerf()
{
	ExtraCharacterInfo calc = _buffs_nerfs->calculate();
	_damage = calc.damage + _base_damage;
	_mdamage = calc.mdamage + _base_mdamage;
	_max_health = calc.max_health + _base_max_hp;
	_max_mana = calc.max_mana + _base_max_mp;
	_defence = calc.protect + _base_defence;
	_mdefence = calc.mprotect + _base_mdefence;
	_max_speed = calc.speed + _base_speed;
}

//------------------------ isAtPosition ---------------------------------------
//
//  returns true if the bot is close to the given position
//-----------------------------------------------------------------------------
bool AbstCharacter::isAtPosition(cocos2d::Vec2 pos, float radius)const
{
	return (getPos() - pos).getLengthSq() < radius * radius;
}

void AbstCharacter::reduceHealth(int damage)
{
	if (!isAlive())
		return;

	_health = std::max(0, _health - damage);

	if (_health <= 0)
	{
		setDying();
		this->getBrain()->removeAllSubgoals();
		this->getBrain()->addGoal_Death();
	}
}

//----------------- CalculateExpectedTimeToReachPosition ----------------------
//
//  returns a value indicating the time in seconds it will take the bot
//  to reach the given position at its current speed.
//-----------------------------------------------------------------------------
double AbstCharacter::calculateTimeToReachPosition(cocos2d::Vec2 pos)const
{
	return (getPos() - pos).getLengthSq() / (getMaxSpeed() * 500);
}

void AbstCharacter::render() 
{
}

bool AbstCharacter::handleMessage(const Telegram& telegram)
{
	if (getBrain()->handleMessage(telegram)) return true;
	
	switch (telegram.msg)
	{
	case MSG_PATH_READY:
	{
		//CCLOG("MSG_PATH_READY");
		return true;
	}
	case MSG_NOPATH_AVAILABLE:
	{
		//CCLOG("MSG_NOPATH_AVAILABLE");
	}
	break;
	}

	return false; 
}

MovingEntity* AbstCharacter::findEnemyInRange()
{
	_game_world->getGameMap()->getAgentCellSpace()->calculateNeighborsForSmall(
		_pos, &geometry::Circle(this->getPos(), _steering->getViewDistance()));

	for (MovingEntity* iter = _game_world->getGameMap()->getAgentCellSpace()->begin();
		!_game_world->getGameMap()->getAgentCellSpace()->end();
		iter = _game_world->getGameMap()->getAgentCellSpace()->next())
	{
		if (iter != this)
		{
			return iter;
		}
	}
	return nullptr;
}


//distance between target->pos and this->pos is smaller than range => return true
bool AbstCharacter::targetInRange(double range) const
{
	return util::inRange(_steering->getTarget(), getPos(), range);
}

bool AbstCharacter::inAttackRange() const
{
	CCASSERT(_steering->getTargetAgent() != nullptr, "No TargetAgent!");
	return ((_steering->getTargetAgent()->getPos() - getPos()).getLength() 
		< Prm.CharacterBaseAttackRange);
}

//returns true if this bot can move directly to the given position
//without bumping into any walls
bool AbstCharacter::canWalkTo(Vec2 pos)const
{
	return _game_world->isLOSOkay(getPos(), pos);
}

//similar to above. Returns true if the bot can move between the two
//given positions without bumping into any walls
bool AbstCharacter::canWalkBetween(Vec2 from, Vec2 to)const
{
	return _game_world->isLOSOkay(from, to);
}

//------------------ RotateFacingTowardPosition -------------------------------
//
//  given a target position, this method rotates the bot's facing vector
//  by an amount not greater than m_dMaxTurnRate until it
//  directly faces the target.
//
//  returns true when the heading is facing in the desired direction
//----------------------------------------------------------------------------
bool AbstCharacter::rotateFacingTowardPosition(cocos2d::Vec2 target)
{
	cocos2d::Vec2 toTarget = (target - _pos).getNormalized();

	double dot = _heading.dot(toTarget);

	//clamp to rectify any rounding errors
	util::clamp(dot, -1, 1);

	//determine the angle between the heading vector and the target
	double angle = acos(dot);

	//return true if the bot's facing is within WeaponAimTolerance degs of
	//facing the target
	const double WeaponAimTolerance = 0.01; //2 degs approx

	if (angle < WeaponAimTolerance)
	{
		_heading = toTarget;
		return true;
	}

	//clamp the amount to turn to the max turn rate
	if (angle > _max_turn_rate) angle = _max_turn_rate;

	//The next few lines use a rotation matrix to rotate the player's facing
	//vector accordingly
	C2DMatrix RotationMatrix;

	//notice how the direction of rotation has to be determined when creating
	//the rotation matrix
	RotationMatrix.Rotate(angle * util::sign(_heading, toTarget));
	RotationMatrix.TransformVector2Ds(_heading);

	return false;
}

//---------------------------- isReadyForGoalArbitration ------------------------
//
//  returns true if the bot is ready to be goal arbitration
//-----------------------------------------------------------------------------
bool AbstCharacter::isReadyForGoalArbitration()const
{
	return _goal_arbitration_regulator->isReady();
}

//---------------------------- isReadyForTriggerUpdate ------------------------
//
//  returns true if the bot is ready to be tested against the world triggers
//-----------------------------------------------------------------------------
bool AbstCharacter::isReadyForTriggerUpdate()const
{
	return _trigger_test_regulator->isReady();
}

bool AbstCharacter::isPossessed()const 
{
	return _possessed; 
}

LARGE_INTEGER AbstCharacter::getTime() const 
{
	return _time; 
}

void AbstCharacter::addTime(LONGLONG delta) 
{
	_time.QuadPart += delta; 
}

void AbstCharacter::addTime(LARGE_INTEGER delta) 
{
	addTime(delta.QuadPart); 
}

//getters & setters

const std::unique_ptr<SteeringBehavior>& AbstCharacter::getSteering() const
{
	return _steering;
}

const std::unique_ptr<Motion>& AbstCharacter::getMotion() const
{
	return _motion;
}

const std::unique_ptr<PathPlanner>& AbstCharacter::getPathPlanner() const
{
	return _path_planner; 
}

const std::unique_ptr<GoalThink>& AbstCharacter::getBrain() const
{
	return _brain; 
}

const std::unique_ptr<SensoryMemory>& AbstCharacter::getSensoryMemory() const
{
	return _sensory_memory;
}

const std::unique_ptr<AbstTargetingSystem>& AbstCharacter::getTargetSys() const
{
	return _target_system;
}

const std::unique_ptr<AbstWeaponSystem>& AbstCharacter::getWeaponSys() const 
{
	return _weapon_system; 
}

const std::unique_ptr<BuffNerf>& AbstCharacter::getBuffsNerfs() const
{
	return _buffs_nerfs;
}

const Vec2 AbstCharacter::getAttackDirection() const
{
	return (_target_system->getTarget()->getPos() - getPos()).getNormalized();
}

int AbstCharacter::getDirection() const
{
	return _direction;
}

void AbstCharacter::setDirection(int direction)
{
	_direction = direction;
}

const std::string& AbstCharacter::getMotionName() const
{
	return _motion_name;
}

void AbstCharacter::setMotionName(const std::string& motion)
{
	_motion_name = motion;
}

bool AbstCharacter::setMotionDirect(const std::string& motion)
{
	_motion_name = motion;
	return _motion->setMotion(motion, _direction);
}

int AbstCharacter::getHealth() const
{
	return _health;
}

void AbstCharacter::setHealth(int health)
{
	_health = health;
}

int AbstCharacter::getMana() const
{
	return _mana;
}

void AbstCharacter::setMana(int mana)
{
	_mana = mana;
}

int AbstCharacter::getMaxHealth() const
{
	return _max_health;
}

void AbstCharacter::setMaxHealth(int maxhealth)
{
	_max_health = maxhealth;
}

int AbstCharacter::getMaxMana() const
{
	return _max_mana;
}

void AbstCharacter::setMaxMana(int max_mana)
{
	_max_mana = max_mana;
}

int AbstCharacter::getAttackSpeed() const
{
	return _attack_speed;
}

void AbstCharacter::setAttackSpeed(int attack_speed)
{
	_attack_speed = attack_speed;
}

int AbstCharacter::getScore() const
{
	return _score;
}

void AbstCharacter::setScore(int score)
{
	_score = score;
}

const Vec2& AbstCharacter::getFacing() const
{
	return _facing;
}

void AbstCharacter::setFacing(const Vec2 facing)
{
	_facing = facing;
}
double AbstCharacter::getViewOnField() const
{
	return _view_on_field;
}

void AbstCharacter::setViewOnField(double view)
{
	_view_on_field = view;
}

int AbstCharacter::getNumHitUpdates() const
{
	return _num_hit_updates;
}

void AbstCharacter::setNumHitUpdates(int num_hit_updates)
{
	_num_hit_updates = num_hit_updates;
}

bool AbstCharacter::getHit() const
{
	return _hit;
}

void AbstCharacter::setHit(bool hit)
{
	_hit = hit;
}

int AbstCharacter::getDamage() const
{
	return _damage;
}

void AbstCharacter::setDamage(int power)
{
	_damage = power;
}

int AbstCharacter::getMDamage() const
{
	return _mdamage;
}

void AbstCharacter::setMDamage(int power)
{
	_mdamage = power;
}

double AbstCharacter::getAttackRange() const
{
	return _attack_range;
}

void AbstCharacter::setAttackRange(double val)
{
	_attack_range = val;
}

int AbstCharacter::getDefence() const
{
	return _defence;
}

void AbstCharacter::setDefence(int val)
{
	_defence = val;
}

int AbstCharacter::getMDefence() const
{
	return _mdefence;
}

void AbstCharacter::setMDefence(int val)
{
	_mdefence = val;
}


void AbstCharacter::setGoldWorth(int gold_worth)
{
	_gold_worth = gold_worth;
}

int AbstCharacter::getGoldWorth() const
{
	return _gold_worth;
}

int AbstCharacter::getExpWorth() const
{
	return _exp_worth;
}

void AbstCharacter::setExpWorth(int exp_worth)
{
	_exp_worth = exp_worth;
}


int AbstCharacter::getPathNumber() const
{
	return _path_number;
}

void AbstCharacter::setPathNumber(int path_number)
{
	_path_number = path_number;
}

int AbstCharacter::getLine() const
{
	return _line;
}

void AbstCharacter::setLine(int line)
{
	_line = line;
}

int AbstCharacter::getBaseDamage() const
{
	return _base_damage;
}

void AbstCharacter::setBaseDamage(int val)
{
	_base_damage = val;
}

int AbstCharacter::getBaseMDamage() const
{
	return _base_mdamage;
}

void AbstCharacter::setBaseMDamage(int val)
{
	_base_mdamage = val;
}

int AbstCharacter::getBaseMaxHp() const
{
	return _base_max_hp;
}

void AbstCharacter::setBaseMaxHp(int val)
{
	_base_max_hp = val;
}

int AbstCharacter::getBaseMaxMp() const
{
	return _base_max_mp;
}

void AbstCharacter::setBaseMaxMp(int val)
{
	_base_max_mp = val;
}

int AbstCharacter::getBaseDefence() const
{
	return _base_defence;
}

void AbstCharacter::setBaseDefence(int val)
{
	_base_defence = val;
}

int AbstCharacter::getBaseMDefence() const
{
	return _base_mdefence;
}

void AbstCharacter::setBaseMDefence(int val)
{
	_base_mdefence = val;
}

int AbstCharacter::getBaseSpeed() const
{
	return _base_speed;
}

void AbstCharacter::setBaseSpeed(int val)
{
	_base_speed = val;
}