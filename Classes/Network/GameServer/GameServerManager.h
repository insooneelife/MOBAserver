#pragma once

//------------------------------------------------------------------------
//
//  Name:	GameServerManager.h
//
//  Desc:	
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <memory>
#include <thread>
#include <boost/asio/io_service.hpp>

#include "GameMessage.hpp"

class GameServer;

class GameServerManager
{
public:
	static GameServerManager& getInstance()
	{
		static GameServerManager instance;
		return instance;
	}

	virtual ~GameServerManager();

	void send(unsigned int id, const GameMessage& msg);

	void sendAll(const GameMessage& msg);

	void update();

	const std::unique_ptr<GameServer>& getGameServer() const;

private:
	GameServerManager(const GameServerManager&) = delete; // no copies
	GameServerManager& operator=(const GameServerManager&) = delete; // no self-assignments
	GameServerManager();

	std::unique_ptr<GameServer> _game_server;
	std::unique_ptr<std::thread> _async_io_thread;
	boost::asio::io_service _io_service;
};