#ifndef STATE_H
#define STATE_H
//------------------------------------------------------------------------
//
//  Name:   State.h
//
//  Desc:   abstract base class to define an interface for a state
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include "Singleton.h"

struct Telegram;

template <class EntityType>
class State
{
public:
  virtual ~State(){}

  //this will execute when the state is entered
  virtual void enter(EntityType*) = 0;

  //this is the states NORMAL update function
  virtual void execute(EntityType*) = 0;

  //this will execute when the state is exited. 
  virtual void exit(EntityType*) = 0;

  //this executes if the agent receives a message from the 
  //message dispatcher
  virtual bool onMessage(EntityType*, const Telegram&) = 0;
};

#endif