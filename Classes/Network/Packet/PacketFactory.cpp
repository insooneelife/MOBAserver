#include <queue>
#include <tuple>
#include <vector>

#include "cocos2d.h"

#include "PacketClick_generated.h"
#include "PacketRenderCharacter_generated.h"
#include "PacketRenderCharacterSet_generated.h"
#include "PacketCreateCharacter_generated.h"
#include "PacketAssignID_generated.h"
#include "PacketCreateSkill_generated.h"
#include "PacketRenderSkill_generated.h"
#include "PacketRenderSkillSet_generated.h"
#include "PacketDeleteCharacter_generated.h"
#include "PacketDeleteSkill_generated.h"
#include "PacketSelectName_generated.h"
#include "PacketStartTeamSelectScene_generated.h"
#include "PacketCreateUserCharacter_generated.h"
#include "PacketSelectCharacter_generated.h"
#include "PacketSelectTeam_generated.h"
#include "PacketClientSelectedCharacter_generated.h"
#include "PacketClientSelectedTeam_generated.h"
#include "PacketClickedReady_generated.h"
#include "PacketClientClickedReady_generated.h"
#include "PacketCreateAnimationOnPos_generated.h"

#include "PacketChatMessageToServer_generated.h"
#include "PacketChatMessageToClient_generated.h"
#include "PacketWorldData_generated.h"
#include "PacketGameOver_generated.h"

#include "PacketFactory.h"

#include "../GameServer/GameMessage.hpp"
#include "../GameServer/GameServerManager.h"
#include "../GameServer/GameServer.h"
#include "../GameServer/GameRoom.h"
#include "../GameServer/GameSession.h"

#include "../../GameWorld.h"
#include "../../AbstCharacter.h"
#include "../../UserCharacter.h"
#include "../../Skills/AbstSkill.h"
#include "../../Motion.h"


PacketFactory& PacketFactory::getInstance()
{
	static PacketFactory instance;
	return instance;
}


//------------------------------ server -> client -----------------------------------//

GameMessage PacketFactory::createPacketRenderCharacterSet()
{
	assert(_world && "world is not set");

	flatbuffers::FlatBufferBuilder builder;

	std::vector<flatbuffers::Offset<packets::PacketRenderCharacter> > characters;

	for (auto iter = std::begin(_world->getCharacters()); 
		iter != std::end(_world->getCharacters()); iter++)
	{
		auto encode_motion = builder.CreateString((*iter)->getMotionName());

		characters.emplace_back(
			packets::CreatePacketRenderCharacter(
				builder,
				(*iter)->getId(),
				(*iter)->getPos().x,
				(*iter)->getPos().y,
				(*iter)->getMotion()->getFrame(),
				encode_motion,
				(*iter)->getHealth(),
				(*iter)->getMana(),
				(*iter)->getDamage(),
				(*iter)->getMDamage(),
				(*iter)->getAttackSpeed(),
				(*iter)->getMaxSpeed()));
	}
		
	//auto container = builder.CreateVectorOfSortedTables(&characters);
	auto container = builder.CreateVector(characters);

	auto character_set = packets::CreatePacketRenderCharacterSet(
		builder,
		container);

	builder.Finish(character_set);
	return GameMessage(builder, PacketRenderCharacterSet);
}

GameMessage PacketFactory::createPacketCreateCharacter(
	unsigned int id,
	std::string name,
	float x,
	float y,
	float mx,
	float my,
	float bradius)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_name = builder.CreateString(name);

	auto render_data =
		packets::CreatePacketCreateCharacter(
			builder, id, encode_name, x, y, mx, my, bradius);

	builder.Finish(render_data);

	return GameMessage(builder, PacketCreateCharacter);
}


GameMessage PacketFactory::createPacketAssignID(unsigned int id)
{
	flatbuffers::FlatBufferBuilder builder;

	auto ussign_id_packet = packets::CreatePacketAssignID(builder, id);

	builder.Finish(ussign_id_packet);

	return GameMessage(builder, PacketAssignID);
}


GameMessage PacketFactory::createPacketCreateUserCharacter(
	unsigned int id,
	const std::string& character_name,
	float x,
	float y,
	float mx,
	float my,
	float bradius,
	const std::string& client_name)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_cname = builder.CreateString(character_name);
	auto encode_cl_name = builder.CreateString(client_name);

	auto render_data =
		packets::CreatePacketCreateUserCharacter(
			builder, id, encode_cname, x, y, mx, my, bradius, encode_cl_name);

	builder.Finish(render_data);

	return GameMessage(builder, PacketCreateUserCharacter);

}


GameMessage PacketFactory::createPacketStartGameScene()
{
	return GameMessage(PacketStartGameScene);
}

GameMessage PacketFactory::createPacketCreateSkill(
	unsigned int id,
	std::string name,
	float x,
	float y,
	float mx,
	float my,
	float bradius)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_name = builder.CreateString(name);

	auto render_data =
		packets::CreatePacketCreateSkill(
			builder, id, encode_name, x, y, mx, my, bradius);

	builder.Finish(render_data);

	return GameMessage(builder, PacketCreateSkill);
}

GameMessage PacketFactory::createPacketRenderSkillSet()
{
	assert(_world && "world is not set");

	flatbuffers::FlatBufferBuilder builder;

	std::vector<flatbuffers::Offset<packets::PacketRenderSkill> > skills;

	for (auto iter = std::begin(_world->getSkills());
		iter != std::end(_world->getSkills()); iter++)
	{
		skills.emplace_back(
			packets::CreatePacketRenderSkill(
				builder,
				(*iter)->getId(),
				(*iter)->getPos().x,
				(*iter)->getPos().y,
				(*iter)->getActivated()));
	}

	//auto container = builder.CreateVectorOfSortedTables(&characters);
	auto container = builder.CreateVector(skills);

	auto skill_set = packets::CreatePacketRenderSkillSet(builder, container);

	builder.Finish(skill_set);
	return GameMessage(builder, PacketRenderSkillSet);
}

GameMessage PacketFactory::createPacketDeleteCharacter(unsigned int id)
{
	flatbuffers::FlatBufferBuilder builder;

	auto delete_data = packets::CreatePacketDeleteCharacter(builder, id);

	builder.Finish(delete_data);

	return GameMessage(builder, PacketDeleteCharacter);
}

GameMessage PacketFactory::createPacketDeleteSkill(unsigned int id)
{
	flatbuffers::FlatBufferBuilder builder;

	auto delete_data = packets::CreatePacketDeleteSkill(builder, id);

	builder.Finish(delete_data);

	return GameMessage(builder, PacketDeleteSkill);
}

GameMessage PacketFactory::createPacketStartTeamSelectScene(
	int slot_number,
	const std::string& client_name)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_name = builder.CreateString(client_name);

	auto start_data = packets::CreatePacketStartTeamSelectScene(builder, slot_number, encode_name);

	builder.Finish(start_data);

	return GameMessage(builder, PacketStartTeamSelectScene);
}

GameMessage PacketFactory::createPacketClientJoinTeamSelectRoom(
	int slot_number,
	const std::string& client_name)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_name = builder.CreateString(client_name);

	// scenario is different but the packet type is same with PacketStartTeamSelectScene.
	auto start_data = packets::CreatePacketStartTeamSelectScene(builder, slot_number, encode_name);

	builder.Finish(start_data);

	return GameMessage(builder, PacketClientJoinTeamSelectRoom);
}

GameMessage PacketFactory::createPacketClientLeaveTeamSelectRoom(int slot_number)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_name = builder.CreateString("");

	// scenario is different but the packet type is same with PacketStartTeamSelectScene.
	auto start_data = packets::CreatePacketStartTeamSelectScene(builder, slot_number, encode_name);

	builder.Finish(start_data);

	return GameMessage(builder, PacketClientLeaveTeamSelectRoom);
}

GameMessage PacketFactory::createPacketClientSelectedCharacter(int slot, const std::string& cname)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_cname = builder.CreateString(cname);

	// scenario is different but the packet type is same with PacketSelectCharacter.
	auto selection_data = packets::CreatePacketClientSelectedCharacter(builder, slot, encode_cname);

	builder.Finish(selection_data);

	return GameMessage(builder, PacketClientSelectedCharacter);
}

GameMessage PacketFactory::createPacketClientSelectedTeam(int slot, int team)
{
	flatbuffers::FlatBufferBuilder builder;

	// scenario is different but the packet type is same with PacketSelectTeam.
	auto selection_data = packets::CreatePacketClientSelectedTeam(builder, slot, team);

	builder.Finish(selection_data);

	return GameMessage(builder, PacketClientSelectedTeam);
}

GameMessage PacketFactory::createPacketClientClickedReady(int slot, int ready_flag)
{
	flatbuffers::FlatBufferBuilder builder;

	// scenario is different but the packet type is same with PacketSelectTeam.
	auto ready_data = packets::CreatePacketClientClickedReady(builder, slot, ready_flag);

	builder.Finish(ready_data);

	return GameMessage(builder, PacketClientClickedReady);

}

GameMessage PacketFactory::createPacketGameChatMessageToClient(
	const std::string& name,
	const std::string& chat_message)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_name = builder.CreateString(name);
	auto encode_message = builder.CreateString(chat_message);

	auto chat_data = packets::CreatePacketChatMessageToClient(builder, encode_name, encode_message);

	builder.Finish(chat_data);

	return GameMessage(builder, PacketGameChatMessageToClient);
}

GameMessage PacketFactory::createPacketRoomChatMessageToClient(
	const std::string& name,
	const std::string& chat_message)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_name = builder.CreateString(name);
	auto encode_message = builder.CreateString(chat_message);

	auto chat_data = packets::CreatePacketChatMessageToClient(builder, encode_name, encode_message);

	builder.Finish(chat_data);

	return GameMessage(builder, PacketRoomChatMessageToClient);
}

GameMessage PacketFactory::createPacketCreateAnimationOnPos(
	const std::string& aname,
	float x,
	float y)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_aname = builder.CreateString(aname);

	auto chat_data = packets::CreatePacketCreateAnimationOnPos(builder, encode_aname, x, y);

	builder.Finish(chat_data);

	return GameMessage(builder, PacketCreateAnimationOnPos);
}

GameMessage PacketFactory::createPacketWorldData()
{
	assert(_world && "world is not set");

	flatbuffers::FlatBufferBuilder builder;

	// characters
	std::vector<flatbuffers::Offset<packets::PacketCreateCharacter> > characters;

	for (auto iter = std::begin(_world->getCharacters());
		iter != std::end(_world->getCharacters()); iter++)
	{
		auto encode_name = builder.CreateString((*iter)->getName());

		characters.emplace_back(
			packets::CreatePacketCreateCharacter(
				builder, 
				(*iter)->getId(),
				encode_name,
				(*iter)->getPos().x,
				(*iter)->getPos().y,
				-1,
				-1,
				(*iter)->getBRadius()));
	}

	auto container1 = builder.CreateVector(characters);


	// user characters
	std::vector<flatbuffers::Offset<packets::PacketCreateUserCharacter> > user_characters;

	for (auto iter = std::begin(_world->getUserCharacters());
		iter != std::end(_world->getUserCharacters()); iter++)
	{
		auto encode_name = builder.CreateString(iter->second->getName());
		auto encode_cl_name = builder.CreateString(iter->second->getClientName());

		user_characters.emplace_back(
			packets::CreatePacketCreateUserCharacter(
				builder,
				iter->second->getId(),
				encode_name,
				iter->second->getPos().x,
				iter->second->getPos().y,
				-1,
				-1,
				iter->second->getBRadius(),
				encode_cl_name));
	}

	auto container2 = builder.CreateVector(user_characters);

	auto world_data = packets::CreatePacketWorldData(builder, container1, container2);

	builder.Finish(world_data);
	return GameMessage(builder, PacketWorldData);
}

GameMessage PacketFactory::createPacketGameOver(const std::string& name, int team)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_name = builder.CreateString(name);

	auto game_over = packets::CreatePacketGameOver(builder, encode_name, team);

	builder.Finish(game_over);

	return GameMessage(builder, PacketGameOver);
}

//------------------------------ client -> server -----------------------------------//

void PacketFactory::decodeByType(GameMessage& msg)
{
	if (msg.getType() == PacketFactory::PacketClick)
	{
		assert(packets::VerifyPacketClickBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketClick");

		PacketFactory::decodePacketClick(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketClientIsReady)
	{
		PacketFactory::decodePacketClientIsReady();
	}
	else if (msg.getType() == PacketFactory::PacketSelectName)
	{
		assert(packets::VerifyPacketSelectNameBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketSelectName");

		PacketFactory::decodePacketSelectName(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketSelectCharacter)
	{
		assert(packets::VerifyPacketSelectCharacterBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketSelectCharacter");

		PacketFactory::decodePacketSelectCharacter(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketSelectTeam)
	{
		assert(packets::VerifyPacketSelectTeamBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketSelectTeam");

		PacketFactory::decodePacketSelectTeam(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketClickedReady)
	{
		assert(packets::VerifyPacketClickedReadyBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketClickedReady");

		PacketFactory::decodePacketClickedReady(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketGameChatMessageToServer)
	{
		assert(packets::VerifyPacketChatMessageToServerBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketGameChatMessageToServer == GetPacketChatMessageToServer");

		PacketFactory::decodePacketGameChatMessageToServer(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketRoomChatMessageToServer)
	{
		assert(packets::VerifyPacketChatMessageToServerBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketRoomChatMessageToServer == GetPacketChatMessageToServer");

		PacketFactory::decodePacketRoomChatMessageToServer(msg.getBody());
	}
	else
	{
		assert(false && "no packet type");
	}
}

void PacketFactory::decodePacketClick(uint8_t* body)
{
	auto decoded = packets::GetPacketClick(body);

	std::cout << "decodePacketClick" << std::endl;
	assert(_world && "no world");

	if(decoded->button() == packets::MouseButton_LEFT)
		_world->clickLeftMouseButton(
			decoded->id(), cocos2d::Vec2(decoded->x(), decoded->y()),
			decoded->command(), decoded->clicked_id());
	
	else if(decoded->button() == packets::MouseButton_RIGHT)
		_world->clickRightMouseButton(
			decoded->id(), cocos2d::Vec2(decoded->x(), decoded->y()),
			decoded->command(), decoded->clicked_id());
	
	else
		assert(false && "no mouse button type");

}

void PacketFactory::decodePacketClientIsReady()
{
	std::cout << "decodePacketClientIsReady" << std::endl;
	GameServerManager::getInstance().getGameServer()->getGameRoom().incClientReadyCnt();
}

// the scenario of decoding packet from client after the selection of client name
void PacketFactory::decodePacketSelectName(uint8_t* body)
{
	std::cout << "decodePacketSelectName" << std::endl;
	auto decoded = packets::GetPacketSelectName(body);

	unsigned int id = decoded->id();
	std::string client_name = decoded->client_name()->c_str();

	GameServerManager::getInstance().getGameServer()->getGameRoom().setParticipantName(id, client_name);
}

void PacketFactory::decodePacketSelectCharacter(uint8_t* body)
{
	std::cout << "decodePacketSelectCharacter" << std::endl;
	auto decoded = packets::GetPacketSelectCharacter(body);

	unsigned int id = decoded->id();
	std::string cname = decoded->cname()->c_str();

	GameServerManager::getInstance().getGameServer()->getGameRoom().setParticipantCharacter(id, cname);
}

void PacketFactory::decodePacketSelectTeam(uint8_t* body)
{
	std::cout << "decodePacketSelectTeam" << std::endl;
	auto decoded = packets::GetPacketSelectTeam(body);

	unsigned int id = decoded->id();
	int team = decoded->team();

	GameServerManager::getInstance().getGameServer()->getGameRoom().setParticipantTeam(id, team);
}

void PacketFactory::decodePacketClickedReady(uint8_t* body)
{
	std::cout << "decodePacketClickedReady" << std::endl;
	auto decoded = packets::GetPacketClickedReady(body);

	unsigned int id = decoded->id();
	int ready_flag = decoded->ready_flag();

	GameServerManager::getInstance().getGameServer()->getGameRoom().setParticipantClickReady(id, ready_flag);
}

void PacketFactory::decodePacketGameChatMessageToServer(uint8_t* body)
{
	std::cout << "decodePacketGameChatMessageToServer" << std::endl;
	auto decoded = packets::GetPacketChatMessageToServer(body);

	unsigned int id = decoded->id();
	std::string chat_message = decoded->chat_message()->c_str();


	auto client = 
		GameServerManager::getInstance().getGameServer()->getGameRoom().getParticipants().find(id);

	if (client != std::end(GameServerManager::getInstance().getGameServer()->getGameRoom().getParticipants()))
	{
		std::string name = boost::dynamic_pointer_cast<GameSession>(client->second)->getClientName();

		GameServerManager::getInstance().getGameServer()->getGameRoom()
			.sendAll(PacketFactory::getInstance()
				.createPacketGameChatMessageToClient(name, chat_message));
	}
}

void PacketFactory::decodePacketRoomChatMessageToServer(uint8_t* body)
{
	std::cout << "decodePacketRoomChatMessageToServer" << std::endl;
	auto decoded = packets::GetPacketChatMessageToServer(body);

	unsigned int id = decoded->id();
	std::string chat_message = decoded->chat_message()->c_str();

	auto client =
		GameServerManager::getInstance().getGameServer()->getGameRoom().getParticipants().find(id);

	if (client != std::end(GameServerManager::getInstance().getGameServer()->getGameRoom().getParticipants()))
	{
		std::string name = boost::dynamic_pointer_cast<GameSession>(client->second)->getClientName();
		GameServerManager::getInstance().getGameServer()->getGameRoom()
			.sendAll(PacketFactory::getInstance()
				.createPacketRoomChatMessageToClient(name, chat_message));
	}
}


void PacketFactory::setGameWorld(GameWorld* world)
{
	_world = world;
}

GameWorld* PacketFactory::getGameWorld() const
{
	return _world;
}

void PacketFactory::setWaitingRoom(WaitingRoom* room)
{
	_waiting_room = room;
}

WaitingRoom* PacketFactory::getWaitingRoom() const
{
	return _waiting_room;
}

boost::lockfree::queue<GameMessage, boost::lockfree::capacity<1000> >&
PacketFactory::getQ()
{
	return _read_msgs;
}