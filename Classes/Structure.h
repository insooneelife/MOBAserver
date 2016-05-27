#ifndef STRUCTURE_H
#define STRUCTURE_H
//------------------------------------------------------------------------
//
//  Name:   Structure.h
//
//  Desc:   Class for structures.
//			It has bounding shapes like circle, polygon or rectangle,
//			but it doesn't check collision with others.
//			It draws pictures.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <string>
#include <sstream>
#include "BaseGameEntity.h"

class GameWorld;

class Structure : public BaseGameEntity 
{
public:
	Structure(
		GameWorld* const		world,
		geometry::Shape* const	bounding_shape,
		const std::string&		name,
		const cocos2d::Vec2&	pos,
		double radius);

	static Structure* create(
		GameWorld* const		world,
		geometry::Shape* const	bounding_shape,
		const std::string&		name,
		const cocos2d::Vec2&	pos,
		double radius);

	virtual ~Structure() override;

	//this is defined as a pure virtual function in BasegameEntity,
	//so it must be implemented
	virtual void update(double time_elapsed) override;

	virtual void render() override;

	virtual bool handleMessage(const Telegram& msg) override;

private:
	Structure(const BaseGameEntity&) = delete; // no copies
	Structure& operator=(const Structure&) = delete; // no self-assignments
	Structure() = delete;
};



#endif

