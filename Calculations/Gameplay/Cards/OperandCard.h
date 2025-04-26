#pragma once
#include <Gameplay/Cards/Card.h>

enum OPERAND_TYPE
{
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION
};

class OperandCard : public Card
{
private:
	OPERAND_TYPE m_OperandType;

public:
	OperandCard(const OPERAND_TYPE& type);
	~OperandCard();

	const OPERAND_TYPE& GetOperand() const;
};

