#pragma once
#include <Scenes/Scene.h>
#include <Graphics/ProgressBar.h>
#include <Gameplay/Player/Player.h>
#include <Gameplay/Enemies/Enemy.h>

class BattleScene : public Scene
{
private:
	ProgressBar* m_PlayerHealthBar;
	ProgressBar* m_EnemyHealthBar;
	Player& m_Player;
	Enemy* m_Enemy;

	void SetupNewBattle();

public:
	BattleScene();
	~BattleScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

