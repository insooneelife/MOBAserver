#include "GameRoom.h"
#include <boost\asio.hpp>
#include <boost/bind.hpp>

#include "cocos2d.h"

#include "GameSession.h"
#include "GameMessage.hpp"
#include "GameServerManager.h"

#include "../Packet/PacketFactory.h"
#include "../../GameWorld.h"
#include "../../AbstCharacter.h"
#include "../../GlobalValues.h"
#include "../../ParameterLoader.h"

GameRoom::GameRoom() 
	:
	_client_ready_cnt(0),
	_room_state(LOGIN_SCENE)
{
	for (size_t i = 0; i < _slots.size(); i++)
	{
		_slots[i] = SLOT_EMPTY;
	}
}

// 세션(참여자)를 채팅방에 참여시키기 위해 command_que에 참여 명령을 넣는다.
void GameRoom::join(boost::shared_ptr<GameParticipant> participant)
{
	std::cout << "GameRoom : join" << std::endl;
	_room_state = TEAM_SELECT_SCENE;

	// 참여자 리스트에 추가
	_command_que.push(std::make_pair(JOIN, participant));
}

// 세션(참여자)를 채팅방에서 제거하기 위해 command_que에 제거 명령을 넣는다.
void GameRoom::leave(boost::shared_ptr<GameParticipant> participant)
{
	std::cout << "GameRoom : leave" << std::endl;
	_command_que.push(std::make_pair(LEAVE, participant));
}

void GameRoom::update()
{
	while (!_command_que.empty())
	{
		int command = _command_que.front().first;
		const boost::shared_ptr<GameParticipant>& participant = _command_que.front().second;
		unsigned int join_id = participant->getId();

		_command_que.pop();

		if (command == JOIN)
		{
			if (updateSlotAtJoin(participant))
				_participants.insert(std::make_pair(join_id, participant));
		}
		else if (command == LEAVE)
		{
			auto iter = _participants.find(join_id);
			if (iter != std::end(_participants))
			{
				_participants.erase(iter);
			}

			updateSlotAtLeave(join_id);
		}
	}

	GameMessage temp_msg;
	while (PacketFactory::getInstance().getQ().pop(temp_msg))
	{
		PacketFactory::getInstance().decodeByType(temp_msg);
	}
}

void GameRoom::send(unsigned int id, const GameMessage& msg)
{
	auto iter = _participants.find(id);
	if (iter != std::end(_participants))
	{
		iter->second->send(boost::ref(msg));
	}
}

void GameRoom::sendExceptOne(unsigned int id, const GameMessage& msg)
{
	for (auto iter = std::begin(_participants); iter != std::end(_participants); iter++)
	{
		if (iter->first == id)
			continue;
		iter->second->send(boost::ref(msg));
	}
}

void GameRoom::sendAll(const GameMessage& msg)
{
	for (auto iter = std::begin(_participants); iter != std::end(_participants); iter++)
	{
		iter->second->send(boost::ref(msg));
	}
}


std::map<unsigned int, boost::shared_ptr<GameParticipant> >& GameRoom::getParticipants()
{
	return _participants;
}

void GameRoom::incClientReadyCnt()
{
	_client_ready_cnt.fetch_add(1, boost::memory_order_relaxed);
}

void GameRoom::decClientReadyCnt()
{
	_client_ready_cnt.fetch_sub(1, boost::memory_order_release);
}

void GameRoom::clearClientReadyCnt()
{
	_client_ready_cnt.store(0, boost::memory_order_release);
}

int GameRoom::getClientReadyCnt() const
{
	return _client_ready_cnt.load(boost::memory_order_consume);
}

bool GameRoom::isReadyToStart() const
{
	return (getClientReadyCnt() >= Prm.NumMaxClients);
}

void GameRoom::setRoomState(int state)
{
	_room_state = state;
}

int GameRoom::getRoomState() const
{
	return _room_state;
}

int GameRoom::getSlotIndex(unsigned int id) const
{
	for (size_t i = 0; i < _slots.size(); i++)
	{
		if (_slots[i] == id)
			return i;
	}
	return -1;
}

unsigned int GameRoom::findSlotIndex() const
{
	for (size_t i = 0; i < _slots.size(); i++)
	{
		if (_slots[i] == SLOT_EMPTY)
			return i;
	}

	return SLOT_FULL;
}

// return true when join is successful
bool GameRoom::updateSlotAtJoin(const boost::shared_ptr<GameParticipant>& participant)
{
	unsigned int join_id = participant->getId();
	int available_idx = findSlotIndex();

	if (available_idx == SLOT_FULL)
	{
		return false;
	}
	else
	{
		_slots[available_idx] = join_id;
		participant->send(PacketFactory::getInstance().createPacketAssignID(join_id));
		return true;
	}
}

void GameRoom::updateSlotAtLeave(unsigned int leave_id)
{
	if (_room_state != TEAM_SELECT_SCENE)
		return;

	for (size_t i = 0; i < _slots.size(); i++)
	{
		if (_slots[i] == leave_id)
		{
			_slots[i] = SLOT_EMPTY;

			//send to all
			sendAll(PacketFactory::getInstance().createPacketClientLeaveTeamSelectRoom(i));
		}
	}
}


void GameRoom::setParticipantName(unsigned int id, const std::string& client_name)
{
	auto iter = _participants.find(id);

	if (iter != std::end(_participants))
	{
		// set the client name in session class
		boost::dynamic_pointer_cast<GameSession>(iter->second)->setClientName(client_name);

		int slot = getSlotIndex(id);
		send(id, PacketFactory::getInstance()
			.createPacketStartTeamSelectScene(slot, client_name));

		// send all already joined clients's name and slot to new joined client
		for (size_t i = 0; i < _slots.size(); i++)
		{
			if (_slots[i] == SLOT_EMPTY)
				continue;

			auto other = _participants.find(_slots[i]);
			if (other != std::end(_participants))
			{
				std::string name = 
					boost::dynamic_pointer_cast<GameSession>(other->second)->getClientName();

				std::string cname =
					boost::dynamic_pointer_cast<GameSession>(other->second)->getCharacterName();

				int team = boost::dynamic_pointer_cast<GameSession>(other->second)->getTeam();

				send(id, PacketFactory::getInstance().createPacketClientJoinTeamSelectRoom(i, name));
				send(id, PacketFactory::getInstance().createPacketClientSelectedCharacter(i, cname));
				send(id, PacketFactory::getInstance().createPacketClientSelectedTeam(i, team));
			}
			else
			{
				assert(false && "not existing id(in loop)");
			}
		}
		// send to all (already joined) clients that new client has joined
		sendAll(PacketFactory::getInstance().createPacketClientJoinTeamSelectRoom(slot, client_name));
	}
	else
	{
		assert(false && "not existing id");
	}
}

void GameRoom::setParticipantCharacter(unsigned int id, const std::string& cname)
{
	auto iter = _participants.find(id);

	if (iter != std::end(_participants))
	{
		// set the client name in session class
		boost::dynamic_pointer_cast<GameSession>(iter->second)->setCharacterName(cname);

		// send to all (already joined) clients that new client has joined
		sendAll(PacketFactory::getInstance()
			.createPacketClientSelectedCharacter(getSlotIndex(id), cname));
	}
	else
	{
		assert(false && "not existing id");
	}
}

void GameRoom::setParticipantTeam(unsigned int id, int team)
{
	auto iter = _participants.find(id);

	if (iter != std::end(_participants))
	{
		// set the client name in session class
		boost::dynamic_pointer_cast<GameSession>(iter->second)->setTeam(team);

		// send to all (already joined) clients that new client has joined
		sendAll(PacketFactory::getInstance()
			.createPacketClientSelectedTeam(getSlotIndex(id), team));
	}
	else
	{
		assert(false && "not existing id");
	}
}


void GameRoom::setParticipantClickReady(unsigned int id, int ready_flag)
{
	if (ready_flag == CLICKED_READY)
		incClientReadyCnt();
	else
		decClientReadyCnt();

	sendAll(PacketFactory::getInstance()
		.createPacketClientClickedReady(getSlotIndex(id), ready_flag));
}