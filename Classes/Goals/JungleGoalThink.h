#ifndef JUNGLE_GOAL_THINK_H
#define JUNGLE_GOAL_THINK_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   JungleGoalThink .h
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


class JungleGoalThink : public GoalThink
{
public:

	JungleGoalThink(AbstCharacter* const bot, const cocos2d::Vec2& destination);

	virtual ~JungleGoalThink() override
	{}
};


#endif