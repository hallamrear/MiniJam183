#include "pch.h"
#include "MushroomEnemy.h"
#include <Graphics/Animation.h>

MushroomEnemy::MushroomEnemy()
{
    std::vector<AnimationDetails> details =
    {
        /* idle   */ AnimationDetails(4, c_DefaultEnemyAttackAnimationLength, true),
        /* hurt   */ AnimationDetails(5,  c_DefaultEnemyAttackAnimationLength, false),
        /* death  */ AnimationDetails(4, c_DefaultEnemyDyingAnimationLength, false),
        /* Attack_1  */ AnimationDetails(8, c_DefaultEnemyAttackAnimationLength, false),
        /* Attack_2  */ AnimationDetails(11, c_DefaultEnemyAttackAnimationLength, false),
    };

    m_PossibleAttackAnimationCount = 2;

    LoadAnimation("Content/Spritesheets/Mushroom/Mushroom.png", 11, details);
}

MushroomEnemy::~MushroomEnemy()
{

}
