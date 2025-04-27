#pragma once
#include "Card.h"

enum NUMBER_CARD_VALUE
{
	ONE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4,
	FIVE = 5,
	SIX = 6,
	SEVEN = 7,
	EIGHT = 8,
	NINE = 9,
	TEN = 10,
	TWELVE = 12,
	FIFTEEN = 15,
	EIGHTEEN = 18,
	TWENTY = 20,
	FIFTY = 50,
};

class NumberCard : public Card
{
private:
	NUMBER_CARD_VALUE m_Value;

public:
	NumberCard(const NUMBER_CARD_VALUE& value);
	~NumberCard();

	const NUMBER_CARD_VALUE& GetValue() const;
	const int& GetIntValue() const;
};

