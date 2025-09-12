#pragma once
#include <Gameplay/Entity.h>
#include <Gameplay/Enemies/EnemyDefinition.h>

class Player;

class Enemy : public Entity
{
private:
	friend class EnemyDefinition;
	EnemyDefinition& m_Definition;

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

	Enemy(EnemyDefinition& definition);
	~Enemy();

	int GetDamageRoll() const;
	void DetermineAttributes(const Player& player);

	const int& GetPossibleAttackAnimationCount() const;
};