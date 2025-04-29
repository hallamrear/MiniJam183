#pragma once

enum CARD_TYPE
{
	OPERAND_CARD,
	NUMBER_CARD
};

constexpr const float c_CardWidth = 128.0f;
constexpr const float c_CardHeight = 183.0f;
constexpr const float c_CardDrawSizeRatio = c_CardHeight / c_CardWidth;

class Card
{
protected:
	CARD_TYPE m_CardType;
	Card(const CARD_TYPE& type);

public:

	virtual ~Card() = 0;
	const CARD_TYPE GetCardType() const;
};

