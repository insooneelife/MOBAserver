#include "Goal_AdjustRange.h"
#include "../AbstRaven_Bot.h"
#include "../AbstSteeringBehaviors.h"
#include "../armory/Raven_Weapon.h"



Goal_AdjustRange::Goal_AdjustRange(AbstRaven_Bot* pBot):Goal<AbstRaven_Bot>(pBot, goal_adjust_range),
                                                    m_dIdealRange(0)
{}

//---------------------------- Initialize -------------------------------------
//-----------------------------------------------------------------------------  
void Goal_AdjustRange::Activate()
{
  m_pOwner->GetSteering()->SetTarget(m_pOwner->GetTargetBot()->Pos());


}

//------------------------------ Process --------------------------------------
//-----------------------------------------------------------------------------
int Goal_AdjustRange::Process()
{
  //if status is inactive, call Activate()
  ActivateIfInactive();

  /*
  if (m_pOwner->GetCurrentWeapon()->isInIdealWeaponRange())
  {
    m_iStatus = completed;
  }
*/
  return m_iStatus;
}

//---------------------------- Terminate --------------------------------------
//-----------------------------------------------------------------------------
void Goal_AdjustRange::Terminate()
{




}

