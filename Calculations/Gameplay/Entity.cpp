#include "pch.h"
#include "Entity.h"

Entity::Entity()
{
	m_MaxHealth = INITIAL_ENTITY_HEALTH;
	m_CurrentHealth = m_MaxHealth;
	m_IsAlive = true;
    m_Texture = nullptr;
}

Entity::~Entity()
{
    if (m_Texture)
    {
        SDL_DestroyTexture(m_Texture);
        m_Texture = nullptr;
    }
}

SDL_Texture* Entity::GetTexture() const
{
    return m_Texture;
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