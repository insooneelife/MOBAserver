#pragma once

#include <boost\shared_ptr.hpp>
#include <boost\atomic.hpp>

#include <map>
#include <set>
#include <iostream>
#include <queue>
#include <array>

class GameParticipant;
class GameMessage;

class GameRoom
{
public:
	enum { SLOT_SIZE = 6, SLOT_EMPTY = 0, SLOT_FULL = -1};

	GameRoom();

	std::map<unsigned int, boost::shared_ptr<GameParticipant> >& getParticipants();

	void incClientReadyCnt();

	void decClientReadyCnt();

	void clearClientReadyCnt();

	int getClientReadyCnt() const;

	bool isReadyToStart() const;

	void setRoomState(int state);

	int getRoomState() const;

	int getSlotIndex(unsigned int id) const;

	void setParticipantName(unsigned int id, const std::string& client_name);

	void setParticipantCharacter(unsigned int id, const std::string& cname);

	void setParticipantTeam(unsigned int id, int team);

	void setParticipantClickReady(unsigned int id, int ready_flag);


	// 세션(참여자)를 채팅방에 참여시키기 위해 command_que에 참여 명령을 넣는다.
	void join(boost::shared_ptr<GameParticipant> participant);

	// 세션(참여자)를 채팅방에서 제거하기 위해 command_que에 제거 명령을 넣는다.
	void leave(boost::shared_ptr<GameParticipant> participant);

	void update();


	void send(unsigned int id, const GameMessage& msg);

	void sendAll(const GameMessage& msg);

	void sendExceptOne(unsigned int id, const GameMessage& msg);

private:

	unsigned int findSlotIndex() const;

	bool updateSlotAtJoin(const boost::shared_ptr<GameParticipant>& participant);

	void updateSlotAtLeave(unsigned int leave_id);


	// 참여자 리스트
	std::map<unsigned int, boost::shared_ptr<GameParticipant> > _participants;
	
	enum { JOIN, LEAVE };

	// 순회 도중 erase나 insert가 일어나면 안되기 때문에 command_que를 두고,
	// 따로 loop을 돌려서 순회 시작 전에 처리되도록 한다.
	// lock free queue로 해야 할 것 같지만, 컴파일 에러를 잡지 못함
	// 또한 shared_ptr로 되어있기 때문에 중간에 raw pointer를 사용하면 메모리가 해제된다.
	std::queue<std::pair<int, boost::shared_ptr<GameParticipant> > > _command_que;

	boost::atomic<int> _client_ready_cnt;

	int _room_state;

	std::array<unsigned int, SLOT_SIZE> _slots;
};