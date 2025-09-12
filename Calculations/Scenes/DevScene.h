#pragma once
#include <Scenes/Scene.h>
#include <Gameplay/Cards/NumberCard.h>
#include <Gameplay/Cards/OperandCard.h>
#include <Gameplay/Cards/Deck.h>

class DevScene :
    public Scene
{
private:
	TTF_Text* m_Text;

	NumberCard m_CardInConstruction;

	std::vector<Card*> m_DeckCards;
	std::vector<OperandCard*> m_OperandCards;
	std::vector<NumberCard*> m_LeftNumberCardSet;
	std::vector<NumberCard*> m_RightNumberCardSet;

	int m_RightCardPreviewIndex;
	int m_LeftCardPreviewIndex;

	void ResetConstructedCard();
	void UpdateCardArrangement();

public:
	DevScene(SceneManager& manager);
	~DevScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

