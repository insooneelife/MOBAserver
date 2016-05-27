#pragma once
#include "State.h"

class Character;

class WalkState : public State<Character> {
public:
	WalkState(){}

public:
	void enter(Character* character) {}

	void execute(Character* character);

	void exit(Character* character) {}

	bool onMessage(Character*, const Telegram&);
};





