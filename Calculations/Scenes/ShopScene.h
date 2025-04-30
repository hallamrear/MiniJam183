#pragma once
#include <Scenes/Scene.h>
#include <Gameplay/Cards/OperandCard.h>
#include <Gameplay/Cards/NumberCard.h>

class Player;

constexpr const int c_ShopSlotCountX = 3;
constexpr const int c_ShopSlotCountY = 3;
constexpr const int c_ShopSlotCountTotal = c_ShopSlotCountX * c_ShopSlotCountY;
constexpr const float c_ButtonPadding = 32.0f;
constexpr const float c_ExitButtonWidth = 256.0f;
constexpr const float c_ExitButtonHeight = c_ExitButtonWidth / 6;

class ShopItem
{
private:
	bool m_HasBeenPurchased;
	bool m_UseNumberCard;
	OperandCard m_OperandCard;
	NumberCard m_NumberCard;

public:
	ShopItem();
	~ShopItem();

	void SetIsNumberCard(const bool& state);
	const bool& IsNumberCard() const;

	void SetPurchased(const bool& state);
	const bool& HasBeenPurchased() const;
	void GenerateRandomItem();
	void Reset();

	const OperandCard& GetGeneratedOperandCard() const;
	const NumberCard&  GetGeneratedNumberCard() const;
};

class ShopScene : public Scene
{
private:
	Player& m_Player;
	SDL_FRect m_IncreaseNumbersHandButtonRect;
	SDL_FRect m_IncreaseOperandsHandButtonRect;
	SDL_FRect m_ShopCentreRect;
	SDL_FRect m_ExitBattleButtonRect;

	SDL_Texture* m_CardBoughtTexture;

	SDL_FRect m_ShopSlotRects[c_ShopSlotCountTotal];
	ShopItem m_ShopItems[c_ShopSlotCountTotal];

	int m_IncreaseNumbersHandCost = 0;
	int m_IncreaseOperandHandCost = 0;
	int m_WindowWidth;
	int m_WindowHeight;
	int m_WindowCentreX;
	int m_WindowCentreY;
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

