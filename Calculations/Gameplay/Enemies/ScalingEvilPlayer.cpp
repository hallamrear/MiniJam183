#include "pch.h"
#include "ScalingEvilPlayer.h"
#include <Graphics/Texture.h>
#include <Gameplay/Player/Player.h>

ScalingEvilPlayer::ScalingEvilPlayer()
{
	Texture::LoadPNG("Content/cat2.png", m_Texture);
}

ScalingEvilPlayer::~ScalingEvilPlayer()
{

}

void ScalingEvilPlayer::DetermineAttributes(const Player& player)
{
	m_MaxHealth =  (player.GetWinCount() + 1) * INITIAL_ENTITY_HEALTH;
	m_CurrentHealth = m_MaxHealth;
	m_BaseDamage = 3 * (player.GetWinCount() + 1);
	m_RollRange = SDL_max(10 - player.GetWinCount() + player.GetGoldCount(), 1);
	m_BonusDamage += ((rand() % (player.GetGoldCount() + 1)) + player.GetWinCount());
}
