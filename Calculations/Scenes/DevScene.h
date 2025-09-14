#pragma once
#include <Scenes/Scene.h>
#include <Gameplay/Cards/NumberCard.h>
#include <Gameplay/Cards/OperandCard.h>
#include <Gameplay/Cards/Deck.h>

class DevScene :
    public Scene
{
private:
	std::vector<SDL_FColor> m_TestPalettes;

	const int m_ButtonSpacing;
	SDL_Window& m_Window;
	TTF_Text* m_Text;

	NumberCard m_CardInConstruction;
	NumberCard* m_SelectedLeftCard;
	OperandCard* m_SelectedOperandCard;
	NumberCard* m_SelectedRightCard;
	bool m_IsValidEquation;

	std::vector<Card*> m_DeckCards;
	std::vector<OperandCard*> m_OperandCardSet;
	std::vector<NumberCard*> m_LeftNumberCardSet;
	std::vector<NumberCard*> m_RightNumberCardSet;

	SDL_FRect m_LeftNumberCardDrawRect;
	SDL_FRect m_OperandCardDrawRect;
	SDL_FRect m_RightNumberCardDrawRect;
	SDL_FRect m_EqualsSignDrawRect;
	SDL_FRect m_ConstructedCardDrawRect;
	SDL_FRect m_ConstructNewCardButtonRect;

	SDL_FRect m_OperandSelectionForwardButton;
	SDL_FRect m_OperandSelectionBackwardButton;
	SDL_FRect m_LeftNumberCardSelectionForwardButton;
	SDL_FRect m_LeftNumberCardSelectionBackwardButton;
	SDL_FRect m_RightNumberCardSelectionForwardButton;
	SDL_FRect m_RightNumberCardSelectionBackwardButton;

	int m_RightCardPreviewIndex;
	int m_OperandCardPreviewIndex;
	int m_LeftCardPreviewIndex;

	void IncrementLeftCardPreview();
	void DecrementLeftCardPreview();
	void IncrementOperandCardPreview();
	void DecrementOperandCardPreview();
	void IncrementRightCardPreview();
	void DecrementRightCardPreview();

	void ResetCardConstruction();
	void UpdateCardAndButtonRects();
	void DetermineIfValidEquation();
	void CheckForSelectionButtonClicks();

public:
	DevScene(SceneManager& manager);
	~DevScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

