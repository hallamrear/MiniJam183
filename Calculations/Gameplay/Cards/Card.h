#pragma once

enum CARD_TYPE
{
	OPERAND_CARD,
	NUMBER_CARD
};

class Card
{
protected:
	CARD_TYPE m_CardType;
	Card(const CARD_TYPE& type);

public:

	virtual ~Card() = 0;
	const CARD_TYPE GetCardType() const;
};

