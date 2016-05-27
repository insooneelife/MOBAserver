#pragma once

#include <boost\asio.hpp>
#include <boost\enable_shared_from_this.hpp>

#include "ChatMessage.hpp"
#include "ChatParticipant.h"
#include "ChatRoom.h"

// chating session
// session은 일종의 participant(참여자)이라 할 수 있다.
class ChatSession
	:
	public ChatParticipant,
	public boost::enable_shared_from_this<ChatSession>
{
public:
	ChatSession(boost::asio::io_service& io_service, ChatRoom& room);

	~ChatSession();

	boost::asio::ip::tcp::socket& socket();
	
	// 세션의 시작
	// 채팅방에 참여하고 socket으로부터 read를 수행한다.
	void start();

	// 참여자의 deliver 함수
	// 클라이언트에게 다시 write을 해주는 함수
	// 즉 다시 전송하는 부분이다.
	void deliver(const ChatMessage& msg);

	// 비동기 read 작업이 끝날 경우 호출되는 함수
	void handleReadHeader(const boost::system::error_code& error);

	// 실제적인 데이터를 받아들이는 함수이다.
	void handleReadBody(const boost::system::error_code& error);

	void handleWrite(const boost::system::error_code& error);

private:
	// client와 통신할 socket
	boost::asio::ip::tcp::socket _socket;

	// 이 세션(참여자)가 참여중인 채팅방
	ChatRoom& _room;

	// 이 참여자가 읽은 메시지
	ChatMessage _read_msg;


	std::deque<ChatMessage> _write_msgs;
};