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
	std::deque<NumberCard> m_DiscaredNumbers;

	void LoadTextures();
	void DestroyTextures();

public:
	Deck();
	~Deck();

	void DrawNumbersHand(const int& count, std::vector<NumberCard*>& handToFill);
	void DrawOperandCardsIntoHand(const int& count, std::vector<OperandCard*>& handToFill);

	void ShuffleNumbersCards();
	void ShuffleOperandCards();

	void ClearDeck();
	void ResetDeck();
	void RestoreDiscardedOperands();
	void RestoreDiscardedNumbers();
	void RestoreDiscardedCards();

	SDL_Texture& GetNumberCardTexture(const NUMBER_CARD_VALUE& value) const;
	SDL_Texture& GetOperandCardTexture(const OPERAND_TYPE& type) const;
};

