#include "pch.h"
#include "GreenPlayer.h"
#include <Graphics/Texture.h>
#include <Gameplay/Player/Player.h>
#include <Graphics/Animation.h>

constexpr const float c_EnemyAttackAnimationLength = 1.0f;
constexpr const float c_EnemyDyingAnimationLength = 2.5f;

GreenPlayer::GreenPlayer()
{
    std::vector<AnimationDetails> details;
    details =
    {
        /* idle   */ AnimationDetails(10, c_EnemyAttackAnimationLength, true),
        /* hurt   */ AnimationDetails(8,  c_EnemyAttackAnimationLength, false),
        /* death  */ AnimationDetails(17, c_EnemyDyingAnimationLength, false),
        /* cross  */ AnimationDetails(7,  c_EnemyAttackAnimationLength, false),
        /* jab    */ AnimationDetails(10, c_EnemyAttackAnimationLength, false),
        /* katana */ AnimationDetails(10, c_EnemyAttackAnimationLength, false)
    };

    LoadAnimation("Content/Spritesheets/Green_Player.png", details);
}

GreenPlayer::~GreenPlayer()
{

}

void GreenPlayer::DetermineAttributes(const Player& player)
{
	m_MaxHealth =  ((player.GetWinCount()) * 5) + INITIAL_ENTITY_HEALTH;
	m_CurrentHealth = m_MaxHealth;
	m_BaseDamage = 2 * (player.GetWinCount() + 1);
	m_RollRange = SDL_max(10 - player.GetWinCount(), 1);
}
