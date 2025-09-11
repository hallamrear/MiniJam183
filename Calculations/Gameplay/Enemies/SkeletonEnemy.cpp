#include "pch.h"
#include "SkeletonEnemy.h"
#include <Graphics/Animation.h>

SkeletonEnemy::SkeletonEnemy()
{
    std::vector<AnimationDetails> details =
    {
        /* idle   */ AnimationDetails(11, c_DefaultEnemyAttackAnimationLength, true),
        /* hurt   */ AnimationDetails(8,  c_DefaultEnemyAttackAnimationLength, false),
        /* death  */ AnimationDetails(15, c_DefaultEnemyDyingAnimationLength, false),
        /* down swing  */ AnimationDetails(18, c_DefaultEnemyAttackAnimationLength, false)
    };

    m_PossibleAttackAnimationCount = 1;

    LoadAnimation("Content/Spritesheets/Skeleton/Skeleton.png", 18, details);
}

SkeletonEnemy::~SkeletonEnemy()
{

}