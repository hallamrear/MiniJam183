#include "pch.h"
#include "SkeletonEnemy.h"
#include <Graphics/Animation.h>

constexpr const float c_EnemyAttackAnimationLength = 1.0f;
constexpr const float c_EnemyDyingAnimationLength = 2.5f;

SkeletonEnemy::SkeletonEnemy()
{
    std::vector<AnimationDetails> details =
    {
        /* idle   */ AnimationDetails(11, c_EnemyAttackAnimationLength, true),
        /* hurt   */ AnimationDetails(8,  c_EnemyAttackAnimationLength, false),
        /* death  */ AnimationDetails(15, c_EnemyDyingAnimationLength, false),
        /* down swing  */ AnimationDetails(18,  c_EnemyAttackAnimationLength, false),
        AnimationDetails(0, 0, false),
        AnimationDetails(0, 0, false),
        AnimationDetails(0, 0, false),
        AnimationDetails(0, 0, false),
        AnimationDetails(0, 0, false),
        AnimationDetails(0, 0, false),
        AnimationDetails(0, 0, false),
        AnimationDetails(0, 0, false),
        AnimationDetails(0, 0, false),
        AnimationDetails(0, 0, false),
        AnimationDetails(0, 0, false),
    };

    LoadAnimation("Content/Spritesheets/Skeleton/Skeleton.png", 18, details);
}

SkeletonEnemy::~SkeletonEnemy()
{

}