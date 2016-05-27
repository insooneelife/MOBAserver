#ifndef RANGE_WEAPON_H
#define RANGE_WEAPON_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   RangeWeapon
//
//  Author: insub im (insooneelife@naver.com)
//
//  Desc:   class to implement a range weapon
//-----------------------------------------------------------------------------
#include "AbstWeapon.h"
#include "cocos2d.h"


class AbstCharacter;

class RangeWeapon : public AbstWeapon
{
public:
	RangeWeapon(AbstCharacter* owner, const std::string& name);

	virtual void attack(cocos2d::Vec2 pos) override;

	virtual void attack() override;

private:
	std::string _name;
};



#endif