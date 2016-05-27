#include "WalkState.h"
#include "cocos2d.h"
#include "Util.h"
#include "Character.h"

USING_NS_CC;

void WalkState::execute(Character* character) {
	//if (util::inRange(character->getPos(), Vec2(50, 100), 1000)) {
	//	character->setPos(character->getPos() + Vec2(1, 1));
	//}
}

bool WalkState::onMessage(Character*, const Telegram&) {
	return 1;
}


/*
void GlobalPlayerState::Execute(FieldPlayer* player)
{
	//if a player is in possession and close to the ball reduce his max speed
	if ((player->BallWithinReceivingRange()) && (player->isControllingPlayer()))
	{
		player->SetMaxSpeed(Prm.PlayerMaxSpeedWithBall);
	}

	else
	{
		player->SetMaxSpeed(Prm.PlayerMaxSpeedWithoutBall);
	}

}


bool GlobalPlayerState::OnMessage(FieldPlayer* player, const Telegram& telegram)
{
	switch (telegram.Msg)
	{
	case Msg_ReceiveBall:
	{
		//set the target
		player->Steering()->SetTarget(*(static_cast<Vector2D*>(telegram.ExtraInfo)));

		//change state 
		player->GetFSM()->ChangeState(ReceiveBall::Instance());

		return true;
	}

	break;

	case Msg_SupportAttacker:
	{
		//if already supporting just return
		if (player->GetFSM()->isInState(*SupportAttacker::Instance()))
		{
			return true;
		}

		//set the target to be the best supporting position
		player->Steering()->SetTarget(player->Team()->GetSupportSpot());

		//change the state
		player->GetFSM()->ChangeState(SupportAttacker::Instance());

		return true;
	}

	break;

	case Msg_Wait:
	{
		//change the state
		player->GetFSM()->ChangeState(Wait::Instance());

		return true;
	}

	break;

	case Msg_GoHome:
	{
		player->SetDefaultHomeRegion();

		player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance());

		return true;
	}

	break;

	case Msg_PassToMe:
	{

		//get the position of the player requesting the pass 
		FieldPlayer* receiver = static_cast<FieldPlayer*>(telegram.ExtraInfo);

#ifdef PLAYER_STATE_INFO_ON
		debug_con << "Player " << player->ID() << " received request from " <<
			receiver->ID() << " to make pass" << "";
#endif

		//if the ball is not within kicking range or their is already a 
		//receiving player, this player cannot pass the ball to the player
		//making the request.
		if (player->Team()->Receiver() != NULL ||
			!player->BallWithinKickingRange()){
#ifdef PLAYER_STATE_INFO_ON
			debug_con << "Player " << player->ID() << " cannot make requested pass <cannot kick ball>" << "";
#endif

			return true;
		}

		//make the pass   
		player->Ball()->Kick(receiver->Pos() - player->Ball()->Pos(),
			Prm.MaxPassingForce);


#ifdef PLAYER_STATE_INFO_ON
		debug_con << "Player " << player->ID() << " Passed ball to requesting player" << "";
#endif

		//let the receiver know a pass is coming 
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			player->ID(),
			receiver->ID(),
			Msg_ReceiveBall,
			&receiver->Pos());



		//change state   
		player->GetFSM()->ChangeState(Wait::Instance());

		player->FindSupport();

		return true;
	}

	break;

	}//end switch

	return false;
}*/