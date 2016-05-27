#ifndef GOAL_WANDER_H
#define GOAL_WANDER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Goal_Wander.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Causes a bot to wander until terminated
//-----------------------------------------------------------------------------
#include "Goals/Goal.h"
#include "Raven_Goal_Types.h"
#include "../AbstRaven_Bot.h"


class Goal_Wander : public Goal<AbstRaven_Bot>
{
protected:

public:

  Goal_Wander(AbstRaven_Bot* pBot):Goal<AbstRaven_Bot>(pBot,goal_wander)
  {}

  void Activate();

  int  Process();

  void Terminate();
};





#endif
