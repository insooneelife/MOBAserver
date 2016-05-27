#ifndef STATEMACHINE_H
#define STATEMACHINE_H

//------------------------------------------------------------------------
//
//  Name:   StateMachine.h
//
//  Desc:   State machine class. Inherit from this class and create some 
//          states to give your agents FSM functionality
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <cassert>
#include <string>
#include "State.h"
#include "Telegram.h"

template <class EntityType>
class StateMachine {
private:

	//a pointer to the agent that owns this instance
	EntityType*          _owner;

	State<EntityType>*   _current_state;

	//a record of the last state the agent was in
	State<EntityType>*   _previous_state;

	//this is called every time the FSM is updated
	State<EntityType>*   _global_state;

	bool				_use_state_machine;

public:
	StateMachine(EntityType* owner)
		:
		_owner(owner),
		_current_state(nullptr),
		_previous_state(nullptr),
		_global_state(nullptr),
		_use_state_machine(true)
	{}

	virtual ~StateMachine() 
	{}

	//use these methods to initialize the FSM
	void setCurrentState(State<EntityType>* s)	{	_current_state = s;		}

	void setGlobalState(State<EntityType>* s)	{	_global_state = s;		}

	void setPreviousState(State<EntityType>* s)	{	_previous_state = s;	}

	void off()						{ _use_state_machine = false; }

	void on()						{ _use_state_machine = true; }

	void isOn()						{ return _use_state_machine; }

	//call this to update the FSM
	void  update() const 
	{
		if (!_use_state_machine) return;

		//if a global state exists, call its execute method, else do nothing
		if (_global_state)   _global_state->execute(_owner);

		//same for the current state
		if (_current_state) _current_state->execute(_owner);
	}

	bool  handleMessage(const Telegram& msg) const 
	{
		//first see if the current state is valid and that it can handle
		//the message
		if (_current_state && _current_state->onMessage(_owner, msg)) {
			return true;
		}

		//if not, and if a global state has been implemented, send 
		//the message to the global state
		if (_global_state && _global_state->onMessage(_owner, msg)) {
			return true;
		}

		return false;
	}

	//change to a new state
	void  changeState(State<EntityType>* new_state) 
	{
		assert(new_state && "<StateMachine::ChangeState>:trying to assign null state to current");

		//keep a record of the previous state
		_previous_state = _current_state;

		//call the exit method of the existing state
		_current_state->exit(_owner);

		//change state to the new state
		_current_state = new_state;

		//call the entry method of the new state
		_current_state->enter(_owner);
	}

	//change state back to the previous state
	void  revertToPreviousState() 
	{
		changeState(_previous_state);
	}

	//returns true if the current state's type is equal to the type of the
	//class passed as a parameter. 
	bool  isInState(const State<EntityType>& st) const 
	{
		if (typeid(*_current_state) == typeid(st)) return true;
		return false;
	}

	State<EntityType>*  currentState()  const {	return _current_state;	}
	State<EntityType>*  globalState()   const {	return _global_state;	}
	State<EntityType>*  previousState() const {	return _previous_state;	}

	//only ever used during debugging to grab the name of the current state
	const std::string&	getNameOfCurrentState() const 
	{
		std::string s(typeid(*_current_state).name());

		//remove the 'class ' part from the front of the string
		if (s.size() > 5) {
			s.erase(0, 6);
		}
		return s;
	}
};


#endif


