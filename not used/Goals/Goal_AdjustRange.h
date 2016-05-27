#ifndef GOAL_ADJUST_RANGE_H
#define GOAL_ADJUST_RANGE_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Goal_AdjustRange.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   
//
//-----------------------------------------------------------------------------
#include "goals/Goal.h"
#include "Raven_Goal_Types.h"
#include "../AbstRaven_Bot.h"





class Goal_AdjustRange : public Goal<AbstRaven_Bot>
{
protected:

  AbstRaven_Bot*  m_pTarget;

  double       m_dIdealRange;

public:

  Goal_AdjustRange(AbstRaven_Bot* pBot);

  void Activate();

  int  Process();

  void Terminate();
 
};






#endif
