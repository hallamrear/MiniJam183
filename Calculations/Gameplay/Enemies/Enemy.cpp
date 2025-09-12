#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(EnemyDefinition& definition) : m_Definition(definition)
{
	m_MaxHealth = definition.MaxHealth;

	if (m_Definition.FrameDetails.size() > 0 && m_Definition.MaxFrameCount > 0 && m_Definition.AttackAnimationCount > 0)
	{
		LoadAnimation(m_Definition.SpriteLocation.c_str(), m_Definition.MaxFrameCount, m_Definition.FrameDetails);
	}
}

Enemy::~Enemy()
{

}

int Enemy::GetDamageRoll() const
{
	int diff = ((rand() % m_Definition.RollRange + 1) / 2);
	return m_Definition.BaseDamage + diff;
}

void Enemy::DetermineAttributes(const Player& player)
{
	m_CurrentHealth = m_MaxHealth;
	Heal(m_MaxHealth);
	SetIsAlive(true);
}

const int& Enemy::GetPossibleAttackAnimationCount() const
{
	return m_Definition.AttackAnimationCount;
}