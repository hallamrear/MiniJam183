#pragma once
#include "Card.h"
#include "OperandCard.h"

/// <summary>
/// A large number card is any card value > 10
/// </summary>
constexpr const int c_LargeNumberCardCount = 5;
constexpr const int c_BasicNumberCardMaxValue = 10;

class NumberCard : public Card
{
private:
	int m_CombinedValueA;
	int m_CombinedValueB;
	OperandCard m_CombinedOp;
	bool m_IsCombinedCard;

	int m_Value;

public:
	NumberCard(const int& valueA, const int& valueB, const OPERAND_TYPE& Op);
	NumberCard(const int& value);
	//Generates random basic number card.
	NumberCard();
	~NumberCard();

	const int& GetValue() const;
	const bool& GetIsCombined() const;
};

