#include "pch.h"
#include "Entity.h"
#include <Graphics/Animation.h>

Entity::Entity()
{
	m_MaxHealth = INITIAL_ENTITY_HEALTH;
	m_CurrentHealth = m_MaxHealth;
	m_IsAlive = true;
    m_Animation = nullptr;
}

bool Entity::LoadAnimation(const std::string& sheetPath, const unsigned int& numberOfAnimations, const unsigned int& frameCount, const float& duration, const bool& looping)
{
    if (m_Animation != nullptr)
    {
        DestroyAnimation();
    }

    m_Animation = new AnimationController(sheetPath, numberOfAnimations, frameCount, duration, looping);

    return (m_Animation->GetSpriteSheet() != nullptr);
}

Entity::~Entity()
{
    DestroyAnimation();
}

void Entity::DestroyAnimation()
{
    if (m_Animation != nullptr)
    {
        delete m_Animation;
        m_Animation = nullptr;
    }
}

AnimationController* Entity::GetAnimation() const
{
    return m_Animation;
}

const void Entity::SetIsAlive(const bool& state)
{
	m_IsAlive = state;
}

const bool Entity::GetIsAlive() const
{
	return m_IsAlive;
}

const void Entity::Kill()
{
	m_CurrentHealth = 0;
	SetIsAlive(false);
}

const int& Entity::GetCurrentHealth() const
{
    return m_CurrentHealth;
}

const int& Entity::GetMaxHealth() const
{
    return m_MaxHealth;
}

void Entity::TakeDamage(const int& damage)
{
    m_CurrentHealth -= damage;
    m_CurrentHealth = SDL_max(m_CurrentHealth, 0);

    if (m_CurrentHealth == 0)
    {
        m_IsAlive = false;
    }
}

void Entity::Heal(const int& heal)
{
    m_CurrentHealth += heal;
    m_CurrentHealth = SDL_min(m_CurrentHealth, m_MaxHealth);

    if (m_CurrentHealth > 0)
    {
        m_IsAlive = true;
    }
}