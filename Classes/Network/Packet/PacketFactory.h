#pragma once

#include "../GameServer/GameMessage.hpp"
#include <boost\lockfree\queue.hpp>
#include <queue>

class GameWorld;
class WaitingRoom;

class PacketFactory
{
public:

	// server -> client
	enum
	{
		PacketAssignID, PacketCreateUserCharacter, PacketStartGameScene,
		PacketStartTeamSelectScene, PacketClientJoinTeamSelectRoom, PacketClientLeaveTeamSelectRoom,
		PacketRenderCharacterSet, PacketRenderSkillSet,
		PacketCreateCharacter, PacketCreateSkill, 
		PacketDeleteCharacter, PacketDeleteSkill,
		PacketClientSelectedCharacter, PacketClientSelectedTeam, PacketClientClickedReady,
		PacketGameChatMessageToClient, PacketRoomChatMessageToClient,
		PacketCreateAnimationOnPos, PacketWorldData, PacketGameOver
	};

	// client -> server

	enum
	{
		PacketClick,
		PacketClickedReady, PacketClientIsReady,
		PacketSelectName, PacketSelectCharacter, PacketSelectTeam, 
		PacketGameChatMessageToServer, PacketRoomChatMessageToServer
	};

	static PacketFactory& getInstance();

	void setGameWorld(GameWorld* world);

	GameWorld* getGameWorld() const;

	void setWaitingRoom(WaitingRoom* world);

	WaitingRoom* getWaitingRoom() const;

	boost::lockfree::queue<GameMessage, boost::lockfree::capacity<1000> >& getQ();



	// server : send to clients
	
	GameMessage createPacketRenderCharacterSet();

	GameMessage createPacketCreateCharacter(
		unsigned int id,
		std::string name,
		float x,
		float y,
		float mx,
		float my,
		float bradius);

	GameMessage createPacketAssignID(unsigned int id);

	GameMessage createPacketCreateUserCharacter(
		unsigned int id,
		const std::string& character_name,
		float x,
		float y,
		float mx,
		float my,
		float bradius,
		const std::string& client_name);

	GameMessage createPacketStartGameScene();

	GameMessage createPacketCreateSkill(
		unsigned int id,
		std::string name,
		float x,
		float y,
		float mx,
		float my,
		float bradius);

	GameMessage createPacketRenderSkillSet();

	GameMessage createPacketDeleteCharacter(unsigned int id);

	GameMessage createPacketDeleteSkill(unsigned int id);

	GameMessage createPacketStartTeamSelectScene(
		int slot_number, 
		const std::string& client_name);

	GameMessage createPacketClientJoinTeamSelectRoom(
		int slot_number,
		const std::string& client_name);

	GameMessage createPacketClientLeaveTeamSelectRoom(int slot_number);

	GameMessage createPacketClientSelectedCharacter(
		int slot_number,
		const std::string& cname);

	GameMessage createPacketClientSelectedTeam(
		int slot_number,
		int team);

	GameMessage createPacketClientClickedReady(
		int slot_number,
		int ready_flag);

	GameMessage createPacketGameChatMessageToClient(
		const std::string& name,
		const std::string& chat_message);

	GameMessage createPacketRoomChatMessageToClient(
		const std::string& name,
		const std::string& chat_message);

	GameMessage createPacketCreateAnimationOnPos(
		const std::string& aname,
		float x,
		float y);

	GameMessage createPacketWorldData();

	GameMessage createPacketGameOver(const std::string& name, int team);


	// server : receive from clients

	void decodeByType(GameMessage& msg);

	void decodePacketClick(uint8_t* body);

	void decodePacketClientIsReady();

	void decodePacketSelectName(uint8_t* body);

	void decodePacketSelectCharacter(uint8_t* body);

	void decodePacketSelectTeam(uint8_t* body);

	void decodePacketClickedReady(uint8_t* body);

	void decodePacketGameChatMessageToServer(uint8_t* body);

	void decodePacketRoomChatMessageToServer(uint8_t* body);
	
private:
	
	PacketFactory() : _world(nullptr), _waiting_room(nullptr)
	{}

	~PacketFactory()
	{}

	GameWorld* _world;

	WaitingRoom* _waiting_room;

	boost::lockfree::queue<GameMessage, boost::lockfree::capacity<1000> > _read_msgs;
};