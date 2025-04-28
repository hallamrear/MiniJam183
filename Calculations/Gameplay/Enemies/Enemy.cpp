#include "pch.h"
#include "Enemy.h"

Enemy::Enemy()
{
	m_BaseDamage = 0;
	m_BonusDamage = 0;
	m_RollRange = 0;
}

Enemy::~Enemy()
{
}

int Enemy::GetDamageRoll() const
{
	int minDamage = m_BaseDamage - m_RollRange;
	int maxDamage = m_BaseDamage + m_RollRange;
	int range = maxDamage - minDamage + 1;
	int roll = rand() % range + minDamage;
	return roll;
}

void Enemy::DetermineAttributes(const Player& player)
{
	m_RollRange = INITIAL_ENEMY_ROLL_RANGE;
	m_BaseDamage = INITIAL_ENEMY_BASE_DAMAGE;
	m_BonusDamage = INITIAL_ENEMY_BONUS_DAMAGE;
}
