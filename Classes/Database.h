#pragma once

#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "Singleton.h"
#include "json\reader.h"
#include "json\document.h"

//provide easy access
#define DB Database::getInstance()

class Database : public Singleton<Database>
{
public:

	std::map<std::string, std::tuple<int, int, int, int, int, int, double, double, int, int, int, int> >&
		getCharacterData()
	{
		return _character_data;
	}

	std::map<std::string, std::tuple<std::string, std::string, std::string, std::string, std::string> >&
		getCharacterSkill()
	{
		return _character_skill;
	}

	std::map<std::string, std::tuple<std::string, int, int, int, int, bool, bool,
		int, int, double,
		int, double, double, int,
		int, double, double, int> >&
		getSkillData()
	{
		return _skill_data;
	}

	std::map<std::string, std::tuple<int, int, int, int, int, int, int, int>>& getItemData() 
	{
		return _item_data;
	}

private:
	Database();
	Database(const Database&) = delete; // no copies
	Database& operator=(const Database&) = delete; // no self-assignments
	friend class Singleton<Database>;

	void loadCharacters();

	void loadSkills();

	void loadItems();



	/*
	int hp,               0
	int mp,               1
	int damage,            2
	int mdamage,         3
	int defence,         4
	int mdefence,         5
	double attack_range,   6
	double view_range,      7
	int move_speed,         8
	int attack_speed,      9
	int gold_worth         10
	int exp_worth         11
	*/
	std::map<std::string, std::tuple<int, int, int, int, int, int, double, double, int, int, int, int> >
		_character_data;

	std::map<std::string, std::tuple<std::string, std::string, std::string, std::string, std::string> >
		_character_skill;

	std::map<std::string, std::tuple<std::string, int, int, int, int, bool, bool,
		int, int, double,
		int, double, double, int,
		int, double, double, int> >
		_skill_data;

	std::map<std::string, std::tuple<int, int, int, int, int, int, int, int>>
		_item_data;
};