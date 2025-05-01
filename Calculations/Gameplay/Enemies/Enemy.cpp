#include "pch.h"
#include "Enemy.h"

Enemy::Enemy()
{
	m_BaseDamage = 0;
	m_RollRange = 0;
}

Enemy::~Enemy()
{
	m_BaseDamage = 0;
	m_RollRange = 0;
}

int Enemy::GetDamageRoll() const
{
	int roll = SDL_max((rand() % m_RollRange), 1);
	int damage = SDL_max((roll - (m_RollRange / 2)) + m_BaseDamage, 1);

	#ifdef _DEBUG
	SDL_Log("Roll: %i", roll);
	SDL_Log("Damage %i", damage);
	#endif

	return damage;
}

void Enemy::DetermineAttributes(const Player& player)
{
	m_MaxHealth = INITIAL_ENTITY_HEALTH;
	m_CurrentHealth = m_MaxHealth;
	m_RollRange = INITIAL_ENEMY_ROLL_RANGE;
	m_BaseDamage = INITIAL_ENEMY_BASE_DAMAGE;
}
