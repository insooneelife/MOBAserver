#ifndef GOAL_PERSISTANCE_ATTACK_H
#define GOAL_PERSISTANCE_ATTACK_H

//-----------------------------------------------------------------------------
//
//  Name:   GoalPersistanceAttack.h
//
//  Author: insub im (insooneelife@naver.com)
//
//  Desc:
//-----------------------------------------------------------------------------
#include "Goals/GoalComposite.h"
#include "../GlobalValues.h"
#include "../AbstCharacter.h"


class GoalPersistanceAttack : public GoalComposite<AbstCharacter>
{
public:

	GoalPersistanceAttack(AbstCharacter* const owner);

	virtual ~GoalPersistanceAttack() override
	{}

	//the usual suspects
	virtual void	activate() override;

	virtual int		process() override;

	virtual void	terminate() override;

	virtual void	render() override;

protected:


};

#endif

