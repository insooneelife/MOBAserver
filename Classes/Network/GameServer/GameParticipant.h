#pragma once

class GameMessage;

class GameParticipant
{
public:
	GameParticipant(unsigned int id) : _id(id) {}
	virtual ~GameParticipant() {}
	
	unsigned int getId() const
	{
		return _id;
	}
	
	virtual void send(const GameMessage& msg) = 0;

private:
	unsigned int _id;
};