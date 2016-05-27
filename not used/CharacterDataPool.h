#pragma once
#include "Singleton.h"
#include "cocos2d.h"
#include <map>
#include <tuple>
#include <vector>

//------------------------------------------------------------------------
//
//  Name:   CharacterDataPool.h
//
//  Desc:   This class makes data structure for character from json file
//			and shares it for every class which needs them.
//			Flus, this class is designed with CRTP Singleton traits
//			which explained in c++ Idioms.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

//provide easy access
#define CharacterPool CharacterDataPool::getInstance()

class CharacterDataPool : public Singleton<CharacterDataPool> {
public:
	typedef std::map<
		std::string,
		std::tuple<
			std::vector<cocos2d::SpriteFrame*>,
			std::map<std::string, std::tuple<int, int> >,
		int
		>
	> DataMap;

	const std::tuple<
		std::vector<cocos2d::SpriteFrame*>,
		std::map<std::string, std::tuple<int, int> >,
		int
	>&  
		insert(
			const std::string& name, 
			const std::string& foldername, 
			int& directions);

private:
	CharacterDataPool(const CharacterDataPool&) = delete; // no copies
	CharacterDataPool& operator=(const CharacterDataPool&) = delete; // no self-assignments
	CharacterDataPool() {}
	friend class Singleton<CharacterDataPool>;

	//"character name" : [SpriteFrames] , {"action name" : [action frames, action cursor]}
	DataMap _pool;
};




