#ifndef SENSORY_SYSTEM_H
#define SENSORY_SYSTEM_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:
//
//-----------------------------------------------------------------------------
#include <map>
#include <list>
#include <sstream>
#include "cocos2d.h"

class AbstCharacter;

class MemoryRecord
{
public:

	//records the time the opponent was last sensed (seen or heard). This
	//is used to determine if a bot can 'remember' this record or not. 
	//(if CurrentTime() - time_last_sensed is greater than the bot's
	//memory span, the data in this record is made unavailable to clients)
	double			time_last_sensed;

	//it can be useful to know how long an opponent has been visible. This 
	//variable is tagged with the current time whenever an opponent first becomes
	//visible. It's then a simple matter to calculate how long the opponent has
	//been in view (CurrentTime - time_became_visible)
	double			time_became_visible;

	//it can also be useful to know the last time an opponent was seen
	double			time_last_visible;

	//a vector marking the position where the opponent was last sensed. This can
	// be used to help hunt down an opponent if it goes out of view
	cocos2d::Vec2	last_sensed_position;

	//set to true if opponent is within the field of view of the owner
	bool			within_view;

	//set to true if there is no obstruction between the opponent and the owner, 
	//permitting a shot.
	bool			attackable;

	int				recent_damage;


	MemoryRecord()
		:
		time_last_sensed(-999),
		time_became_visible(-999),
		time_last_visible(0),
		within_view(false),
		attackable(false),
		recent_damage(0)
	{}

	friend std::stringstream & operator<<(std::stringstream &ss, const MemoryRecord& b);
};


class SensoryMemory
{
private:

	typedef std::map<AbstCharacter*, MemoryRecord> MemoryMap;

private:

	//the owner of this instance
	AbstCharacter* const	_owner;

	//this container is used to simulate memory of sensory events. A MemoryRecord
	//is created for each opponent in the environment. Each record is updated 
	//whenever the opponent is encountered. (when it is seen or heard)
	MemoryMap				_memory_map;

	//a bot has a memory span equivalent to this value. When a bot requests a 
	//list of all recently sensed opponents this value is used to determine if 
	//the bot is able to remember an opponent or not.
	double					_memory_span;

	//this methods checks to see if there is an existing record for bot. If
	//not a new MemoryRecord record is made and added to the memory map.(called
	//by updateWithSoundSource & updateVision)
	void       makeNewRecordIfNotAlreadyPresent(AbstCharacter* bot);

public:

	SensoryMemory(AbstCharacter* const owner, double memory_span);

	~SensoryMemory();

	//this method is used to update the memory map whenever an opponent makes
	//a noise
	void	updateWithSoundSource(AbstCharacter* const noise_maker);

	void	updateWithDamageSource(AbstCharacter* const shooter, int damage);

	//this removes a bot's record from memory
	void	removeBotFromMemory(AbstCharacter* const bot);

	//this method iterates through all the opponents in the game world and 
	//updates the records of those that are in the owner's FOV(field of view)
	void	updateVision();

	//this method renders the infomation about sensory memory
	void	render();

	bool			isOpponentAttackable(AbstCharacter* const opponent)const;
	bool			isOpponentWithinFOV(AbstCharacter* const opponent)const;
	cocos2d::Vec2	getLastRecordedPositionOfOpponent(AbstCharacter* const opponent)const;
	double			getTimeOpponentHasBeenVisible(AbstCharacter* const opponent)const;
	double			getTimeSinceLastSensed(AbstCharacter* const opponent)const;
	double			getTimeOpponentHasBeenOutOfView(AbstCharacter* const opponent)const;
	int				getDamage(AbstCharacter* const opponent)const;
	bool			isUnderAttack() const;

	//this method returns a list of all the opponents that have had their
	//records updated within the last _memory_span seconds.
	std::list<AbstCharacter*> getListOfRecentlySensedOpponents()const;


};


#endif