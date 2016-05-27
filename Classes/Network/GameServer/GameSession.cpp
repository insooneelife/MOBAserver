#include <cassert>
#include <boost/bind.hpp>
#include "GameSession.h"
#include "../Packet/PacketFactory.h"

// ### shared_ptr의 use_count가 엄청나게 증가하는 문제
// 비동기로 callback이 등록되는 속도가 처리되는 속도보다 빠르기 때문이라고 예상

GameSession::GameSession(
	boost::asio::io_service& io_service,
	GameRoom& room,
	unsigned int id)
	:
	GameParticipant(id),
	// 소켓을 만든다. 이후에 이 socket은 accepet를 통해 서버와 클라이언트의 통신을 위한 장치가 된다.
	_socket(io_service),
	_room(room),
	_client_name("default"),
	_character_name("Andariel"),
	_team(1)
{
	std::cout << "GameSession : constructed" << std::endl;
}


GameSession::~GameSession()
{
	std::cout << "~GameSession()" << std::endl;
	
	_socket.close();
}

boost::asio::ip::tcp::socket& GameSession::socket()
{
	return _socket;
}

// 세션의 시작
// 채팅방에 참여하고 socket으로부터 read를 수행한다.
void GameSession::start()
{
	std::cout << "GameSession : start" << std::endl;

	// 채팅방에 세션에 대한 this포인터를 넘겨준다.
	// 포인터를 스마트포인터로 받기 때문에, shared_from_this를 이용한다.
	// shared_from_this는 스마트포인터의 this 버전이다.
	_room.join(shared_from_this());

	// 비동기로 header를 read를 한다.
	// 읽은 데이터는 _read_msg.data()에 저장한다.
	// read가 끝날 경우 handleReadHeader함수를 호출한다.
	boost::asio::async_read(
		_socket,
		boost::asio::buffer(
			_read_msg.getData(),
			GameMessage::HEADER_LENGTH),
		boost::bind(
			&GameSession::handleReadHeader,
			shared_from_this(),
			boost::asio::placeholders::error));
}

void GameSession::send(const GameMessage& msg)
{
	//std::cout << "GameSession : update" << std::endl;

	boost::asio::async_write(
		_socket,
		boost::asio::buffer(
			msg.getData(),
			msg.getLength()),
		boost::bind(
			&GameSession::handleWrite,
			shared_from_this(),
			boost::asio::placeholders::error));

	/*bool write_in_progress = !_write_msgs.empty();

	_write_msgs.push(msg);
	
	// false일 때 실행
	if (!write_in_progress)
	{
		GameMessage temp_msg;
		
		while (_write_msgs.pop(temp_msg))
		{
			boost::asio::async_write(
				_socket,
				boost::asio::buffer(
					temp_msg.getData(),
					temp_msg.getLength()),
				boost::bind(
					&GameSession::handleWrite,
					shared_from_this(),
					boost::asio::placeholders::error));
		}
	}*/
}

// 비동기 read 작업이 끝날 경우 호출되는 함수
void GameSession::handleReadHeader(const boost::system::error_code& error)
{
	// error가 없고, decodeHeader가 정상적일 경우
	if (!error && _read_msg.decodeHeader())
	{
		// body를 읽는다.
		// 마찬가지로 비동기로 읽는다.
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.getBody(),
				_read_msg.getBodyLength()),
			boost::bind(
				&GameSession::handleReadBody,
				shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		_room.leave(shared_from_this());
	}
}

// 실제적인 데이터를 받아들이는 함수이다.
void GameSession::handleReadBody(const boost::system::error_code& error)
{
	//std::cout << "GameSession : handleReadBody" << std::endl;
	if (!error)
	{
		// type에 맞는 처리
		//PacketFactory::getInstance().decodeByType(_read_msg);
		PacketFactory::getInstance().getQ().push(_read_msg);

		// 다시 header부터 읽는다.
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.getData(),
				GameMessage::HEADER_LENGTH),
			boost::bind(
				&GameSession::handleReadHeader,
				shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		_room.leave(shared_from_this());
	}
}

// 비동기 write가 끝나지 않은 상태에서 buffer를 건드리면 문제가 생기기 때문에
// queue를 이용한다.
void GameSession::handleWrite(const boost::system::error_code& error)
{
	//std::cout << "GameSession : handleWrite" << std::endl;
	if (!error)
	{
		/*if (!_write_msgs.empty())
		{
			GameMessage temp_msg;
			if (_write_msgs.pop(temp_msg))
			{
				boost::asio::async_write(
					_socket,
					boost::asio::buffer(
						temp_msg.getData(),
						temp_msg.getLength()),
					boost::bind(
						&GameSession::handleWrite,
						shared_from_this(),
						boost::asio::placeholders::error));
			}
			else
			{
				std::cout << "failed to pop" << std::endl;
			}
		}*/
	}
	else
	{
		_room.leave(shared_from_this());
	}
}


std::string GameSession::getClientName() const
{
	return _client_name;
}

std::string GameSession::getCharacterName() const
{
	return _character_name;
}

int GameSession::getTeam() const
{
	return _team;
}

void GameSession::setClientName(const std::string& name)
{
	_client_name = name;
}

void GameSession::setCharacterName(const std::string& name)
{
	_character_name = name;
}

void GameSession::setTeam(int team)
{
	_team = team;
}