#pragma once
#include <Scenes/Scene.h>

class MenuScene : public Scene
{
public:
	MenuScene(SceneManager& manager);
	~MenuScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

