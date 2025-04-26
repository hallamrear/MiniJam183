#include "pch.h"
#include "NumberCard.h"

NumberCard::NumberCard(const int& value) : Card(CARD_TYPE::NUMBER_CARD)
{
	m_Value = value;
}

NumberCard::~NumberCard()
{

}

const int& NumberCard::GetValue() const
{
	return m_Value;
}
