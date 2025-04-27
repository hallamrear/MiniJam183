#pragma once
#include <Gameplay/Entity.h>
#include <Gameplay/Cards/Deck.h>

class Player : public Entity
{
private:
	Deck m_Deck;
	std::vector<OperandCard*> m_OperandHand;
	std::vector<NumberCard*> m_NumbersHand;

public:
	Player();
	~Player();

	std::vector<OperandCard*>& GetOperandHand();
	std::vector<NumberCard*>& GetNumbersHand();

	void EmptyOperandHand();
	void EmptyNumberHand();
	void EmptyHands();
	void DrawNumberCardsIntoHand(const int& count);
	void DrawOperandCardsIntoHand(const int& count);
	Deck& GetDeck();
};

