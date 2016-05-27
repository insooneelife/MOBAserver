#ifndef __CHARACTERBASESTATES_H__
#define __CHARACTERBASESTATES_H__
//------------------------------------------------------------------------
//
//  Name: FieldPlayerStates.h
//
//  Desc: States for the field players of Simple Soccer. See my book
//        for detailed descriptions
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <string>
#include "State.h"

class Character;
class GameWorld;

//--------------------------CharacterBaseGlobalState-----------------------------//
class CharacterBaseGlobalState : public State<Character>,
								 public Singleton<CharacterBaseGlobalState>
{
public:

	void enter(Character* character) {}

	void execute(Character* character);

	void exit(Character* character) {}

	bool onMessage(Character*, const Telegram&);

private:
	CharacterBaseGlobalState(const CharacterBaseGlobalState&) = delete; // no copies
	CharacterBaseGlobalState& operator=(const CharacterBaseGlobalState&) = delete; // no self-assignments
	CharacterBaseGlobalState() {}
	friend class Singleton<CharacterBaseGlobalState>;
};

//-------------------------------Neutral---------------------------------//

class Neutral : public State<Character>,
				public Singleton<Neutral>
{
public:

	void enter(Character* character);

	void execute(Character* character);

	void exit(Character* character) {}

	bool onMessage(Character*, const Telegram&) { return false; }

private:
	Neutral(const Neutral&) = delete; // no copies
	Neutral& operator=(const Neutral&) = delete; // no self-assignments
	Neutral() {}
	friend class Singleton<Neutral>;
};

//---------------------------------Idle---------------------------------//

class Idle : public State<Character>,
			 public Singleton<Idle>
{
public:

	void enter(Character* character);

	void execute(Character* character);

	void exit(Character* character);

	bool onMessage(Character*, const Telegram&) { return false; }

private:
	Idle(const Idle&) = delete; // no copies
	Idle& operator=(const Idle&) = delete; // no self-assignments
	Idle() {}
	friend class Singleton<Idle>;
};


//-------------------------------ChaseEnemy--------------------------------//

class ChaseEnemy :  public State<Character>,
					public Singleton<ChaseEnemy>
{
public:

	void enter(Character* character);

	void execute(Character* character);

	void exit(Character* character);

	bool onMessage(Character*, const Telegram&) { return false; }

private:
	ChaseEnemy(const ChaseEnemy&) = delete; // no copies
	ChaseEnemy& operator=(const ChaseEnemy&) = delete; // no self-assignments
	ChaseEnemy() {}
	friend class Singleton<ChaseEnemy>;
};


//-------------------------------ChaseEnemy--------------------------------//

class Attack : public State<Character>,
			   public Singleton<Attack>
{
public:

	void enter(Character* character);

	void execute(Character* character);

	void exit(Character* character);

	bool onMessage(Character*, const Telegram&) { return false; }

private:
	Attack(const Attack&) = delete; // no copies
	Attack& operator=(const Attack&) = delete; // no self-assignments
	Attack() {}
	friend class Singleton<Attack>;
};


/*

class ChaseBall : public State<Character>
{
private:

ChaseBall(){}

public:

//this is a singleton
static ChaseBall* Instance();

void Enter(Character* character);

void Execute(Character* character);

void Exit(Character* character);

bool OnMessage(Character*, const Telegram&){return false;}
};


//------------------------------------------------------------------------
class Dribble : public State<Character>
{
private:

  Dribble(){}

public:

  //this is a singleton
  static Dribble* Instance();

  void Enter(Character* player);

  void Execute(Character* player);

  void Exit(Character* player){}

  bool OnMessage(Character*, const Telegram&){return false;}
};


//------------------------------------------------------------------------
class ReturnToHomeRegion: public State<Character>
{
private:

  ReturnToHomeRegion(){}

public:

  //this is a singleton
  static ReturnToHomeRegion* Instance();

  void Enter(Character* player);

  void Execute(Character* player);

  void Exit(Character* player);

  bool OnMessage(Character*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Wait: public State<Character>
{
private:

  Wait(){}

public:

  //this is a singleton
  static Wait* Instance();

  void Enter(Character* player);

  void Execute(Character* player);

  void Exit(Character* player);

  bool OnMessage(Character*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class KickBall: public State<Character>
{
private:

  KickBall(){}

public:

  //this is a singleton
  static KickBall* Instance();

  void Enter(Character* player);

  void Execute(Character* player);

  void Exit(Character* player){}

  bool OnMessage(Character*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class ReceiveBall: public State<Character>
{
private:

  ReceiveBall(){}

public:

  //this is a singleton
  static ReceiveBall* Instance();

  void Enter(Character* player);

  void Execute(Character* player);

  void Exit(Character* player);

  bool OnMessage(Character*, const Telegram&){return false;}
};


//------------------------------------------------------------------------
class SupportAttacker: public State<Character>
{
private:

  SupportAttacker(){}

public:

  //this is a singleton
  static SupportAttacker* Instance();

  void Enter(Character* player);

  void Execute(Character* player);

  void Exit(Character* player);

  bool OnMessage(Character*, const Telegram&){return false;}
};


*/


#endif