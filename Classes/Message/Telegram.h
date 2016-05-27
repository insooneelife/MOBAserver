#ifndef TELEGRAM_H
#define TELEGRAM_H
//------------------------------------------------------------------------
//
//  Name:   Telegram.h
//
//  Desc:   This defines a telegram. A telegram is a data structure that
//          records information required to dispatch messages. Messages 
//          are used by game agents to communicate with each other.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <iostream>
#include <math.h>


struct Telegram {

	//the entity that sent this telegram
	unsigned int		sender;

	//the entity that is to receive this telegram
	unsigned int		receiver;

	//the message itself. These are all enumerated in the file
	//"MessageTypes.h"
	int					msg;

	//messages can be dispatched immediately or delayed for a specified amount
	//of time. If a delay is necessary this field is stamped with the time 
	//the message should be dispatched.
	double       dispatch_time;

	//any additional information that may accompany the message
	void*        extra_info;

	Telegram() :dispatch_time(-1), sender(0), receiver(0), msg(-1) {}

	Telegram(
		double time,
		unsigned int sender,
		unsigned int receiver,
		int msg,
		void* info = nullptr)
		:
		dispatch_time(time),
		sender(sender),
		receiver(receiver),
		msg(msg),
		extra_info(info) 
	{}

};

//these telegrams will be stored in a priority queue.
//Therefore the operator< needs to be overloaded so that the PQ can sort the telegrams by time priority.
//Note how the times must be smaller than
//SmallestDelay apart before two Telegrams are considered unique.
const double SmallestDelay = 0.25;

inline bool operator==(const Telegram& t1, const Telegram& t2) 
{
	return	(fabs(t1.dispatch_time - t2.dispatch_time) < SmallestDelay) &&
		(t1.sender == t2.sender) &&
		(t1.receiver == t2.receiver) &&
		(t1.msg == t2.msg);
}

inline bool operator<(const Telegram& t1, const Telegram& t2) {
	if (t1 == t2)
		return false;
	else
		return  (t1.dispatch_time < t2.dispatch_time);
}

inline std::ostream& operator<<(std::ostream& os, const Telegram& t) 
{
	os << "time: " << t.dispatch_time << "  sender: " << t.sender
		<< "   receiver: " << t.receiver << "   msg: " << t.msg;

	return os;
}

//handy helper function for dereferencing the getExtraInfo field of the Telegram to the required type.
template <class T>
inline T dereferenceToType(void* p) 
{
	return *(T*)(p);
}


#endif