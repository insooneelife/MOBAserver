#include <sstream>
#include <string>
#include "Motion.h"
#include "Util.h"
#include "GlobalValues.h"
#include "Util.h"
#include "AbstCharacter.h"
#include "SteeringBehavior.h"

USING_NS_CC;	

const Motion::bm_direction_type Motion::direction_types_map =
boost::assign::list_of< bm_direction_type::relation >
(DOWN_8, "DOWN_8")
(LEFT_DOWN_8, "LEFT_DOWN_8")
(LEFT_8, "LEFT_8")
(LEFT_UP_8, "LEFT_UP_8")
(UP_8, "UP_8")
(RIGHT_UP_8, "RIGHT_UP_8")
(RIGHT_8, "RIGHT_8")
(RIGHT_DOWN_8, "RIGHT_DOWN_8");

Motion::~Motion() 
{}

Motion::Motion(
	const std::string& name,
	const std::string& foldername,
	AbstCharacter* const agent,
	const Vec2& pos,
	int direction,
	int dummy)
	:
	_agent(agent),
	_frame(0),
	_divide_frame(0),
	_active(true)
{
	rapidjson::Document json;
	util::readJSON(foldername + "/" + name + "/" + name + ".json", json);

	_direction_num = json["directions"].GetInt();

	for (rapidjson::Value::MemberIterator M = json["actions"].MemberBegin();
		M != json["actions"].MemberEnd(); M++) 
	{
		std::string key = M->name.GetString();
		_motions.insert(std::make_pair(key,
				std::make_tuple(
					M->value["frameSize"].GetInt(), 
					M->value["marginX"].GetInt(),
					M->value["marginY"].GetInt())));
	}

	setMotion("Neutral", direction);
}

void Motion::update(
	const std::string& new_motion,
	const Vec2& new_pos,
	int new_dir) 
{
	if (!_active)return;
	_frame = (_frame + 1) % (_divide_frame * (new_dir + 1));
	_frame_active = true;

	if (_frame == 0) {
		_frame_active = false;
		_frame = _divide_frame * new_dir;
		if (new_motion != _agent->getMotionName())
			setMotion(new_motion, new_dir);
	}
} 

void Motion::render()
{
	
}

bool Motion::setMotion(const std::string& new_motion, int new_dir)
{
	auto iter = _motions.find(new_motion);
	bool exist = false;

	if (iter != _motions.end()) {
		auto data = iter->second;
		_divide_frame = std::get<0>(data);
		exist = true;
	}

	_frame = _divide_frame * new_dir;
	return exist;
}


//getters & setters

const AbstCharacter* Motion::getAgent() const
{
	return _agent;
}

int Motion::getFrame() const 
{
	return _frame;
}

int Motion::getDivideFrame() const 
{
	return _divide_frame;
}

bool Motion::getFrameActive() const
{
	return _frame_active;
}

void Motion::setFrameActive(bool how)
{
	_frame_active = how;
}

bool Motion::getActive() const
{
	return _active;
}

void Motion::setActive(bool how)
{
	_active = how;
}

bool Motion::isDirectionEight() const
{
	return _direction_num == 8;
}