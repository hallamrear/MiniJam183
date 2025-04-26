#include "pch.h"
#include "Card.h"

Card::Card(const CARD_TYPE& type)
{
	m_CardType = type;
}

Card::~Card()
{

}

const CARD_TYPE Card::GetCardType() const
{
	return m_CardType;
}
