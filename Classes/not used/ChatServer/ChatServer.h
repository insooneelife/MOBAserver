#pragma once
#include <iostream>
#include <boost\asio.hpp>
#include <boost\bind.hpp>

#include "ChatRoom.h"

class ChatSession;

class ChatServer
{
public:
	ChatServer(
		// 어떤 방법으로 통신할 것인지
		boost::asio::io_service& io_service,
		// 서버로 사용할 endpoint
		const boost::asio::ip::tcp::endpoint& endpoint);

	~ChatServer();

	void startAccept();

	// 비동기 accept가 끝날 때의 대한 작업
	void handleAccept(
		boost::shared_ptr<ChatSession> session,
		const boost::system::error_code& error);

private:
	boost::asio::io_service& _io_service;
	boost::asio::ip::tcp::acceptor _acceptor;
	ChatRoom _room;
};