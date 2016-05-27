#include "TowerGoalThink.h"
#include "ExploreGoalEvaluator.h"
#include "AttackTargetGoalEvaluator.h"

TowerGoalThink::TowerGoalThink(AbstCharacter* const bot)
	:
	GoalThink(bot)
{
	const double low_range_bias = 0.5;
	const double high_range_bias = 1.5;

	double attack_bias = util::genRand(low_range_bias + 1, high_range_bias + 1);

	_evaluators.emplace_back(new AttackTargetGoalEvaluator(attack_bias));
}

