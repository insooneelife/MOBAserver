#ifndef BUFF_NERF_H
#define BUFF_NERF_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
// 각 캐릭터의 버프와 너프
//-----------------------------------------------------------------------------
#include "cocos2d.h"
#include "AbstCharacter.h"
#include <list>
#include <memory>


struct ExtraCharacterInfo
{
	std::string name;
	int damage;
	int mdamage;
	int max_health;
	int max_mana;
	int protect;
	int mprotect;
	int speed;
	ExtraCharacterInfo()
		:
		name(""),
		damage(0),
		mdamage(0),
		max_health(0),
		max_mana(0),
		protect(0),
		mprotect(0),
		speed(0)
	{}
};

class BuffNerf
{
public:

	BuffNerf();

	~BuffNerf();

	ExtraCharacterInfo calculate();

	void add(const ExtraCharacterInfo& extra_info);
	void remove(const std::string& name);

private:
	//AbstCharacter* const	_owner;
	std::list<std::unique_ptr<ExtraCharacterInfo> > _buffs_nerfs;
};




#endif
