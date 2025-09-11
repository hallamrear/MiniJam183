#pragma once
#include <Gameplay/Entity.h>

class Player;

class Enemy : public Entity
{
protected:
	const float c_DefaultEnemyAttackAnimationLength = 1.0f;
	const float c_DefaultEnemyDyingAnimationLength = 2.5f;

	int m_BaseDamage;
	int m_RollRange;
	int m_PossibleAttackAnimationCount;

public:
	enum ANIMATION_STATES
	{
		IDLE = 0,
		HURT = 1,
		DEATH = 2,
		ATTACK_1 = 3,
		ATTACK_2 = 4,
		ATTACK_3 = 5
	};

	Enemy();
	~Enemy();

	virtual int GetDamageRoll() const;
	virtual void DetermineAttributes(const Player& player);

	const int& GetPossibleAttackAnimationCount() const;
};