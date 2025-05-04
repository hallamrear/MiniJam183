#pragma once
#include <Gameplay/Enemies/Enemy.h>

class GreenPlayer : public Enemy
{
private:

public:
	enum ANIMATION_STATES
	{
		IDLE = 0,
		HURT = 1,
		DEATH = 2,
		ATTACK_1 = 3,
		ATTACK_2 = 4,
		ATTACK_3 = 5
	};

	GreenPlayer();
	~GreenPlayer();

	void DetermineAttributes(const Player& player);
};

