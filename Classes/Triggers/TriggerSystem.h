#ifndef TRIGGERSYSTEM_H
#define TRIGGERSYSTEM_H
//-----------------------------------------------------------------------------
//
//  Name:    TriggerSystem.h
//
//  Author:  Mat Buckland (ai-junkie.com)
//
//  Desc:    Class to manage a collection of triggers. Triggers may be
//           registered with an instance of this class. The instance then 
//           takes care of updating those triggers and of removing them from
//           the system if their lifetime has expired.
//
//-----------------------------------------------------------------------------

#include <memory>

template <class Trigger>
class TriggerSystem
{
public:
	typedef std::list<std::unique_ptr<Trigger> > TriggerList;

public:

	~TriggerSystem()
	{
#ifdef SHOW_DTOR_INFO
		CCLOG("~Trigger()");
#endif
	}

	//getters && setters
	const TriggerList& getTriggers() const { return _triggers; }

	//This method should be called each update-step of the game. It will first
	//update the internal state odf the triggers and then try each entity against
	//each ACTIVE trigger to test if any should be triggered.
	template <class ContainerOfEntities>
	void update(ContainerOfEntities& entities, double time_elapsed)
	{
		updateTriggers(time_elapsed);
		tryTriggers(entities);
	}

	//this is used to register triggers with the TriggerSystem 
	//(the TriggerSystem will take care of tidying up memory used by a trigger)
	void registerTrigger(Trigger* trigger)
	{
		_triggers.emplace_back(trigger);
	}

	//some triggers are required to be rendered (like giver-triggers for example)
	void render()
	{}

private:
	//helper function template
	template<typename T>
	static inline T* helpToReturnRawPointer(const std::unique_ptr<T>& val)
	{
		return val.get();
	}

	template<typename T>
	static inline T* helpToReturnRawPointer(T* const val)
	{
		return val;
	}

	template<typename T>
	inline void helpToDeletePointer(std::unique_ptr<T>& val)
	{
		val.reset();
	}

	template<typename T>
	inline void helpToDeletePointer(T* const val)
	{
		delete val;
	}

	//this method iterates through all the triggers present in the system and
	//calls their update method in order that their internal state can be
	//updated if necessary. It also removes any triggers from the system that
	//have their _remove_from_game field set to true.
	void updateTriggers(double time_elapsed)
	{
		TriggerList::iterator trg = std::begin(_triggers);
		while (trg != std::end(_triggers))
		{
			//remove trigger if DEAD
			if ((*trg)->isToBeRemoved())
			{
				helpToDeletePointer(*trg);

				trg = _triggers.erase(trg);
			}
			else
			{
				//update this trigger
				(*trg)->update(time_elapsed);

				++trg;
			}
		}
	}

	//this method iterates through the container of entities passed as a
	//parameter and passes each one to the actIfOverlap method of each trigger *provided*
	//the entity is ALIVE and provided the entity is ready for a trigger update.
	template <class Container>
	void tryTriggers(const Container& entities)
	{
		for (Container::const_iterator ent = std::begin(entities);
		ent != std::end(entities); ++ent)
		{
			//an entity must be ready for its next trigger update and it must be 
			//ALIVE before it is tested against each trigger.
			if ((*ent)->isReadyForTriggerUpdate() && (*ent)->isAlive())
			{
				for (TriggerList::const_iterator trg = std::begin(_triggers);
				trg != std::end(_triggers); ++trg)
				{
					(*trg)->actIfOverlap(helpToReturnRawPointer(*ent));
				}
			}
		}
	}

	TriggerList   _triggers;
};


#endif