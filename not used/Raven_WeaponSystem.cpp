#include "Raven_WeaponSystem.h"
#include "../../armory/Weapon_RocketLauncher.h"
#include "../../armory/Weapon_RailGun.h"
#include "../../armory/Weapon_ShotGun.h"
#include "../../armory/Weapon_Blaster.h"
#include "../../AbstRaven_Bot.h"
#include "misc/utils.h"
#include "../../Raven_Game.h"
#include "../../Raven_UserOptions.h"
#include "2D/transformations.h"



//------------------------- ctor ----------------------------------------------
//-----------------------------------------------------------------------------
Raven_WeaponSystem::Raven_WeaponSystem(AbstRaven_Bot* owner,
                                       double ReactionTime,
                                       double AimAccuracy,
                                       double AimPersistance): 
	AbstWeaponSystem(owner, ReactionTime, AimAccuracy, AimPersistance)

{

}
