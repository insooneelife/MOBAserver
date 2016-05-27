#include "Database.h"
#include "Util.h"

Database::Database()
{
	loadCharacters();
	loadSkills();
	loadItems();
}

void Database::loadCharacters()
{
	rapidjson::Document json;
	util::readJSON("characters/characters.json", json);

	for (rapidjson::Value::ValueIterator it = json.Begin(); it != json.End(); it++)
	{
		std::string cname = it->GetString();
		rapidjson::Document json;
		util::readJSON("characters/" + cname + "/" + cname + ".json", json);

		int hp = json.FindMember("data")->value.FindMember("hp")->value.GetInt();
		int mp = json.FindMember("data")->value.FindMember("mp")->value.GetInt();
		int damage = json.FindMember("data")->value.FindMember("damage")->value.GetInt();
		int mdamage = json.FindMember("data")->value.FindMember("mdamage")->value.GetInt();
		int defence = json.FindMember("data")->value.FindMember("defence")->value.GetInt();
		int mdefence = json.FindMember("data")->value.FindMember("mdefence")->value.GetInt();
		int attack_range = json.FindMember("data")->value.FindMember("attack_range")->value.GetInt();
		int view_range = json.FindMember("data")->value.FindMember("view_range")->value.GetInt();
		int move_speed = json.FindMember("data")->value.FindMember("move_speed")->value.GetInt();
		int attack_speed = json.FindMember("data")->value.FindMember("attack_speed")->value.GetInt();
		int gold_worth = json.FindMember("data")->value.FindMember("gold_worth")->value.GetInt();
		int exp_worth = json.FindMember("data")->value.FindMember("exp_worth")->value.GetInt();

		_character_data.emplace(
			cname,
			std::make_tuple(
				hp, mp, damage, mdamage, defence, mdefence,
				attack_range, view_range, move_speed, attack_speed, gold_worth, exp_worth));

		//std::cout << hp << " " << mp << " " << damage << " " << mdamage << " " << defence << " " << mdefence 
		//<< " " << attack_range << " " << view_range << " " << move_speed << " " << attack_speed << " " << gold_worth << std::endl;


		/*"attack" : "Arrow",
		"skill1" : "",
		"skill2" : "",
		"skill3" : "",
		"skill4" : ""*/
		std::string attack = json.FindMember("strategy")->value.FindMember("attack")->value.GetString();
		std::string skill1 = json.FindMember("strategy")->value.FindMember("skill1")->value.GetString();
		std::string skill2 = json.FindMember("strategy")->value.FindMember("skill2")->value.GetString();
		std::string skill3 = json.FindMember("strategy")->value.FindMember("skill3")->value.GetString();
		std::string skill4 = json.FindMember("strategy")->value.FindMember("skill4")->value.GetString();

		_character_skill.emplace(
			cname,
			std::make_tuple(attack, skill1, skill2, skill3, skill4));
	}
}



void Database::loadSkills()
{
	rapidjson::Document json;
	util::readJSON("skills/skills.json", json);
	for (rapidjson::Value::ValueIterator it = json.Begin(); it != json.End(); it++)
	{
		std::string cname = it->GetString();
		CCLOG("%s", cname.c_str());
		rapidjson::Document json;
		util::readJSON("skills/" + cname + "/" + cname + ".json", json);

		/*   "name": "FireBall",
		"data" : {
		"damage": 10,
		"mana" : 10,
		"emission_rate" : 100,
		"duration" : 100,
		"scale" : 0.3f,
		"speed" : 40,
		"period" : 0,
		"time_limit" : 0,
		"splash_radius" : 0,
		"survive_time" : 0*/

		std::string type = json.FindMember("data")->value.FindMember("type")->value.GetString();
		int damage = json.FindMember("data")->value.FindMember("damage")->value.GetInt();
		int mana = json.FindMember("data")->value.FindMember("mana")->value.GetInt();
		int radius = json.FindMember("data")->value.FindMember("radius")->value.GetInt();
		int speed = json.FindMember("data")->value.FindMember("speed")->value.GetInt();

		bool emitter_flag = json.FindMember("data")->value.FindMember("emitter_flag")->value.GetBool();
		bool explosion_flag = json.FindMember("data")->value.FindMember("explosion_flag")->value.GetBool();

		int period = json.FindMember("data")->value.FindMember("period")->value.GetInt();
		int splash_radius = json.FindMember("data")->value.FindMember("splash_radius")->value.GetInt();
		double survive_time = json.FindMember("data")->value.FindMember("survive_time")->value.GetDouble();

		int emitter_emission_rate = json.FindMember("data")->value.FindMember("emitter_emission_rate")->value.GetInt();
		double emitter_duration = json.FindMember("data")->value.FindMember("emitter_duration")->value.GetDouble();
		double emitter_scale = json.FindMember("data")->value.FindMember("emitter_scale")->value.GetDouble();
		int emitter_speed = json.FindMember("data")->value.FindMember("emitter_speed")->value.GetInt();

		int explosion_emission_rate = json.FindMember("data")->value.FindMember("explosion_emission_rate")->value.GetInt();
		double explosion_duration = json.FindMember("data")->value.FindMember("explosion_duration")->value.GetDouble();
		double explosion_scale = json.FindMember("data")->value.FindMember("explosion_scale")->value.GetDouble();
		int explosion_speed = json.FindMember("data")->value.FindMember("explosion_speed")->value.GetInt();

		_skill_data.emplace(
			cname,
			std::make_tuple(
				type, damage, mana, radius, speed, emitter_flag, explosion_flag, period, splash_radius, survive_time,
				emitter_emission_rate, emitter_duration, emitter_scale, emitter_speed,
				explosion_emission_rate, explosion_duration, explosion_scale, explosion_speed));

		//std::cout << hp << " " << mp << " " << damage << " " << mdamage << " " << defence << " " << mdefence 
		//<< " " << attack_range << " " << view_range << " " << move_speed << " " << attack_speed << " " << gold_worth << std::endl;


		/*"attack" : "Arrow",
		"skill1" : "",
		"skill2" : "",
		"skill3" : "",
		"skill4" : ""*/
	}

}



void Database::loadItems()
{
	rapidjson::Document json;
	util::readJSON("items/items.json", json);

	for (rapidjson::Value::ValueIterator it = json.Begin(); it != json.End(); it++)
	{
		std::string cname = it->GetString();
		rapidjson::Document json;
		util::readJSON("items/" + cname + "/" + cname + ".json", json);

		int max_hp = json.FindMember("data")->value.FindMember("max_hp")->value.GetInt();
		int max_mp = json.FindMember("data")->value.FindMember("max_mp")->value.GetInt();
		int damage = json.FindMember("data")->value.FindMember("damage")->value.GetInt();
		int mdamage = json.FindMember("data")->value.FindMember("mdamage")->value.GetInt();
		int defence = json.FindMember("data")->value.FindMember("defence")->value.GetInt();
		int mdefence = json.FindMember("data")->value.FindMember("mdefence")->value.GetInt();
		int move_speed = json.FindMember("data")->value.FindMember("move_speed")->value.GetInt();
		int price = json.FindMember("data")->value.FindMember("price")->value.GetInt();

		_item_data.emplace(cname, std::make_tuple(max_hp, max_mp, damage, mdamage, defence, mdefence, move_speed, price));

		//std::cout << hp << " " << mp << " " << damage << " " << mdamage << " " << defence << " " << mdefence 
		//<< " " << attack_range << " " << view_range << " " << move_speed << " " << attack_speed << " " << worth << std::endl;
	}
}
