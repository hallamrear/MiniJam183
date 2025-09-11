#include "pch.h"
#include "ScalingEvilPlayer.h"
#include <Graphics/Texture.h>
#include <Gameplay/Player/Player.h>
#include <Graphics/Animation.h>

ScalingEvilPlayer::ScalingEvilPlayer()
{
    std::vector<AnimationDetails> details;
    details =
    {
        /* idle   */ AnimationDetails(10, c_DefaultEnemyAttackAnimationLength, true),
        /* hurt   */ AnimationDetails(8,  c_DefaultEnemyAttackAnimationLength, false),
        /* death  */ AnimationDetails(16, c_DefaultEnemyDyingAnimationLength, false),
        /* cross  */ AnimationDetails(7,  c_DefaultEnemyAttackAnimationLength, false),
        /* jab    */ AnimationDetails(10, c_DefaultEnemyAttackAnimationLength, false),
        /* katana */ AnimationDetails(10, c_DefaultEnemyAttackAnimationLength, false)
    };

    m_PossibleAttackAnimationCount = 3;

    LoadAnimation("Content/Spritesheets/Blue_Player.png", 16, details);
}

ScalingEvilPlayer::~ScalingEvilPlayer()
{

}

void ScalingEvilPlayer::DetermineAttributes(const Player& player)
{
	m_MaxHealth =  ((player.GetWinCount()) * 5) + INITIAL_ENTITY_HEALTH;
	m_CurrentHealth = m_MaxHealth;
	m_BaseDamage = 2 * (player.GetWinCount() + 1);
	m_RollRange = SDL_max(10 - player.GetWinCount(), 1);
}
