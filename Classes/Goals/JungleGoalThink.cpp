#include "JungleGoalThink.h"
#include "HoldPositionGoalEvaluator.h"
#include "AttackTargetGoalEvaluator.h"

JungleGoalThink::JungleGoalThink(AbstCharacter* const bot, const cocos2d::Vec2& destination)
	:
	GoalThink(bot)
{
	const double low_range_bias = 0.5;
	const double high_range_bias = 1.5;

	double hold_position_bias = util::genRand(low_range_bias, high_range_bias);
	double attack_bias = util::genRand(low_range_bias + 1, high_range_bias + 1);

	_evaluators.emplace_back(new HoldPositionGoalEvaluator(hold_position_bias, destination));
	_evaluators.emplace_back(new AttackTargetGoalEvaluator(attack_bias));
}
