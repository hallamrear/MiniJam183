#pragma once
#include <Gameplay/Enemies/Enemy.h>

class ScalingEvilPlayer : public Enemy
{
private:

public:
	ScalingEvilPlayer();
	~ScalingEvilPlayer();

	void DetermineAttributes(const Player& player);
};

