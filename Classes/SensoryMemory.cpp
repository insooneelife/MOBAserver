#include "SensoryMemory.h"
#include "GameWorld.h"
#include "AbstCharacter.h"
#include "Timer/CrudeTimer.h"
#include "SteeringBehavior.h"

USING_NS_CC;

std::stringstream & operator<<(std::stringstream &ss, const MemoryRecord& m)
{
	ss << m.attackable << std::endl
		<< m.within_view << std::endl
		<< m.time_became_visible << std::endl
		<< m.time_last_sensed << std::endl
		<< m.time_last_visible << std::endl
		<< m.recent_damage << std::endl
		<< m.last_sensed_position.x << ", "
		<< m.last_sensed_position.y << std::endl;
	return ss;
}

//------------------------------- ctor ----------------------------------------
//-----------------------------------------------------------------------------
SensoryMemory::SensoryMemory(
	AbstCharacter* const owner,
	double memory_span)
	:
	_owner(owner),
	_memory_span(memory_span)
{}

SensoryMemory::~SensoryMemory()
{}

//--------------------- makeNewRecordIfNotAlreadyPresent ----------------------

void SensoryMemory::makeNewRecordIfNotAlreadyPresent(AbstCharacter* const opponent)
{
	//else check to see if this Opponent already exists in the memory. If it doesn't,
	//create a new record
	if (_memory_map.find(opponent) == _memory_map.end())
	{
		_memory_map[opponent] = MemoryRecord();
	}
}

//------------------------ removeBotFromMemory --------------------------------
//
//  this removes a bot's record from memory
//-----------------------------------------------------------------------------
void SensoryMemory::removeBotFromMemory(AbstCharacter* const bot)
{
	MemoryMap::iterator record = _memory_map.find(bot);

	if (record != _memory_map.end())
	{
		_memory_map.erase(record);
	}
}

//----------------------- updateWithSoundSource -------------------------------
//
// this updates the record for an individual opponent. Note, there is no need to
// test if the opponent is within the FOV because that test will be done when the
// updateVision method is called
//-----------------------------------------------------------------------------
void SensoryMemory::updateWithSoundSource(AbstCharacter* const noise_maker)
{
	//make sure the bot being examined is not this bot
	if (_owner != noise_maker)
	{
		//if the bot is already part of the memory then update its data, else
		//create a new memory record and add it to the memory
		makeNewRecordIfNotAlreadyPresent(noise_maker);

		MemoryRecord& info = _memory_map[noise_maker];

		//test if there is LOS between bots 
		if (_owner->getWorld()->isLOSOkay(_owner->getPos(), noise_maker->getPos()))
		{
			info.attackable = true;

			//record the position of the bot
			info.last_sensed_position = noise_maker->getPos();
		}
		else
		{
			info.attackable = false;
		}

		//record the time it was sensed
		info.time_last_sensed = (double)Clock.getCurrentTime();
	}
}

//----------------------- updateWithDamageSource -------------------------------
//
// this updates the record for an individual opponent. Note, there is no need to
// test if the opponent is within the FOV because that test will be done when the
// updateVision method is called
// 때린사람 위치 기억.
//-----------------------------------------------------------------------------
void SensoryMemory::updateWithDamageSource(AbstCharacter* const shooter, int damage)
{
	if (shooter == nullptr)
		return;

	//make sure the bot being examined is not this bot
	if (_owner != shooter) // probably not possible
	{
		//if the bot is already part of the memory then update its data, else
		//create a new memory record and add it to the memory
		makeNewRecordIfNotAlreadyPresent(shooter);

		MemoryRecord& info = _memory_map[shooter];

		info.recent_damage += damage;

		//test if there is LOS between bots 
		if (_owner->getWorld()->isLOSOkay(_owner->getPos(), shooter->getPos()))
		{
			//record the position of the bot
			info.last_sensed_position = shooter->getPos();
		}
		else
		{
			info.attackable = false;
		}

		//record the time it was sensed
		info.time_last_sensed = (double)Clock.getCurrentTime();
	}
}

//----------------------------- updateVision ----------------------------------
//
//  this method iterates through all the bots in the game world to test if
//  they are in the field of view. Each bot's memory record is updated
//  accordingly
//-----------------------------------------------------------------------------
void SensoryMemory::updateVision()
{
	_owner->getWorld()->getGameMap()->getAgentCellSpace()->
		calculateNeighborsForSmall(
			_owner->getPos(), 
			&geometry::Circle(
				_owner->getPos(),
				_owner->getSteering()->getViewDistance()));

	for (auto iter = _owner->getWorld()->getGameMap()->getAgentCellSpace()->begin();
		!_owner->getWorld()->getGameMap()->getAgentCellSpace()->end();
		iter = _owner->getWorld()->getGameMap()->getAgentCellSpace()->next())
	{
		if (_owner != iter)
		{
			//make sure it is part of the memory map
			makeNewRecordIfNotAlreadyPresent(iter);

			//get a reference to this bot's data
			MemoryRecord& info = _memory_map[iter];

			if (util::inRange(
				_owner->getPos(),
				iter->getPos(),
				_owner->getSteering()->getViewDistance()))
			{
				info.time_last_sensed = Clock.getCurrentTime();
				info.last_sensed_position = iter->getPos();
				info.time_last_visible = Clock.getCurrentTime();

				if (util::inRange(
					iter->getPos(),
					_owner->getPos(),
					_owner->getBRadius() + iter->getBRadius() + _owner->getAttackRange()))
				{
					info.attackable = true;
				}
				else
					info.attackable = false;

				if (info.within_view == false)
				{
					info.within_view = true;
					info.time_became_visible = info.time_last_sensed;
				}
			}
			else
			{
				info.attackable = false;
				info.within_view = false;
			}
		}
	}
}


void SensoryMemory::render()
{}

//------------------------ getListOfRecentlySensedOpponents -------------------
//
//  returns a list of the bots that have been sensed recently
//-----------------------------------------------------------------------------
std::list<AbstCharacter*>
SensoryMemory::getListOfRecentlySensedOpponents()const
{
	//this will store all the opponents the bot can remember
	std::list<AbstCharacter*> opponents;

	double current_time = Clock.getCurrentTime();

	MemoryMap::const_iterator rec = _memory_map.begin();
	for (rec; rec != _memory_map.end(); ++rec)
	{
		//if this bot has been updated in the memory recently, add to list
		if ((current_time - rec->second.time_last_sensed) <= _memory_span)
		{
			opponents.push_back(rec->first);
		}
	}

	return opponents;
}

//----------------------------- isOpponentAttackable --------------------------------
//
//  returns true if the bot given as a parameter can be shot (ie. its not
//  obscured by walls)
//-----------------------------------------------------------------------------
bool SensoryMemory::isOpponentAttackable(AbstCharacter* const opponent)const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end())
	{
		return it->second.attackable; 
	}

	return false;
}

//----------------------------- isOpponentWithinFOV --------------------------------
//
//  returns true if the bot given as a parameter is within FOV
//-----------------------------------------------------------------------------
bool SensoryMemory::isOpponentWithinFOV(AbstCharacter* const opponent) const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end())
	{
		return it->second.within_view;
	}

	return false;
}

//---------------------------- getLastRecordedPositionOfOpponent -------------------
//
//  returns the last recorded position of the bot
//-----------------------------------------------------------------------------
Vec2 SensoryMemory::getLastRecordedPositionOfOpponent(AbstCharacter* const opponent) const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end())
	{
		return it->second.last_sensed_position;
	}

	throw std::runtime_error("< SensoryMemory::getLastRecordedPositionOfOpponent>: Attempting to get position of unrecorded bot");
}

//----------------------------- getTimeOpponentHasBeenVisible ----------------------
//
//  returns the amount of time the given bot has been visible
//-----------------------------------------------------------------------------
double SensoryMemory::getTimeOpponentHasBeenVisible(AbstCharacter* const opponent) const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end() && it->second.within_view)
	{
		return Clock.getCurrentTime() - it->second.time_became_visible;
	}

	return 0;
}

int SensoryMemory::getDamage(AbstCharacter* const opponent)const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end())
	{
		return it->second.recent_damage;
	}

	return 0;
}

//-------------------- getTimeOpponentHasBeenOutOfView ------------------------
//
//  returns the amount of time the given opponent has remained out of view
//  returns a high value if opponent has never been seen or not present
//-----------------------------------------------------------------------------
double SensoryMemory::getTimeOpponentHasBeenOutOfView(AbstCharacter* const opponent)const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end())
	{
		return Clock.getCurrentTime() - it->second.time_last_visible;
	}

	return std::numeric_limits<double>::max();
}

//------------------------ getTimeSinceLastSensed ----------------------
//
//  returns the amount of time the given bot has been visible
//-----------------------------------------------------------------------------
double SensoryMemory::getTimeSinceLastSensed(AbstCharacter* const opponent)const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end() && it->second.within_view)
	{
		return Clock.getCurrentTime() - it->second.time_last_sensed;
	}

	return 0;
}

bool SensoryMemory::isUnderAttack() const
{
	MemoryMap::const_iterator rec = _memory_map.begin();
	for (rec; rec != _memory_map.end(); ++rec)
	{
		//if this bot has hit us, return true
		if ((*rec).second.recent_damage > 0)
			return true;
	}
	return false;
}