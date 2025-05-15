#pragma once
#include "Scene.h"

class RestScene :
    public Scene
{
private:
	SDL_Window& m_Window;
	float m_ClickCooldown;
	SDL_Texture* m_LitCampfireTexture;
	SDL_Texture* m_UnlitCampfireTexture;
	SDL_Texture* m_ReturnButtonTexture;
	TTF_Text* m_ReturnText;
	TTF_Text* m_DetailsText;
	int m_HealAmount;
	SDL_FRect m_ReturnButton;
	SDL_FRect m_CampfireRect;
	SDL_Point m_DetailTextOffset;
	int m_WindowWidth;
	int m_WindowHeight;
	bool m_HasBeenUsed;

public:
	RestScene(SceneManager& manager);
	~RestScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

