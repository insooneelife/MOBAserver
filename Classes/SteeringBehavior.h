#ifndef STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   SteeringBehaviors.h
//
//  Desc:   class to encapsulate steering behaviors for a Vehicle
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------
#include <vector>
#include <string>
#include <list>

#include "boost\bimap.hpp"
#include "boost\assign.hpp"
#include "cocos2d.h"

class Path;
class AbstCharacter;
class CollisionArea;
class Wall;
class GameWorld;

class SteeringBehavior {
public:
	enum behavior_type
	{
		NONE = 0x00000,
		SEEK = 0x00002,
		FLEE = 0x00004,
		ARRIVE = 0x00008,
		WANDER = 0x00010,
		COHESION = 0x00020,
		SEPARATION = 0x00040,
		ALIGNMENT = 0x00080,
		OBSTACLE_AVOIDANCE = 0x00100,
		WALL_AVOIDANCE = 0x00200,
		FOLLOW_PATH = 0x00400,
		PURSUIT = 0x00800,
		EVADE = 0x01000,
		INTERPOSE = 0x02000,
		HIDE = 0x04000,
		FLOCK = 0x08000,
		OFFSET_PURSUIT = 0x10000,
	};


	typedef boost::bimaps::bimap< behavior_type, std::string > bm_type;
	static const bm_type behavior_types_map;

	//arrive makes use of these to determine how quickly a vehicle
	//should decelerate to its target

	SteeringBehavior(GameWorld* const world, AbstCharacter* const agent);
	virtual ~SteeringBehavior();

	//calculates and sums the steering forces from any ACTIVE behaviors
	const cocos2d::Vec2& calculate();

	//calculates the component of the steering force that is parallel
	//with the vehicle heading
	double		forwardComponent();

	//calculates the component of the steering force that is perpendicuar
	//with the vehicle heading
	double		sideComponent();

	//makes path
	void		createRandomPath(int num_waypoints, int mx, int my, int cx, int cy) const;

	void		fleeOn();
	void		seekOn();
	void		arriveOn();
	void		wanderOn();
	void		pursuitOn(AbstCharacter* const v);
	void		evadeOn(AbstCharacter* const v);
	void		cohesionOn();
	void		separationOn();
	void		alignmentOn();
	void		obstacleAvoidanceOn();
	void		wallAvoidanceOn();
	void		followPathOn();
	void		interposeOn(AbstCharacter* v1, AbstCharacter* v2);
	void		hideOn(AbstCharacter* const v);
	void		offsetPursuitOn(AbstCharacter* const v, const cocos2d::Vec2& offset);
	void		flockingOn();

	void		fleeOff();
	void		seekOff();
	void		arriveOff();
	void		wanderOff();
	void		pursuitOff();
	void		evadeOff();
	void		cohesionOff();
	void		separationOff();
	void		alignmentOff();
	void		obstacleAvoidanceOff();
	void		wallAvoidanceOff();
	void		followPathOff();
	void		interposeOff();
	void		hideOff();
	void		offsetPursuitOff();
	void		flockingOff();

	bool		isFleeOn();
	bool		isSeekOn();
	bool		isArriveOn();
	bool		isWanderOn();
	bool		isPursuitOn();
	bool		isEvadeOn();
	bool		isCohesionOn();
	bool		isSeparationOn();
	bool		isAlignmentOn();
	bool		isObstacleAvoidanceOn();
	bool		isWallAvoidanceOn();
	bool		isFollowPathOn();
	bool		isInterposeOn();
	bool		isHideOn();
	bool		isOffsetPursuitOn();

	void					setTarget(const cocos2d::Vec2& t);
	const cocos2d::Vec2&	getTarget() const;
	void					setTargetAgent(AbstCharacter* const agent);
	AbstCharacter*			getTargetAgent() const;

	void					setOffset(const cocos2d::Vec2& offset);
	const cocos2d::Vec2&	getOffset() const;

	void					setPath(const std::list<cocos2d::Vec2>& new_path);
	const cocos2d::Vec2&	getSteeringForce() const;

	double					getWeightSeparation()const;
	double					getWeightAlignment()const;
	double					getWeightCohesion()const;
	double					getWanderJitter()const;
	double					getWanderDistance()const;
	double					getWanderRadius()const;
	int						getBehaviorSet()const;
	double					getBoxLength()const;
	void					setBoxLength(double box_length);
	double					getWallDetectionFeelerLength()const;
	void					setPath(double feeler_length);
	double					getViewDistance() const;
	void					setViewDistance(double view_distance);

	const std::vector<cocos2d::Vec2>&	get_feelers()const;
	void								setFeelers(const std::vector<cocos2d::Vec2>& feelers);

private:
	SteeringBehavior() = delete;	//no default constructors
	SteeringBehavior(const SteeringBehavior&) = delete; // no copies
	SteeringBehavior& operator=(const SteeringBehavior&) = delete; // no self-assignments

	/* .......................................................

	BEGIN BEHAVIOR DECLARATIONS

	.......................................................*/

	//this behavior moves the agent towards a target position
	cocos2d::Vec2	seek(const cocos2d::Vec2& target_pos);

	//this behavior returns a vector that moves the agent away
	//from a target position
	cocos2d::Vec2	flee(const cocos2d::Vec2& target_pos);

	//this behavior is similar to seek but it attempts to arrive 
	//at the target position with a zero velocity
	cocos2d::Vec2	arrive(const cocos2d::Vec2& target_pos, double deceleration);

	//this behavior predicts where an agent will be in time T and seeks
	//towards that point to intercept it.
	cocos2d::Vec2	pursuit(const AbstCharacter* const agent);

	//this behavior maintains a position, in the direction of offset
	//from the target vehicle
	cocos2d::Vec2	offsetPursuit(const AbstCharacter* const agent, const cocos2d::Vec2& offset);

	//this behavior attempts to evade a pursuer
	cocos2d::Vec2	evade(const AbstCharacter* const agent);

	//this behavior makes the agent wander about randomly
	cocos2d::Vec2	wander();

	//given a series of Vector2Ds, this method produces a force that will
	//move the agent along the waypoints in order
	cocos2d::Vec2	followPath();

	//this returns a steering force which will attempt to keep the agent 
	//away from any obstacles it may encounter
	cocos2d::Vec2	obstacleAvoidance(const std::vector<std::unique_ptr<CollisionArea> >& obstacles);

	//this results in a steering force that attempts to steer the vehicle
	//to the center of the vector connecting two moving agents.
	cocos2d::Vec2	interpose(const AbstCharacter* agentA, const AbstCharacter* agentB);

	//given another agent position to hide from and a list of BaseGameEntitys this
	//method attempts to put an obstacle between itself and its opponent
	cocos2d::Vec2	hide(
		const AbstCharacter* hunter,
		const std::vector<std::unique_ptr<CollisionArea> >& obstacles);

	//helper method for Hide. Returns a position located on the other
	//side of an obstacle to the pursuer
	cocos2d::Vec2 getHidingPosition(
		const cocos2d::Vec2&	posOb,
		const double			radiusOb,
		const cocos2d::Vec2&	posHunter);

	// -- Group Behaviors -- //

	cocos2d::Vec2	cohesion(const std::vector<std::unique_ptr<AbstCharacter> >& agents);

	cocos2d::Vec2	separation(const std::vector<std::unique_ptr<AbstCharacter> >& agents);

	cocos2d::Vec2	alignment(const std::vector<std::unique_ptr<AbstCharacter> >& agents);


	//the following three are the same as above but they use cell-space
	//partitioning to find the neighbors
	cocos2d::Vec2	cohesionCellSpace();

	cocos2d::Vec2	separationCellSpace();

	cocos2d::Vec2	alignmentCellSpace();

	/* .......................................................

	END BEHAVIOR DECLARATIONS

	.......................................................*/

	//this function tests if a specific bit of _flag is set
	bool			isOn(behavior_type bt);

	bool			accumulateForce(cocos2d::Vec2 &running_tot, const cocos2d::Vec2& force_to_add);

	void			createFeelers();

	//calculates and sums the steering forces from any ACTIVE behaviors
	const cocos2d::Vec2&	calculatePrioritized();

	//A pointer for reference the world data.
	//So a vehicle can access any obstacle, path, wall or agent data.
	//Weak reference for remove circular reference.
	GameWorld* const		_game_world;

	//default
	double					_deceleration;

	//a pointer to the owner of this instance
	AbstCharacter* const	_moving_entity;

	//these can be used to keep track of friends, pursuers, or prey
	AbstCharacter*			_target_agent;

	AbstCharacter*			_target_agent2;

	//the steering force created by the combined effect of all
	//the selected behaviors
	cocos2d::Vec2			_steering_force;

	//the current target
	cocos2d::Vec2			_target;

	//the current position on the wander circle the agent is attempting to steer towards
	cocos2d::Vec2			_wander_target;

	//explained above
	double					_wander_jitter;
	double					_wander_radius;
	double					_wander_distance;

	//multipliers. These can be adjusted to effect strength of the  
	//appropriate behavior. Useful to get flocking the way you require
	//for example.
	double					_weight_separation;
	double					_weight_cohesion;
	double					_weight_alignment;
	double					_weight_wander;
	double					_weight_obstacle_avoidance;
	double					_weight_wall_avoidance;
	double					_weight_seek;
	double					_weight_flee;
	double					_weight_arrive;
	double					_weight_pursuit;
	double					_weight_offset_pursuit;
	double					_weight_interpose;
	double					_weight_hide;
	double					_weight_evade;
	double					_weight_follow_path;

	//how far the agent can 'see'
	double					_view_distance;

	//pointer to any current path
	std::unique_ptr<Path>	_path;

	//the distance (squared) a vehicle has to be from a path waypoint before
	//it starts seeking to the next waypoint
	double					_waypoint_seek_distSq;

	//any offset used for formations or offset pursuit
	cocos2d::Vec2			_offset;

	//binary flags to indicate whether or not a behavior should be ACTIVE
	int						_behavior_set;

	//length of the 'detection box' utilized in obstacle avoidance
	double					_box_length;

	//a vertex buffer to contain the feelers rqd for wall avoidance  
	std::vector<cocos2d::Vec2> _feelers;

	//the length of the 'feeler/s' used in wall detection
	double					_wall_detection_feeler_length;
};




#endif