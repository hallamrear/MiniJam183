#pragma once
#include "Scene.h"

class RestScene :
    public Scene
{
public:
	RestScene(SceneManager& manager);
	~RestScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

