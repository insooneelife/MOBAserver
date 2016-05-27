#include "MessageDispatcher.h"
#include "../BaseGameEntity.h"
#include "../Timer/FrameCounter.h"
#include "../EntityManager.h"

//uncomment below to send message info to the debug window
//#define SHOW_MESSAGING_INFO

//----------------------------- Dispatch ---------------------------------
//  
//  see description in header
//------------------------------------------------------------------------
void MessageDispatcher::discharge(
	BaseGameEntity* const receiver,
	const Telegram& telegram)
{
	if (receiver == nullptr)
		return;

	if (!receiver->handleMessage(telegram))
	{
		//telegram could not be handled
#ifdef SHOW_MESSAGING_INFO
		debug_con << "Message not handled" << "";
#endif
	}
}

//---------------------------- DispatchMsg ---------------------------
//
//  given a message, a receiver_id, a sender_id and any time delay, this function
//  routes the message to the correct agent (if no delay) or stores
//  in the message queue to be dispatched at the correct time
//------------------------------------------------------------------------
void MessageDispatcher::dispatchMsg(
	double			delay,
	unsigned int	sender_id,
	unsigned int	receiver_id,
	int				msg,
	void* const		extra_info = nullptr)
{

	//get a pointer to the receiver
	BaseGameEntity* const receiver = EntityMgr.getEntityFromID(receiver_id);

	//make sure the receiver is valid
	if (receiver == nullptr)
	{
#ifdef SHOW_MESSAGING_INFO
		debug_con << "\nWarning! No Receiver with ID of " << receiver_id << " found" << "";
#endif

		return;
	}

	//create the telegram
	Telegram telegram(0, sender_id, receiver_id, msg, extra_info);

	//if there is no delay, route telegram immediately                       
	if (delay <= 0.0)
	{
#ifdef SHOW_MESSAGING_INFO
		debug_con << "\nTelegram dispatched at time: " << TickCounter->GetCurrentFrame()
			<< " by " << sender_id << " for " << receiver_id
			<< ". Msg is " << msg << "";
#endif

		//send the telegram to the recipient
		discharge(receiver, telegram);
	}

	//else calculate the time when the telegram should be dispatched
	else
	{
		double current_time = FrameCnt.getCurrentFrame();

		telegram.dispatch_time = current_time + delay;

		//and put it in the queue
		_pque.insert(telegram);

#ifdef SHOW_MESSAGING_INFO
		debug_con << "\nDelayed telegram from " << sender_id << " recorded at time "
			<< TickCounter->GetCurrentFrame() << " for " << receiver_id
			<< ". Msg is " << msg << "";
#endif
	}
}

//---------------------- DispatchDelayedMessages -------------------------
//
//  This function dispatches any telegrams with a timestamp that has
//  expired. Any dispatched telegrams are removed from the queue
//------------------------------------------------------------------------
void MessageDispatcher::dispatchDelayedMessages()
{
	//first get current time
	double current_time = FrameCnt.getCurrentFrame();

	//now peek at the queue to see if any telegrams need dispatching.
	//remove all telegrams from the front of the queue that have gone
	//past their sell by date
	while (!_pque.empty() &&
		(_pque.begin()->dispatch_time < current_time) &&
		(_pque.begin()->dispatch_time > 0))
	{
		//read the telegram from the front of the queue
		const Telegram& telegram = *_pque.begin();

		//find the recipient
		BaseGameEntity* const receiver = EntityMgr.getEntityFromID(telegram.receiver);

#ifdef SHOW_MESSAGING_INFO
		debug_con << "\nQueued telegram ready for dispatch: Sent to "
			<< receiver->ID() << ". Msg is " << telegram.Msg << "";
#endif

		//send the telegram to the recipient
		discharge(receiver, telegram);

		//remove it from the queue
		_pque.erase(_pque.begin());
	}
}



