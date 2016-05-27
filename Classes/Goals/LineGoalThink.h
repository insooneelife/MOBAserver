#ifndef LINE_GOAL_THINK_H
#define LINE_GOAL_THINK_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   LineGoalThink .h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <memory>
#include "cocos2d.h"
#include "GoalThink.h"


class LineGoalThink : public GoalThink
{
public:

	LineGoalThink(AbstCharacter* const bot);

	virtual ~LineGoalThink() override
	{};
};


#endif