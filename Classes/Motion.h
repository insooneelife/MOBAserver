#pragma once
#include "cocos2d.h"
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include "GlobalValues.h"
#include "boost\bimap.hpp"
#include "boost\assign.hpp"

//------------------------------------------------------------------------
//
//  Name:   Motion.h
//
//  Desc:   This class selects animation frame, 
//			which is suitable for current state
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

class AbstCharacter;

class Motion {
public:
	enum DirectionType8 {
		DOWN_8, LEFT_DOWN_8, LEFT_8, LEFT_UP_8,
		UP_8, RIGHT_UP_8, RIGHT_8, RIGHT_DOWN_8,
		UNABLE_8
	};

	enum DirectionType16 {
		DOWN_16, LEFT_DOWN1_16, LEFT_DOWN2_16, LEFT_DOWN3_16,
		LEFT_16, LEFT_UP1_16, LEFT_UP2_16, LEFT_UP3_16,
		UP_16, RIGHT_UP1_16, RIGHT_UP2_16, RIGHT_UP3_16,
		RIGHT_16, RIGHT_DOWN1_16, RIGHT_DOWN2_16, RIGHT_DOWN3_16,
		UNABLE_16
	};

	typedef boost::bimaps::bimap< DirectionType8, std::string > bm_direction_type;
	static const bm_direction_type direction_types_map;

	Motion(
		const std::string&			name,
		const std::string&			foldername,
		AbstCharacter* const		agent,
		const cocos2d::Vec2&		pos,
		int							direction,
		int							dummy = 0);

	~Motion();

	int getFrame() const;

	int getMotionFrame() const;

	int getDivideFrame() const;

	bool getFrameActive() const;

	void setFrameActive(bool how);

	bool setMotion(const std::string& new_motion, int new_dir);

	bool getActive() const;

	void setActive(bool how);

	bool isDirectionEight() const;

	const AbstCharacter* getAgent() const;

	void update(const std::string& new_motion, const cocos2d::Vec2& new_pos, int new_dir);

	void render();

private:
	Motion() = delete;
	Motion(const Motion& copy) = delete;
	Motion& operator=(const Motion& copy) = delete;

	//only for reference
	std::map<std::string, std::tuple<int, int, int> >	_motions;
	AbstCharacter* const								_agent;

	int		_frame;
	int		_divide_frame;
	bool	_frame_active;
	bool	_active;
	int		_direction_num;
};