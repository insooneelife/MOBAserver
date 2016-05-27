#ifndef TOWER_GOAL_THINK_H
#define TOWER_GOAL_THINK_H
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


class TowerGoalThink : public GoalThink
{
public:

	TowerGoalThink(AbstCharacter* const bot);

	virtual ~TowerGoalThink() override
	{};
};


#endif