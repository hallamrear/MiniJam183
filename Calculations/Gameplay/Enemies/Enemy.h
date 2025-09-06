#pragma once
#include <Gameplay/Entity.h>

class Player;

class Enemy : public Entity
{
protected:
	int m_BaseDamage;
	int m_RollRange;

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
};