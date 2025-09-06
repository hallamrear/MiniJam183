#pragma once
#include <Gameplay/Enemies/Enemy.h>

class GreenPlayer : public Enemy
{
private:

public:
	GreenPlayer();
	~GreenPlayer();

	void DetermineAttributes(const Player& player);
};

