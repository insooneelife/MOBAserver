#ifndef MELEE_WEAPON_H
#define MELEE_WEAPON_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   MeleeWeapon
//
//  Author: insub im (insooneelife@naver.com)
//
//  Desc:   class to implement a MeleeWeapon
//-----------------------------------------------------------------------------
#include "AbstWeapon.h"
#include "cocos2d.h"


class AbstCharacter;

class MeleeWeapon : public AbstWeapon
{
private:

public:

	MeleeWeapon(AbstCharacter* owner);

	virtual void attack(cocos2d::Vec2 pos) override;

	virtual void attack() override;
};



#endif