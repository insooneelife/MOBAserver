#pragma once

class ChatMessage;

class ChatParticipant
{
public:
	virtual ~ChatParticipant() {}
	virtual void deliver(const ChatMessage& msg) = 0;
};