#include "CharacterBaseStates.h"
#include "Character.h"
#include "GameWorld.h"
#include "Util.h"
#include "MessageDispatcher.h"
#include "StateMachine.h"
#include "Motion.h"
#include "Telegram.h"
#include "SteeringBehavior.h"
#include "cocos2d.h"

USING_NS_CC;

//************************************************************************ GLOBAL STATE

void CharacterBaseGlobalState::execute(Character* character)
{
	/*
	if (character->getSteering()->getTargetAgent() != nullptr) {
		if(util::inRange(
			character->getPos(),
			character->getSteering()->getTargetAgent()->getPos(),
			character->getSteering()->getViewDistance())) 
		{
			character->getSteering()->setTargetAgent(nullptr);
			character->getSteering()->pursuitOff();
		}
	}

	MovingEntity* temp = character->findEnemyInRange();
	if (temp != nullptr)
	{
		character->getSteering()->pursuitOn(temp);
	}
	*/
}


bool CharacterBaseGlobalState::onMessage(Character* character, const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case 1:
	{
		return true;
	}
	break;

	case 2:
	{
		/*
		//let the receiver know a pass is coming 
		Dispatcher.dispatchMsg(
			SEND_MSG_IMMEDIATELY,
			character->getId(),
			receiver->ID(),
			Msg_ReceiveBall,
			&receiver->Pos());

		//change state   
		character->GetFSM()->ChangeState(Wait::Instance());

		character->FindSupport();
		*/
		return true;
	}

	break;

	}//end switch

	return false;
}

//***************************************************************************** Neutral

void Neutral::enter(Character* character)
{
	character->setVelocity(Vec2(0, 0));
	character->setMotionDirect("Neutral");
	character->setStateName("Neutral");
}

void Neutral::execute(Character* character)
{
	character->setVelocity(Vec2(0, 0));

	MovingEntity* temp = character->findEnemyInRange();
	if (temp == nullptr)
	{
		if (util::genRand(0, 100) < 3)
		{
			character->getFSM()->changeState(&Idle::getInstance());
		}
		return;
	}
	//if found
	character->getSteering()->setTargetAgent(temp);
	character->getFSM()->changeState(&ChaseEnemy::getInstance());
}


//***************************************************************************** Idle

void Idle::enter(Character* character)
{
	Vec2 pos = character->getPos();
	Vec2 target_pos;

	while (1)
	{
		target_pos = pos + util::makeRandomPosition(400, 500);
		if (0 < target_pos.x && target_pos.x < character->getWorld()->getGameMap()->getSizeX() &&
			0 < target_pos.y && target_pos.y < character->getWorld()->getGameMap()->getSizeY())
		{
			break;
		}
	}
	
	character->getSteering()->setTarget(target_pos);
	character->getSteering()->seekOn();

	character->GetPathPlanner()->RequestPathToPosition(target_pos);

	character->setHeading(character->getHeading());
	character->setMotionDirect("Walk");
	character->setStateName("Idle");
}


void Idle::execute(Character* character)
{
	character->setHeading(character->getHeading());

	MovingEntity* temp = character->findEnemyInRange();
	if (temp == nullptr)
	{
		if(character->targetInRange(character->getBoundingRadius() / 2)) 
		{
			character->getFSM()->changeState(&Neutral::getInstance());
		}

		return;
	}
	
	//if found
	character->getSteering()->setTargetAgent(temp);
	character->getFSM()->changeState(&ChaseEnemy::getInstance());
}


void Idle::exit(Character* character)
{
	character->getSteering()->seekOff();
}


//***************************************************************************** CHASEENEMY

void ChaseEnemy::enter(Character* character)
{
	character->setHeading(character->getHeading());
	character->setMotionDirect("Walk");
	character->setStateName("ChaseEnemy");
	character->getSteering()->pursuitOn(character->getSteering()->getTargetAgent());
}

void ChaseEnemy::execute(Character* character)
{
	character->setHeading(character->getHeading());

	if (character->inAttackRange())
	{
		character->getFSM()->changeState(&Attack::getInstance());
		return;
	}

	if (!character->targetInRange(character->getSteering()->getViewDistance())
		&& character->getMotion()->getFrameChanged())
	{
		character->getSteering()->setTargetAgent(nullptr);
		character->getFSM()->changeState(&Neutral::getInstance());
		return;
	}
}


void ChaseEnemy::exit(Character* character)
{
	character->getSteering()->pursuitOff();
}

//***************************************************************************** Attack

void Attack::enter(Character* character)
{
	character->setHeading(character->getAttackDirection());
	character->setMotionDirect("Attack1");
	character->setStateName("Attack");
	character->setVelocity(Vec2(0, 0));
	if (!character->inAttackRange())
	{
		character->getFSM()->changeState(&ChaseEnemy::getInstance());
		return;
	}
}

void Attack::execute(Character* character)
{
	if (character->getMotion()->getFrameChanged())
	{
		character->getFSM()->changeState(&Attack::getInstance());
		return;
	}
}


void Attack::exit(Character* character)
{}




