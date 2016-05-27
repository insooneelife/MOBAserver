#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   MessageDispatcher.h
//
//  Desc:   A message dispatcher. Manages messages of the type Telegram.
//          Instantiated as a singleton.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <set>
#include <string>
#include <memory>
#include "Telegram.h"
#include "../Singleton.h"

class BaseGameEntity;

//provide easy access
#define Dispatcher MessageDispatcher::getInstance()

class MessageDispatcher : public Singleton<MessageDispatcher> {
public:

	//send a message to another agent. Receiving agent is referenced by ID.
	void dispatchMsg(
		double			delay,
		unsigned int	sender,
		unsigned int	receiver,
		int				msg,
		void* const		extra_info);

	//send out any delayed messages. This method is called each time through   
	//the main game loop.
	void dispatchDelayedMessages();

private:
	MessageDispatcher(const MessageDispatcher&) = delete; // no copies
	MessageDispatcher& operator=(const MessageDispatcher&) = delete; // no self-assignments
	MessageDispatcher() {}
	friend class Singleton<MessageDispatcher>;

	//this method is utilized by DispatchMsg or DispatchDelayedMessages.
	//This method calls the message handling member function of the receiving
	//entity, pReceiver, with the newly created telegram
	void discharge(BaseGameEntity* const receiver, const Telegram& msg);

	//a std::set is used as the container for the delayed messages
	//because of the benefit of automatic sorting and avoidance
	//of duplicates. Messages are sorted by their dispatch time.
	std::set<Telegram> _pque;
};

#endif