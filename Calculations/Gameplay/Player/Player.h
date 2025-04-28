#pragma once
#include <Gameplay/Entity.h>
#include <Gameplay/Cards/Deck.h>

class Player : public Entity
{
private:
	Deck m_Deck;
	std::vector<OperandCard*> m_OperandHand;
	std::vector<NumberCard*> m_NumbersHand;

	int m_NumbersHandSize;
	int m_OperandHandSize;

	void RemoveOperandCardFromHand(const OperandCard* card);
	void RemoveNumbersCardFromHand(const NumberCard* card);

	int m_GoldCount;
	int m_WinCount;

public:
	Player();
	~Player();

	std::vector<OperandCard*>& GetOperandHand();
	std::vector<NumberCard*>& GetNumbersHand();

	void HardReset();

	const int GetOperandHandSize() const;
	const int GetNumbersHandSize() const;

	void IncreaseWinCount(const int& count);
	const int& GetWinCount() const;

	void IncreaseNumbersHandSize(const int& count);
	void IncreaseOperandsHandSize(const int& count);
	const int& GetGoldCount() const; 
	const void IncreaseGold(const int& count);
	const void DecreaseGold(const int& count);

	void EmptyOperandHand();
	void EmptyNumberHand();
	void EmptyHands();
	void DrawNumberCardsIntoHand(const int& count);
	void DrawOperandCardsIntoHand(const int& count);
	void RemoveCardFromHand(const Card* card);
	Deck& GetDeck();
};

