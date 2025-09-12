#include "pch.h"
#include "NumberCard.h"

NumberCard::NumberCard(const int& valueA, const int& valueB, const OPERAND_TYPE& Op) : Card(CARD_TYPE::NUMBER_CARD)
{
	m_CombinedValueA = valueA;
	m_CombinedValueB = valueB;
	m_CombinedOp = Op;
	m_IsCombinedCard = true;

	switch (Op)
	{
	case OPERAND_TYPE::ADDITION: { m_Value = (valueA + valueB); }; break;
	case OPERAND_TYPE::SUBTRACTION: { m_Value = (valueA - valueB); }; break;

	case OPERAND_TYPE::MULTIPLICATION:
	case OPERAND_TYPE::DIVISION:
	{
		if (valueA == 0 || valueB == 0)
		{
			m_Value = 0;
		}
		else
		{
			if (Op == MULTIPLICATION)
			{
				m_Value = (valueA * valueB);
			}
			else
			{
				m_Value = (valueA / valueB);
			}
		}
	};
	break;

	default:
		break;
	}
}

NumberCard::NumberCard(const int& value) : Card(CARD_TYPE::NUMBER_CARD)
{
	m_CombinedValueA = INT_MAX;
	m_CombinedValueB = INT_MAX;
	m_CombinedOp = OPERAND_TYPE::ADDITION;
	m_IsCombinedCard = false;
	m_Value = value;
}

NumberCard::NumberCard() : Card(CARD_TYPE::NUMBER_CARD)
{
	m_CombinedValueA = INT_MAX;
	m_CombinedValueB = INT_MAX;
	m_CombinedOp = OPERAND_TYPE::ADDITION;
	m_IsCombinedCard = false;
	m_Value = rand() % c_BasicNumberCardMaxValue + 1;
}

NumberCard::~NumberCard()
{

}

const int& NumberCard::GetValue() const
{
	return m_Value;
}

const bool& NumberCard::GetIsCombined() const
{
	return m_IsCombinedCard;
}
