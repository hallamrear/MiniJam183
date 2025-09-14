#pragma once
#include <deque>
#include <Gameplay/Cards/OperandCard.h>
#include <Gameplay/Cards/NumberCard.h>
#include <unordered_map>

class Card;

class Deck
{
private:
	std::unordered_map<int, SDL_Texture*> m_NumberCardTextures;
	std::vector<SDL_Texture*> m_OperandCardTextures;

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

	SDL_Texture& GetNumberCardTexture(const NumberCard& value) const;
	SDL_Texture& GetOperandCardTexture(const OperandCard& type) const;
};

