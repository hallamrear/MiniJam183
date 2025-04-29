#include "pch.h"
#include "NumberCard.h"

NumberCard::NumberCard() : Card(CARD_TYPE::NUMBER_CARD)
{
	m_Value = NUMBER_CARD_VALUE::ONE;
}

NumberCard::NumberCard(const NUMBER_CARD_VALUE& value) : Card(CARD_TYPE::NUMBER_CARD)
{
	m_Value = value;
}

NumberCard::~NumberCard()
{

}

const NUMBER_CARD_VALUE& NumberCard::GetValue() const
{
	return m_Value;
}

const int& NumberCard::GetIntValue() const
{
	return (int)m_Value;
}
