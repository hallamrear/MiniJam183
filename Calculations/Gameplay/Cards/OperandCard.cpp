#include "pch.h"
#include "OperandCard.h"

OperandCard::OperandCard() : Card(CARD_TYPE::OPERAND_CARD)
{
	m_OperandType = OPERAND_TYPE::ADDITION;
}

OperandCard::OperandCard(const OPERAND_TYPE& type) : Card(CARD_TYPE::OPERAND_CARD)
{
	m_OperandType = type;
}

OperandCard::~OperandCard()
{

}

const OPERAND_TYPE& OperandCard::GetOperand() const
{
	return m_OperandType;
}