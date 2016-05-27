#pragma once

#include <set>
#include <deque>
#include <iostream>
#include "ChatMessage.hpp"

class ChatParticipant;

class ChatRoom
{
public:
	// 세션(참여자)를 채팅방에 참여시킨다.
	void join(boost::shared_ptr<ChatParticipant> participant);

	// 세션(참여자)를 채팅방에서 제거한다.
	void leave(boost::shared_ptr<ChatParticipant> participant);

	// 채팅방으로 msg를 뿌린다.
	void deliver(const ChatMessage& msg);

private:

	// 채팅방 참여자 리스트
	std::set< boost::shared_ptr<ChatParticipant> > _participants;
	
	// 최대 허용 msg
	enum { MAX_RECENT_MSG = 100 };

	// msg 리스트
	std::deque<ChatMessage> _recent_msgs;
};