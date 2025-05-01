#pragma once
#include <Gameplay/Entity.h>

class Player;

class Enemy : public Entity
{
protected:
	int m_BaseDamage;
	int m_RollRange;

public:
	Enemy();
	~Enemy();

	virtual int GetDamageRoll() const;
	virtual void DetermineAttributes(const Player& player);
};