#pragma once

//------------------------------------------------------------------------
//
//  Name:	ChatServerManager.h
//
//  Desc:	
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include "ChatServer.h"

#include <string>
#include <memory>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/format.hpp>

#include "Singleton.h"


class ChatServerManager : public Singleton<ChatServerManager>
{
public:

	virtual ~ChatServerManager()
	{}

	void forwardWrite(std::string str)
	{}

	const std::unique_ptr<ChatServer>& getChatServer() const
	{
		return _chat_server;
	}

private:
	ChatServerManager(const ChatServerManager&) = delete; // no copies
	ChatServerManager& operator=(const ChatServerManager&) = delete; // no self-assignments
	ChatServerManager()
	{
		try
		{
			std::string port = "8001";

			// 서버에 대한 endpoint를 만든다. (ip와 포트 설정)
			boost::asio::ip::tcp::endpoint endpoint(
				boost::asio::ip::tcp::v4(), std::atoi(port.c_str()));

			// enpoint로 서버를 만든다.
			_chat_server.reset(new ChatServer(_io_service, endpoint));
			_async_io_thread.reset(
				new std::thread(
					boost::bind(&boost::asio::io_service::run, &_io_service)));

			_async_io_thread->detach();
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}
	}

	friend class Singleton<ChatServerManager>;

	std::unique_ptr<ChatServer> _chat_server;
	std::unique_ptr<std::thread> _async_io_thread;
	boost::asio::io_service _io_service;
};