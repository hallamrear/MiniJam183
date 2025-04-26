#pragma once
#include <deque>
#include <Gameplay/Cards/OperandCard.h>
#include <Gameplay/Cards/NumberCard.h>

class Card;

class Deck
{
private:
	std::deque<OperandCard> m_HeldOperands;
	std::deque<OperandCard> m_DiscardedOperands;
	std::deque<NumberCard> m_HeldNumbers;
	std::deque<NumberCard> m_DiscaredNumbers;

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
};

