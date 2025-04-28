#pragma once
#include <Scenes/Scene.h>

class Player;

class ShopScene : public Scene
{
private:
	const int c_ShopSlotCountX = 3;
	const int c_ShopSlotCountY = 3;
	const int c_ShopSlotCountTotal = c_ShopSlotCountX * c_ShopSlotCountY;
	const float c_ButtonPadding = 32.0f;
	const float c_ExitButtonWidth = 256.0f;
	const float c_ExitButtonHeight = c_ExitButtonWidth / 6;

	Player& m_Player;
	SDL_FRect m_IncreaseNumbersHandButtonRect;
	SDL_FRect m_IncreaseOperandsHandButtonRect;
	SDL_FRect m_ShopCentreRect;
	SDL_FRect m_ExitBattleButtonRect;

	SDL_FRect m_ShopSlotRects[9];

	int m_IncreaseHandCost;
	int m_WindowWidth;
	int m_WindowHeight;
	int m_WindowCentreX;
	int m_WindowCentreY;
	int m_MouseX;
	int m_MouseY;

	bool m_IsLeftClickDown;
	bool m_CanClickButtons;

	void RegenerateShop();
	void CalculateShopRects();
	void CalculateButtonRects();
	void CheckButtonClicks();

public:
	ShopScene(SceneManager& manager);
	~ShopScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

