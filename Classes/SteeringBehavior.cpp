#include <algorithm>
#include "SteeringBehavior.h"
#include "GlobalValues.h"
#include "CollisionArea.h"
#include "AbstCharacter.h"
#include "ParameterLoader.h"
#include "GameMap.h"
#include "Geometry\Geometry.h"
#include "GameWorld.h"
#include "Path.h"

using std::string;
using std::vector;
USING_NS_CC;

const SteeringBehavior::bm_type SteeringBehavior::behavior_types_map =
boost::assign::list_of< bm_type::relation >
(NONE, "NONE")
(SEEK, "SEEK")
(FLEE, "FLEE")
(ARRIVE, "ARRIVE")
(WANDER, "WANDER")
(COHESION, "COHESION")
(SEPARATION, "SEPARATION")
(ALIGNMENT, "ALIGNMENT")
(OBSTACLE_AVOIDANCE, "OBSTACLE_AVOIDANCE")
(WALL_AVOIDANCE, "WALL_AVOIDANCE")
(FOLLOW_PATH, "FOLLOW_PATH")
(PURSUIT, "PURSUIT")
(EVADE, "EVADE")
(INTERPOSE, "INTERPOSE")
(HIDE, "HIDE")
(FLOCK, "FLOCK")
(OFFSET_PURSUIT, "OFFSET_PURSUIT");


//------------------------- ctor -----------------------------------------
//
//------------------------------------------------------------------------
SteeringBehavior::SteeringBehavior(GameWorld* const world, AbstCharacter* const agent)
	:
	_game_world(world),
	_moving_entity(agent),
	_behavior_set(NONE),
	_weight_cohesion(Prm.CohesionWeight),
	_weight_alignment(Prm.AlignmentWeight),
	_weight_separation(Prm.SeparationWeight),
	_weight_obstacle_avoidance(Prm.ObstacleAvoidanceWeight),
	_weight_wander(Prm.WanderWeight),
	_weight_wall_avoidance(Prm.WallAvoidanceWeight),
	_view_distance(Prm.ViewDistance),
	_deceleration(NORMAL),
	_target_agent(nullptr),
	_wander_distance(Prm.WanderDist),
	_wander_jitter(Prm.WanderJitterPerSec),
	_wander_radius(Prm.WanderRad),
	_waypoint_seek_distSq(Prm.WaypointSeekDist * Prm.WaypointSeekDist),
	_weight_seek(Prm.SeekWeight),
	_weight_flee(Prm.FleeWeight),
	_weight_arrive(Prm.ArriveWeight),
	_weight_pursuit(Prm.PursuitWeight),
	_weight_offset_pursuit(Prm.OffsetPursuitWeight),
	_weight_interpose(Prm.InterposeWeight),
	_weight_hide(Prm.HideWeight),
	_weight_evade(Prm.EvadeWeight),
	_weight_follow_path(Prm.FollowPathWeight),
	_wall_detection_feeler_length(Prm.WallDetectionFeelerLength),
	_feelers(3),
	_path(new Path())
{
	//stuff for the wander behavior
	double theta = util::genRand<0, 1>() * 2 * M_PI;

	//create a vector to a target position on the wander circle
	_wander_target = Vec2(_wander_radius * cos(theta),
		_wander_radius * sin(theta));

	_path->loopOn();
}

//---------------------------------dtor ----------------------------------
SteeringBehavior::~SteeringBehavior() 
{}


/////////////////////////////////////////////////////////////////////////////// CALCULATE METHODS 


//----------------------- calculate --------------------------------------
//
//  calculates the accumulated steering force according to the method set
//  in m_SummingMethod
//------------------------------------------------------------------------
const Vec2& SteeringBehavior::calculate()
{
	//reset the steering force
	_steering_force = _steering_force.ZERO;

	//calculate neighbours in cell-space if any of the following 3 group
	//behaviors are switched on
	if (isOn(SEPARATION) || isOn(ALIGNMENT) || isOn(COHESION))
	{
		_game_world->getGameMap()->getAgentCellSpace()->
			calculateNeighborsForSmall(
				_moving_entity->getPos(),
				&geometry::Circle(_moving_entity->getPos(), _view_distance));
	}

	_steering_force = calculatePrioritized();

	return _steering_force;
}

//------------------------- ForwardComponent -----------------------------
//
//  returns the forward oomponent of the steering force
//------------------------------------------------------------------------
double SteeringBehavior::forwardComponent()
{
	return _moving_entity->getHeading().dot(_steering_force);
}

//--------------------------- SideComponent ------------------------------
//  returns the side component of the steering force
//------------------------------------------------------------------------
double SteeringBehavior::sideComponent()
{
	return _moving_entity->getSide().dot(_steering_force);
}


//--------------------- accumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  vehicle has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool SteeringBehavior::accumulateForce(Vec2 &running_tot, const Vec2& force_to_add) 
{

	//calculate how much steering force the vehicle has used so far
	double magnitude_so_far = running_tot.getLength();

	//calculate how much steering force remains to be used by this vehicle
	double magnitude_remaining = _moving_entity->getMaxForce() - magnitude_so_far;

	//return false if there is no more force left to use
	if (magnitude_remaining <= 0.0) return false;

	//calculate the magnitude of the force we want to add
	double magnitude_to_add = force_to_add.getLength();

	//if the magnitude of the sum of ForceToAdd and the running total
	//does not exceed the maximum force available to this vehicle, just
	//add together. Otherwise add as much of the ForceToAdd vector is
	//possible without going over the max.
	if (magnitude_to_add < magnitude_remaining)
	{
		running_tot += force_to_add;
	}

	else
	{
		//add it to the steering force
		running_tot += (force_to_add.getNormalized() * magnitude_remaining);
	}

	return true;
}

//---------------------- calculatePrioritized ----------------------------
//
//  this method calls each ACTIVE steering behavior in order of priority
//  and acumulates their forces until the max steering force magnitude
//  is reached, at which time the function returns the steering force 
//  accumulated to that  point
//------------------------------------------------------------------------
const Vec2& SteeringBehavior::calculatePrioritized() 
{
	Vec2 force;

	if (isOn(OBSTACLE_AVOIDANCE))
	{
		force = obstacleAvoidance(_game_world->getGameMap()->getCollisionAreas()) *
			_weight_obstacle_avoidance;
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}

	if (isOn(EVADE))
	{
		CCASSERT(_target_agent, "evade target not assigned");
		force = evade(_target_agent) * _weight_evade;
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}


	if (isOn(FLEE))
	{
		force = flee(_game_world->getCrossHair()) * _weight_flee;
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}


	if (isOn(SEPARATION))
	{
		force = separationCellSpace() * _weight_separation;
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}

	if (isOn(ALIGNMENT))
	{
		force = alignmentCellSpace() * _weight_alignment;
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}

	if (isOn(COHESION))
	{
		force = cohesionCellSpace() * _weight_cohesion;
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}
	



	if (isOn(SEEK))
	{
		force = seek(_target) * _weight_seek;

		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}


	if (isOn(ARRIVE))
	{
		force = arrive(_target, _deceleration) * _weight_arrive;

		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}

	if (isOn(WANDER))
	{
		force = wander() * _weight_wander;
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}

	if (isOn(PURSUIT))
	{
		CCASSERT(_target_agent, "pursuit target not assigned");

		force = pursuit(_target_agent) * _weight_pursuit;
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}

	if (isOn(OFFSET_PURSUIT))
	{
		CCASSERT(_target_agent, "pursuit target not assigned");
		CCASSERT(!_offset.isZero(), "No offset assigned");

		force = offsetPursuit(_target_agent, _offset);
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}

	if (isOn(FOLLOW_PATH))
	{
		force = followPath() * _weight_follow_path;
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}

	if (isOn(INTERPOSE))
	{
		force += interpose(_target_agent, _target_agent2) * _weight_interpose;
		if (!accumulateForce(_steering_force, force)) return _steering_force;
	}

	return _steering_force;
}

/////////////////////////////////////////////////////////////////////////////// START OF BEHAVIORS

//------------------------------- seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  direct the agent towards the target
//------------------------------------------------------------------------
Vec2 SteeringBehavior::seek(const Vec2& target_pos)
{
	Vec2 temp = target_pos - _moving_entity->getPos();
	Vec2 desired_velocity = temp.getNormalized() * _moving_entity->getMaxSpeed();

	return (desired_velocity - _moving_entity->getVelocity());
}

//----------------------------- flee -------------------------------------
//
//  Does the opposite of seek
//------------------------------------------------------------------------
Vec2 SteeringBehavior::flee(const Vec2& target_pos)
{
	//only flee if the target is within 'panic distance'. Work in distance
	//squared space.
   /* const double PanicDistanceSq = 100.0f * 100.0;
	if (Vec2DDistanceSq(m_pVehicle->Pos(), target) > PanicDistanceSq)
	{
	  return Vector2D(0,0);
	}
	*/
	Vec2 temp = _moving_entity->getPos() - target_pos;
	Vec2 desired_velocity = temp.getNormalized() * _moving_entity->getMaxSpeed();

	return (desired_velocity - _moving_entity->getVelocity());
}

//--------------------------- arrive -------------------------------------
//
//  This behavior is similar to seek but it attempts to arrive at the
//  target with a zero velocity
//------------------------------------------------------------------------

Vec2 SteeringBehavior::arrive(const Vec2& target_pos, double deceleration)
{
	Vec2 to_target = target_pos - _moving_entity->getPos();

	//calculate the distance to the target
	double dist = to_target.getLength();

	if (dist > 0)
	{
		//because Deceleration is enumerated as an int, this value is required
		//to provide fine tweaking of the deceleration..
		const double deceleration_tweaker = 0.3;

		//calculate the speed required to reach the target given the desired
		//deceleration
		double speed = dist / (deceleration * deceleration_tweaker);

		//make sure the velocity does not exceed the max
		speed = std::min(speed, _moving_entity->getMaxSpeed());

		//from here proceed just like seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		Vec2 desired_velocity = to_target * speed / dist;

		return (desired_velocity - _moving_entity->getVelocity());
	}

	return Vec2(0, 0);
}

//------------------------------ pursuit ---------------------------------
//
//  this behavior creates a force that steers the agent towards the 
//  evader
//------------------------------------------------------------------------
Vec2 SteeringBehavior::pursuit(const AbstCharacter* const evader)
{
	//if the evader is ahead and facing the agent then we can just seek
	//for the evader's current position.
	Vec2 to_evader = evader->getPos() - _moving_entity->getPos();

	double relative_heading = _moving_entity->getHeading().dot(evader->getHeading());

	if ((to_evader.dot(_moving_entity->getHeading()) > 0) &&
		(relative_heading < -0.95))  //acos(0.95)=18 degs
	{
		return seek(evader->getPos());
	}

	//Not considered ahead so we predict where the evader will be.

	//the lookahead time is propotional to the distance between the evader
	//and the pursuer; and is inversely proportional to the sum of the
	//agent's velocities
	double look_ahead_time = to_evader.getLength() /
		(_moving_entity->getMaxSpeed() + evader->getMaxSpeed());

	//now seek to the predicted future position of the evader
	return seek(evader->getPos() + evader->getVelocity() * look_ahead_time);
}


//----------------------------- evade ------------------------------------
//
//  similar to pursuit except the agent Flees from the estimated future
//  position of the pursuer
//------------------------------------------------------------------------
Vec2 SteeringBehavior::evade(const AbstCharacter* const pursuer)
{
	/* Not necessary to include the check for facing direction this time */

	Vec2 to_pursuer = pursuer->getPos() - _moving_entity->getPos();

	//uncomment the following two lines to have evade only consider pursuers 
	//within a 'threat range'
	const double threat_range = 100.0;
	if (to_pursuer.getLengthSq() > threat_range * threat_range) return Vec2();

	//the lookahead time is propotional to the distance between the pursuer
	//and the pursuer; and is inversely proportional to the sum of the
	//agents' velocities
	double look_ahead_time = to_pursuer.getLength() /
		(_moving_entity->getMaxSpeed() + pursuer->getSpeed());

	//now flee away from predicted future position of the pursuer
	return flee(pursuer->getPos() + pursuer->getVelocity() * look_ahead_time);
}


//--------------------------- wander -------------------------------------
//
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
Vec2 SteeringBehavior::wander()
{
	//this behavior is dependent on the update rate, so this line must
	//be included when using time independent framerate.
	double jitter_this_time_slice = _wander_jitter * _moving_entity->getTimeElapsed();

	//first, add a small random vector to the target's position
	_wander_target += Vec2(
		util::randomClamped() * jitter_this_time_slice,
		util::randomClamped() * jitter_this_time_slice);
	
	//reproject this new vector back on to a unit circle
	_wander_target.getNormalized();

	//increase the length of the vector to the same as the radius
	//of the wander circle
	_wander_target *= _wander_radius;

	//move the target into a position WanderDist in front of the agent
	Vec2 target = _wander_target + Vec2(_wander_distance, 0);

	//project the target into world space
	Vec2 target_in_world = PointToWorldSpace(target,
		_moving_entity->getHeading(),
		_moving_entity->getSide(),
		_moving_entity->getPos());

	//and steer towards it
	return target_in_world - _moving_entity->getPos();
}


//---------------------------- separation --------------------------------
//
// this calculates a force repelling from the other neighbors
//------------------------------------------------------------------------
Vec2 SteeringBehavior::separation(const vector<std::unique_ptr<AbstCharacter> >& neighbors)
{
	Vec2 steering_force;

	for (unsigned int a = 0; a < neighbors.size(); ++a)
	{
		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough. ***also make sure it doesn't
		//include the evade target ***
		if ((neighbors[a].get() != _moving_entity) && neighbors[a]->isTagged() &&
			(neighbors[a].get() != _target_agent))
		{
			Vec2 to_agent = _moving_entity->getPos() - neighbors[a]->getPos();

			//scale the force inversely proportional to the agents distance  
			//from its neighbor.
			Vec2 temp = to_agent.getNormalized();
			steering_force += temp / to_agent.getLength();
		}
	}
	return steering_force;
}


Vec2 SteeringBehavior::separationCellSpace()
{
	Vec2 steering_force;

	//iterate through the neighbors and sum up all the position vectors
	for (BaseGameEntity* ent = _game_world->getGameMap()->getAgentCellSpace()->begin();
		!_game_world->getGameMap()->getAgentCellSpace()->end();
		ent = _game_world->getGameMap()->getAgentCellSpace()->next())
	{
		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough
		if (ent != _moving_entity)
		{
			Vec2 ToAgent = _moving_entity->getPos() - ent->getPos();

			//scale the force inversely proportional to the agents distance  
			//from its neighbor.
			steering_force += (ToAgent).getNormalized() / ToAgent.getLength();
		}
	}
	return steering_force;
}

//---------------------------- alignment ---------------------------------
//
//  returns a force that attempts to align this agents heading with that
//  of its neighbors
//------------------------------------------------------------------------
Vec2 SteeringBehavior::alignment(const vector<std::unique_ptr<AbstCharacter> >& neighbors)
{
	//used to record the average heading of the neighbors
	Vec2 average_heading;

	//used to count the number of vehicles in the neighborhood
	int    neighbor_count = 0;

	//iterate through all the tagged vehicles and sum their heading vectors  
	for (unsigned int a = 0; a < neighbors.size(); ++a)
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined  is close enough ***also make sure it doesn't
		//include any evade target ***
		if ((neighbors[a].get() != _moving_entity) && neighbors[a]->isTagged() &&
			(neighbors[a].get() != _target_agent))
		{
			average_heading += neighbors[a]->getHeading();
			++neighbor_count;
		}
	}

	//if the neighborhood contained one or more vehicles, average their
	//heading vectors.
	if (neighbor_count > 0)
	{
		average_heading = average_heading / (double)neighbor_count;
		average_heading -= _moving_entity->getHeading();
	}

	return average_heading;
}

Vec2 SteeringBehavior::alignmentCellSpace()
{
	//This will record the average heading of the neighbors
	Vec2 average_heading;

	//This count the number of vehicles in the neighborhood
	double    neighbor_count = 0.0;

	//iterate through the neighbors and sum up all the position vectors
	for (MovingEntity* ent = _game_world->getGameMap()->getAgentCellSpace()->begin();
		!_game_world->getGameMap()->getAgentCellSpace()->end();
		ent = _game_world->getGameMap()->getAgentCellSpace()->next())
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined  is close enough
		if (ent != _moving_entity)
		{
			average_heading += ent->getHeading();
			++neighbor_count;
		}
	}

	//if the neighborhood contained one or more vehicles, average their
	//heading vectors.
	if (neighbor_count > 0.0)
	{
		average_heading = average_heading / neighbor_count;
		average_heading -= _moving_entity->getHeading();
	}
	return average_heading;
}

//-------------------------------- cohesion ------------------------------
//
//  returns a steering force that attempts to move the agent towards the
//  center of mass of the agents in its immediate area
//------------------------------------------------------------------------
Vec2 SteeringBehavior::cohesion(const vector<std::unique_ptr<AbstCharacter> >& neighbors)
{
	//first find the center of mass of all the agents
	Vec2 center_of_mass, steering_force;

	int neighbor_count = 0;

	//iterate through the neighbors and sum up all the position vectors
	for (unsigned int a = 0; a < neighbors.size(); ++a)
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined is close enough ***also make sure it doesn't
		//include the evade target ***
		if ((neighbors[a].get() != _moving_entity) && neighbors[a]->isTagged() &&
			(neighbors[a].get() != _target_agent))
		{
			center_of_mass += neighbors[a]->getPos();

			++neighbor_count;
		}
	}

	if (neighbor_count > 0)
	{
		//the center of mass is the average of the sum of positions
		center_of_mass = center_of_mass / (double)neighbor_count;

		//now seek towards that position
		steering_force = seek(center_of_mass);
	}
	
	//the magnitude of cohesion is usually much larger than separation or
	//allignment so it usually helps to normalize it.
	return steering_force.getNormalized();
}

Vec2 SteeringBehavior::cohesionCellSpace()
{
	//first find the center of mass of all the agents
	Vec2 center_of_mass, steering_force;

	int neighbor_count = 0;

	//iterate through the neighbors and sum up all the position vectors
	for (BaseGameEntity* ent = _game_world->getGameMap()->getAgentCellSpace()->begin();
		!_game_world->getGameMap()->getAgentCellSpace()->end();
		ent = _game_world->getGameMap()->getAgentCellSpace()->next())
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined is close enough
		if (ent != _moving_entity)
		{
			center_of_mass += ent->getPos();

			++neighbor_count;
		}
	}

	if (neighbor_count > 0)
	{
		//the center of mass is the average of the sum of positions
		center_of_mass = center_of_mass / (double)neighbor_count;

		//now seek towards that position
		steering_force = seek(center_of_mass);
	}

	//the magnitude of cohesion is usually much larger than separation or
	//allignment so it usually helps to normalize it.
	return (steering_force).getNormalized();
}



//------------------------------- followPath -----------------------------
//
//  Given a series of Vector2Ds, this method produces a force that will
//  move the agent along the waypoints in order. The agent uses the
// 'seek' behavior to move to the next waypoint - unless it is the last
//  waypoint, in which case it 'Arrives'
//------------------------------------------------------------------------

Vec2 SteeringBehavior::followPath()
{
	//move to next target if close enough to current target (working in
	//distance squared space)

	if (_path->currentWaypoint().distance(_moving_entity->getPos()) < _waypoint_seek_distSq)
	{
		_path->setNextWaypoint();
	}


	if (!_path->finished())
	{
		return seek(_path->currentWaypoint());
	}

	else
	{
		return arrive(_path->currentWaypoint(), NORMAL);
	}
}

//------------------------- Offset pursuit -------------------------------
//
//  Produces a steering force that keeps a vehicle at a specified offset
//  from a leader vehicle
//------------------------------------------------------------------------
Vec2 SteeringBehavior::offsetPursuit(
	const AbstCharacter* const leader,
	const Vec2& offset)
{
	//calculate the offset's position in world space
	Vec2 world_offset_pos = PointToWorldSpace(offset,
		leader->getHeading(),
		leader->getSide(),
		leader->getPos());

	Vec2 to_offset = world_offset_pos - _moving_entity->getPos();

	//the lookahead time is propotional to the distance between the leader
	//and the pursuer; and is inversely proportional to the sum of both
	//agent's velocities
	double look_ahead_time = to_offset.getLength() /
		(_moving_entity->getMaxSpeed() + leader->getMaxSpeed());

	//now arrive at the predicted future position of the offset
	return arrive(world_offset_pos + leader->getVelocity() * look_ahead_time, FAST);
}


//---------------------- obstacleAvoidance -------------------------------
//
//  Given a vector of CObstacles, this method returns a steering force
//  that will prevent the agent colliding with the closest obstacle
//------------------------------------------------------------------------
Vec2 SteeringBehavior::obstacleAvoidance(
	const std::vector<std::unique_ptr<CollisionArea> >& obstacles)
{
	//the detection box length is proportional to the agent's velocity
	_box_length = Prm.MinDetectionBoxLength +
		(_moving_entity->getSpeed() / _moving_entity->getMaxSpeed()) *
		Prm.MinDetectionBoxLength;

	//tag all obstacles within range of the box for processing

	_game_world->getGameMap()->tagObstaclesWithinViewRange(_moving_entity, _box_length);


	//this will keep track of the closest intersecting obstacle (CIB)
	CollisionArea* closest_intersecting_obstacle = nullptr;

	//this will be used to track the distance to the CIB
	double dist_to_closest_ip = std::numeric_limits<double>::max();

	//this will record the transformed local coordinates of the CIB
	Vec2 local_pos_of_closest_obstacle;

	std::vector<std::unique_ptr<CollisionArea> >::const_iterator cur_ob = obstacles.begin();

	while (cur_ob != obstacles.end())
	{
		//if the obstacle has been tagged within range proceed
		if ((*cur_ob)->isTagged())
		{

			//calculate this obstacle's position in local space
			Vec2 local_pos = PointToLocalSpace((*cur_ob)->getPos(),
				_moving_entity->getHeading(),
				_moving_entity->getSide(),
				_moving_entity->getPos());

			//if the local position has a negative x value then it must lay
			//behind the agent. (in which case it can be ignored)
			if (local_pos.x >= 0)
			{
				//if the distance from the x axis to the object's position is less
				//than its radius + half the width of the detection box then there
				//is a potential intersection.
				double expanded_radius = (*cur_ob)->getBRadius() + _moving_entity->getBRadius();

				if (fabs(local_pos.y) < expanded_radius)
				{
					//now to do a line/circle intersection test. The center of the 
					//circle is represented by (cX, cY). The intersection points are 
					//given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
					//We only need to look at the smallest positive value of x because
					//that will be the closest point of intersection.
					double cx = local_pos.x;
					double cy = local_pos.y;

					//we only need to calculate the sqrt part of the above equation once
					double SqrtPart = sqrt(expanded_radius*expanded_radius - cy*cy);

					double ip = cx - SqrtPart;

					if (ip <= 0.0)
					{
						ip = cx + SqrtPart;
					}

					//test to see if this is the closest so far. If it is keep a
					//record of the obstacle and its local coordinates
					if (ip < dist_to_closest_ip)
					{
						dist_to_closest_ip = ip;

						closest_intersecting_obstacle = (*cur_ob).get();
						//delete closest_intersecting_obstacle;
						local_pos_of_closest_obstacle = local_pos;
					}
				}
			}
		}

		cur_ob++;
	}

	//if we have found an intersecting obstacle, calculate a steering 
	//force away from it
	Vec2 steering_force;

	if (closest_intersecting_obstacle)
	{

		//the closer the agent is to an object, the stronger the 
		//steering force should be
		double multiplier = 1.0 + (_box_length - local_pos_of_closest_obstacle.x) /
			_box_length;

		//calculate the lateral force
		steering_force.y = (closest_intersecting_obstacle->getBRadius() -
			local_pos_of_closest_obstacle.y)  * multiplier;

		//apply a braking force proportional to the obstacles distance from
		//the vehicle. 
		const double braking_weight = 0.2;

		steering_force.x = (closest_intersecting_obstacle->getBRadius() -
			local_pos_of_closest_obstacle.x) *
			braking_weight;
	}

	//finally, convert the steering vector from local to world space
	return VectorToWorldSpace(steering_force,
		_moving_entity->getHeading(),
		_moving_entity->getSide());
}


//--------------------------- Interpose ----------------------------------
//
//  Given two agents, this method returns a force that attempts to 
//  position the vehicle between them
//------------------------------------------------------------------------

Vec2 SteeringBehavior::interpose(const AbstCharacter* agentA, const AbstCharacter* agentB)
{
	//first we need to figure out where the two agents are going to be at 
	//time T in the future. This is approximated by determining the time
	//taken to reach the mid way point at the current time at at max speed.
	Vec2 mid_point = (agentA->getPos() + agentB->getPos()) / 2.0;

	double time_to_reach_mid_point = 
		(_moving_entity->getPos(), mid_point).getLength() /
		_moving_entity->getMaxSpeed();

	//now we have T, we assume that agent A and agent B will continue on a
	//straight trajectory and extrapolate to get their future positions
	Vec2 posA = agentA->getPos() + agentA->getVelocity() * time_to_reach_mid_point;
	Vec2 posB = agentB->getPos() + agentB->getVelocity() * time_to_reach_mid_point;

	//calculate the mid point of these predicted positions
	mid_point = (posA + posB) / 2.0;

	//then steer to Arrive at it
	return arrive(mid_point, FAST);
}


//given another agent position to hide from and a list of BaseGameEntitys this
//method attempts to put an obstacle between itself and its opponent
Vec2 SteeringBehavior::hide(
	const AbstCharacter* hunter,
	const std::vector<std::unique_ptr<CollisionArea> >& obstacles)
{
	double dist_to_closest = std::numeric_limits<double>::max();
	Vec2 best_hiding_spot;

	std::vector<std::unique_ptr<CollisionArea> >::const_iterator ob(obstacles.begin());
	std::vector<std::unique_ptr<CollisionArea> >::const_iterator closest;

	while (ob != obstacles.end()) {
		//calculate the position of the hiding spot for this obstacle
		Vec2 hiding_spot = getHidingPosition(
			(*ob)->getPos(),
			(*ob)->getBRadius(),
			hunter->getPos());

		//work in distance-squared space to find the closest hiding
		//spot to the agent
		double dist = (hiding_spot, _moving_entity->getPos()).getLengthSq();

		if (dist < dist_to_closest) {
			dist_to_closest = dist;
			best_hiding_spot = hiding_spot;
			closest = ob;
		}
		++ob;
	}//end while

	 //if no suitable obstacles found then Evade the hunter
	if (dist_to_closest == std::numeric_limits<double>::max())
		return evade(hunter);

	//else use Arrive on the hiding spot
	return arrive(best_hiding_spot, FAST);
}

//------------------------- GetHidingPosition ----------------------------
//
//  Given the position of a hunter, and the position and radius of
//  an obstacle, this method calculates a position distance_from_boundary 
//  away from its bounding radius and directly opposite the hunter
//------------------------------------------------------------------------
Vec2 SteeringBehavior::getHidingPosition(
	const Vec2&		pos_ob,
	const double	radius_ob,
	const Vec2&		pos_hunter)
{
	//calculate how far away the agent is to be from the chosen obstacle's
	//bounding radius
	const double distance_from_boundary = 30.0;
	double       dist_away = radius_ob + distance_from_boundary;

	//calculate the heading toward the object from the hunter
	Vec2 to_ob = (pos_ob - pos_hunter).getNormalized();

	//scale it to size and add to the obstacles position to get
	//the hiding spot.
	return (to_ob * dist_away) + pos_ob;
}

//------------------------------- CreateFeelers --------------------------
//
//  Creates the antenna utilized by WallAvoidance
//------------------------------------------------------------------------
void SteeringBehavior::createFeelers()
{
	//feeler pointing straight in front
	_feelers[0] = _moving_entity->getPos() + _wall_detection_feeler_length * _moving_entity->getHeading();

	//feeler to left
	Vec2 temp = _moving_entity->getHeading();
	Vec2DRotateAroundOrigin(temp, M_PI / 2 * 3.5f);
	_feelers[1] = _moving_entity->getPos() + _wall_detection_feeler_length / 2.0f * temp;

	//feeler to right
	temp = _moving_entity->getHeading();
	Vec2DRotateAroundOrigin(temp, M_PI / 2 * 0.5f);
	_feelers[2] = _moving_entity->getPos() + _wall_detection_feeler_length / 2.0f * temp;
}


void SteeringBehavior::createRandomPath(int num_waypoints, int mx, int my, int cx, int cy) const
{
	_path->createRandomPath(num_waypoints, mx, my, cx, cy);
}

void SteeringBehavior::fleeOn() 
{
	_behavior_set |= FLEE; 
}

void SteeringBehavior::seekOn() 
{
	_behavior_set |= SEEK; 
}

void SteeringBehavior::arriveOn() 
{
	_behavior_set |= ARRIVE; 
}

void SteeringBehavior::wanderOn()
{
	_behavior_set |= WANDER; 
}

void SteeringBehavior::pursuitOn(AbstCharacter* const v)
{
	_behavior_set |= PURSUIT;
	_target_agent = v; 
}

void SteeringBehavior::evadeOn(AbstCharacter* const v)
{
	_behavior_set |= EVADE;
	_target_agent = v; 
}

void SteeringBehavior::cohesionOn() 
{
	_behavior_set |= COHESION; 
}

void SteeringBehavior::separationOn() 
{
	_behavior_set |= SEPARATION; 
}

void SteeringBehavior::alignmentOn() 
{
	_behavior_set |= ALIGNMENT; 
}

void SteeringBehavior::obstacleAvoidanceOn() 
{
	_behavior_set |= OBSTACLE_AVOIDANCE; 
}

void SteeringBehavior::wallAvoidanceOn() 
{
	_behavior_set |= WALL_AVOIDANCE; 
}

void SteeringBehavior::followPathOn() 
{
	_behavior_set |= FOLLOW_PATH; 
}

void SteeringBehavior::interposeOn(AbstCharacter* v1, AbstCharacter* v2)
{
	_behavior_set |= INTERPOSE;
	_target_agent = v1;
	_target_agent2 = v2; 
}

void SteeringBehavior::hideOn(AbstCharacter* const v)
{
	_behavior_set |= HIDE;
	_target_agent = v; 
}

void SteeringBehavior::offsetPursuitOn(AbstCharacter* const v, const cocos2d::Vec2& offset)
{
	_behavior_set |= OFFSET_PURSUIT; 
	_offset = offset; _target_agent = v; 
}

void SteeringBehavior::flockingOn() 
{
	cohesionOn(); 
	alignmentOn(); 
	separationOn(); 
	wanderOn(); 
}

void SteeringBehavior::fleeOff() 
{
	if (isOn(FLEE))   
		_behavior_set ^= FLEE; 
}

void SteeringBehavior::seekOff() 
{
	if (isOn(SEEK))   
		_behavior_set ^= SEEK; 
}

void SteeringBehavior::arriveOff() 
{
	if (isOn(ARRIVE)) 
		_behavior_set ^= ARRIVE; 
}

void SteeringBehavior::wanderOff() 
{
	if (isOn(WANDER)) 
		_behavior_set ^= WANDER; 
}

void SteeringBehavior::pursuitOff() 
{
	if (isOn(PURSUIT)) 
		_behavior_set ^= PURSUIT; 
}

void SteeringBehavior::evadeOff() 
{
	if (isOn(EVADE)) 
		_behavior_set ^= EVADE; 
}

void SteeringBehavior::cohesionOff() 
{
	if (isOn(COHESION)) 
		_behavior_set ^= COHESION; 
}

void SteeringBehavior::separationOff() 
{
	if (isOn(SEPARATION)) 
		_behavior_set ^= SEPARATION; 
}

void SteeringBehavior::alignmentOff() 
{
	if (isOn(ALIGNMENT)) 
		_behavior_set ^= ALIGNMENT; 
}

void SteeringBehavior::obstacleAvoidanceOff() 
{
	if (isOn(OBSTACLE_AVOIDANCE)) 
		_behavior_set ^= OBSTACLE_AVOIDANCE; 
}

void SteeringBehavior::wallAvoidanceOff() 
{
	if (isOn(WALL_AVOIDANCE)) 
		_behavior_set ^= WALL_AVOIDANCE; 
}

void SteeringBehavior::followPathOff() 
{
	if (isOn(FOLLOW_PATH)) 
		_behavior_set ^= FOLLOW_PATH; 
}

void SteeringBehavior::interposeOff() 
{
	if (isOn(INTERPOSE)) 
		_behavior_set ^= INTERPOSE; 
}

void SteeringBehavior::hideOff() 
{
	if (isOn(HIDE)) 
		_behavior_set ^= HIDE; 
}

void SteeringBehavior::offsetPursuitOff() 
{
	if (isOn(OFFSET_PURSUIT)) 
		_behavior_set ^= OFFSET_PURSUIT; 
}

void SteeringBehavior::flockingOff() 
{
	cohesionOff();
	alignmentOff();
	separationOff(); 
	wanderOff(); 
}

bool SteeringBehavior::isFleeOn() 
{
	return isOn(FLEE); 
}

bool SteeringBehavior::isSeekOn() 
{
	return isOn(SEEK); 
}

bool SteeringBehavior::isArriveOn() 
{
	return isOn(ARRIVE); 
}

bool SteeringBehavior::isWanderOn() 
{
	return isOn(WANDER); 
}

bool SteeringBehavior::isPursuitOn() 
{
	return isOn(PURSUIT); 
}

bool SteeringBehavior::isEvadeOn() 
{
	return isOn(EVADE); 
}

bool SteeringBehavior::isCohesionOn() 
{
	return isOn(COHESION); 
}

bool SteeringBehavior::isSeparationOn() 
{
	return isOn(SEPARATION); 
}

bool SteeringBehavior::isAlignmentOn() 
{
	return isOn(ALIGNMENT); 
}

bool SteeringBehavior::isObstacleAvoidanceOn() 
{
	return isOn(OBSTACLE_AVOIDANCE); 
}

bool SteeringBehavior::isWallAvoidanceOn() 
{
	return isOn(WALL_AVOIDANCE); 
}

bool SteeringBehavior::isFollowPathOn() 
{
	return isOn(FOLLOW_PATH); 
}

bool SteeringBehavior::isInterposeOn() 
{
	return isOn(INTERPOSE); 
}

bool SteeringBehavior::isHideOn() 
{
	return isOn(HIDE); 
}

bool SteeringBehavior::isOffsetPursuitOn() 
{
	return isOn(OFFSET_PURSUIT); 
}

//this function tests if a specific bit of _flag is set
bool SteeringBehavior::isOn(behavior_type bt) 
{
	return (_behavior_set & bt) == bt; 
}

void SteeringBehavior::setTarget(const cocos2d::Vec2& t) 
{
	_target = t; 
}

const cocos2d::Vec2& SteeringBehavior::getTarget() const
{
	return _target;
}

void SteeringBehavior::setTargetAgent(AbstCharacter* const agent)
{
	_target_agent = agent; 
}

AbstCharacter* SteeringBehavior::getTargetAgent() const
{
	return _target_agent;
}

void SteeringBehavior::setOffset(const cocos2d::Vec2& offset) 
{
	_offset = offset; 
}

const cocos2d::Vec2&  SteeringBehavior::getOffset() const 
{
	return _offset; 
}

void SteeringBehavior::setPath(const std::list<cocos2d::Vec2>& new_path) 
{
	_path->setPath(new_path); 
}

const cocos2d::Vec2& SteeringBehavior::getSteeringForce() const 
{
	return _steering_force; 
}

double SteeringBehavior::getWeightSeparation()const 
{
	return _weight_separation; 
}

double SteeringBehavior::getWeightAlignment()const 
{
	return _weight_alignment; 
}

double SteeringBehavior::getWeightCohesion()const 
{
	return _weight_cohesion; 
}

double SteeringBehavior::getWanderJitter()const 
{
	return _wander_jitter; 
}

double SteeringBehavior::getWanderDistance()const 
{
	return _wander_distance; 
}

double SteeringBehavior::getWanderRadius()const 
{
	return _wander_radius; 
}

int SteeringBehavior::getBehaviorSet()const 
{
	return _behavior_set; 
}

double SteeringBehavior::getBoxLength()const 
{
	return _box_length; 
}

void SteeringBehavior::setBoxLength(double box_length) 
{
	_box_length = box_length;
}

double SteeringBehavior::getWallDetectionFeelerLength()const 
{
	return _wall_detection_feeler_length; 
}

void SteeringBehavior::setPath(double feeler_length) 
{
	_wall_detection_feeler_length = feeler_length; 
}

double SteeringBehavior::getViewDistance() const
{
	return _view_distance;
}

void SteeringBehavior::setViewDistance(double view_distance)
{
	_view_distance = view_distance;
}

const std::vector<cocos2d::Vec2>& SteeringBehavior::get_feelers()const 
{
	return _feelers; 
}

void SteeringBehavior::setFeelers(const std::vector<cocos2d::Vec2>& feelers) 
{
	_feelers = feelers; 
}