#include "pch.h"
#include "Player.h"
#include <Graphics/Texture.h>

Player::Player()
{
    m_MaxHealth = INITIAL_PLAYER_HEALTH;
    m_CurrentHealth = m_MaxHealth;
    m_IsAlive = true;

    Texture::LoadPNG("Content/cat.png", m_Texture);
}

Player::~Player()
{

}

