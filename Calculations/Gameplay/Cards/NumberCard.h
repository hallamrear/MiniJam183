#pragma once
#include "Card.h"

class NumberCard : public Card
{
private:
	int m_Value;

public:
	NumberCard(const int& value);
	~NumberCard();

	const int& GetValue() const;
};

