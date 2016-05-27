#include <memory>
#include <algorithm>
#include "cocos2d.h"
#include "main.h"
#include "Network\GameServer\GameServerManager.h"
#include "Network\GameServer\GameServer.h"
#include "Network\GameServer\GameSession.h"
#include "Network\Packet\PacketFactory.h"
#include "GameWorld.h"
#include "Timer\PrecisionTimer.h"
#include "Database.h"

int main()
{
	GameServerManager::getInstance();
	DB;

	PrecisionTimer timer;
	timer.smoothUpdatesOn();
	timer.start();

	std::unique_ptr<GameWorld> game_world(new GameWorld());

	while (!GameServerManager::getInstance().getGameServer()->getGameRoom().isReadyToStart())
	{
		GameServerManager::getInstance().update();
	}

	GameServerManager::getInstance().getGameServer()->getGameRoom().setRoomState(GAME_WORLD_SCENE);
	GameServerManager::getInstance().getGameServer()->getGameRoom().clearClientReadyCnt();

	GameServerManager::getInstance().sendAll(
		PacketFactory::getInstance().createPacketStartGameScene());


	while (!GameServerManager::getInstance().getGameServer()->getGameRoom().isReadyToStart()) 
	{
		GameServerManager::getInstance().update();
	}

	CCLOG("all clients are ready!");

	GameRoom& room = GameServerManager::getInstance().getGameServer()->getGameRoom();

	// set user characters
	for (auto iter = std::begin(room.getParticipants());
	iter != std::end(room.getParticipants()); iter++)
	{
		unsigned int id = boost::dynamic_pointer_cast<GameSession>(iter->second)->getId();
		std::string cname = boost::dynamic_pointer_cast<GameSession>(iter->second)->getCharacterName();
		std::string cl_name = boost::dynamic_pointer_cast<GameSession>(iter->second)->getClientName();
		int team = boost::dynamic_pointer_cast<GameSession>(iter->second)->getTeam();

		//std::cout << id << " " << cname << " " << team << " " << cl_name << std::endl;

		game_world->createUserCharacter(id, cname, team, cl_name);
	}


	GameServerManager::getInstance().getGameServer()->getGameRoom().clearClientReadyCnt();
	
	// set world for clients
	game_world->worldDataToClient();

	while (!GameServerManager::getInstance().getGameServer()->getGameRoom().isReadyToStart())
	{
		GameServerManager::getInstance().update();
	}

	while (1)
	{
		int begin = clock();
		
		game_world->update(timer.timeElapsed());

		int end = clock();

		Sleep(std::max(40 - (end - begin), 0));
	}

	return 0;
}
