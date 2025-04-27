#pragma once
#include <Scenes/Scene.h>

class MenuScene : public Scene
{
private:
	SDL_FRect m_ExitButtonRect;
	SDL_FRect m_PlayButtonRect;

	SDL_Texture* m_BackgroundTexture;
	SDL_Texture* m_ExitButtonTexture;
	SDL_Texture* m_PlayButtonTexture;

public:
	MenuScene(SceneManager& manager);
	~MenuScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

