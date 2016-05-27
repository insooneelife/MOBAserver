#include "GameServerManager.h"
#include "GameServer.h"
#include "GameMessage.hpp"

#include <string>
#include <memory>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/format.hpp>

#include "../Packet/PacketFactory.h"

GameServerManager::~GameServerManager()
{}

void GameServerManager::send(unsigned int id, const GameMessage& msg)
{
	_game_server->getGameRoom().send(id, msg);
}

void GameServerManager::sendAll(const GameMessage& msg)
{
	_game_server->getGameRoom().sendAll(msg);
}

void GameServerManager::update()
{
	_game_server->getGameRoom().update();
}

const std::unique_ptr<GameServer>& GameServerManager::getGameServer() const
{
	return _game_server;
}


GameServerManager::GameServerManager()
{
	try
	{
		std::string port = "8001";

		// 서버에 대한 endpoint를 만든다. (ip와 포트 설정)
		boost::asio::ip::tcp::endpoint endpoint(
			boost::asio::ip::tcp::v4(), std::atoi(port.c_str()));

		// enpoint로 서버를 만든다.
		_game_server.reset(new GameServer(_io_service, endpoint));
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
