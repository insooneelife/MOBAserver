#include "BuffNerf.h"

USING_NS_CC;

BuffNerf::BuffNerf()
{}

BuffNerf::~BuffNerf()
{}

ExtraCharacterInfo BuffNerf::calculate()
{
	ExtraCharacterInfo accum;
	
	for (auto e = std::begin(_buffs_nerfs); e != std::end(_buffs_nerfs); e++)
	{
		accum.damage += (*e)->damage;

		accum.mdamage += (*e)->mdamage;

		accum.max_health += (*e)->max_health;

		accum.max_mana += (*e)->max_mana;

		accum.protect += (*e)->protect;

		accum.mprotect += (*e)->mprotect;

		accum.speed += (*e)->speed;
	}

	return accum;
}

void BuffNerf::add(const ExtraCharacterInfo& extra_info)
{
	_buffs_nerfs.push_back(std::make_unique<ExtraCharacterInfo>(extra_info));
}

void BuffNerf::remove(const std::string& name)
{
	auto iter = std::begin(_buffs_nerfs);

	for (; iter != std::end(_buffs_nerfs); iter++)
	{
		if ((*iter)->name == name)
		{
			_buffs_nerfs.erase(iter);
			return;
		}
	}
	
}