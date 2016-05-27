#include "LineGoalThink.h"
#include "ExploreGoalEvaluator.h"
#include "AttackTargetGoalEvaluator.h"

LineGoalThink::LineGoalThink(AbstCharacter* const bot)
	:
	GoalThink(bot)
{
	const double low_range_bias = 0.5;
	const double high_range_bias = 1.5;

	double explore_bias = util::genRand(low_range_bias, high_range_bias);
	double attack_bias = util::genRand(low_range_bias + 1, high_range_bias + 1);

	_evaluators.emplace_back(new ExploreGoalEvaluator(explore_bias));
	_evaluators.emplace_back(new AttackTargetGoalEvaluator(attack_bias));
}

