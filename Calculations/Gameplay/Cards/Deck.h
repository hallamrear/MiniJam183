#pragma once
#include <deque>
#include <Gameplay/Cards/OperandCard.h>
#include <Gameplay/Cards/NumberCard.h>
#include <unordered_map>

class Card;
struct TTF_Text;

class Deck
{
private:
	std::unordered_map<int, TTF_Text*> m_NumberTextures;
	SDL_Texture* m_OperandTexture;
	SDL_Texture* m_BlankCardTexture;

	std::deque<OperandCard> m_HeldOperands;
	std::deque<OperandCard> m_DiscardedOperands;
	std::deque<NumberCard> m_HeldNumbers;
	std::deque<NumberCard> m_DiscardedNumbers;

	void LoadTextures();
	void DestroyTextures();

public:
	Deck();
	~Deck();

	void GetAllCardsInDeck(std::vector<Card*>& numbersCards);
	void GetNumbersCardsInDeck(std::vector<NumberCard*>& numbersCards);
	void GetOperandCardsInDeck(std::vector<OperandCard*>& numbersCards);

	void DrawNumbersHand(const int& count, std::vector<NumberCard*>& handToFill);
	void DrawOperandCardsIntoHand(const int& count, std::vector<OperandCard*>& handToFill);

	void ShuffleNumbersCards();
	void ShuffleOperandCards();

	void AddCard(const NumberCard& card);
	void AddCard(const OperandCard& card);

	void ClearDeck();
	void ResetDeck();
	void RestoreDiscardedOperands();
	void RestoreDiscardedNumbers();
	void RestoreDiscardedCards();

	void RemoveSpecificCard(Card* card);

	TTF_Text& GetNumberTextTexture(const NumberCard& numCard);
	SDL_Texture& GetOperandCardTexture() const;
	SDL_Texture& GetBlankCardTexture() const;
};

