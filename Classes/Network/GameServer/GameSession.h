#pragma once

#include <boost\asio.hpp>
#include <boost\enable_shared_from_this.hpp>

#include "GameMessage.hpp"
#include "GameParticipant.h"
#include "GameRoom.h"

#include <boost\lockfree\queue.hpp>

// session은 서버 단에서 클라이언트와 통신하는 대리자이다.
// 즉 client와 1:1 대응된다.
class GameSession
	:
	public GameParticipant,
	public boost::enable_shared_from_this<GameSession>
{
public:
	GameSession(
		boost::asio::io_service& io_service,
		GameRoom& room,
		unsigned int id);

	virtual ~GameSession();

	boost::asio::ip::tcp::socket& socket();
	

	std::string		getClientName() const;
	std::string		getCharacterName() const;
	int				getTeam() const;
	
	void			setClientName(const std::string& name);
	void			setCharacterName(const std::string& name);
	void			setTeam(int team);


	// 세션의 시작
	// 채팅방에 참여하고 socket으로부터 read를 수행한다.
	void start();

	// 모든 client에게 비동기 write
	virtual void send(const GameMessage& msg) override;

	// 비동기 read 작업이 끝날 경우 호출되는 함수
	void handleReadHeader(const boost::system::error_code& error);

	// 실제적인 데이터를 받아들이는 함수이다.
	void handleReadBody(const boost::system::error_code& error);

	void handleWrite(const boost::system::error_code& error);

private:

	// client와 통신할 socket
	boost::asio::ip::tcp::socket _socket;

	// 이 세션(참여자)가 참여중인 채팅방
	GameRoom& _room;

	// 이 참여자가 읽은 메시지
	GameMessage _read_msg;

	// 비동기 작업과 일반 작업이 critical section을 만들어 프로그램이 죽을 수 있다.
	// 그러므로 lockfree queue를 이용한다.
	boost::lockfree::queue<GameMessage, boost::lockfree::capacity<1000> > _write_msgs;
	

	// 참여자 정보
	std::string		_client_name;
	std::string		_character_name;
	int				_team;
};