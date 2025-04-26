#include "pch.h"
#include "Deck.h"

Deck::Deck()
{
	m_HeldNumbers = std::deque<NumberCard>();
	m_DiscaredNumbers = std::deque<NumberCard>();
	m_HeldOperands = std::deque<OperandCard>();
	m_DiscardedOperands = std::deque<OperandCard>();
}

Deck::~Deck()
{
	ClearDeck();
}

void Deck::DrawNumbersHand(const int& count, std::vector<NumberCard*>& handToFill)
{
	//Loop through I times
	for (size_t i = 0; i < count; i++)
	{
		//Check if we can pull a card, if not, restore cards.
		if (m_HeldNumbers.empty())
		{
			RestoreDiscardedNumbers();
		}

		//Add front card to hand.
		handToFill.push_back(&m_HeldNumbers.front());
		//Discard the same card.
		m_DiscaredNumbers.push_front(m_HeldNumbers.front());
		//Remove from front of held cards.
		m_HeldNumbers.pop_front();
	}
}

void Deck::DrawOperandCardsIntoHand(const int& count, std::vector<OperandCard*>& handToFill)
{
	//Loop through I times
	for (size_t i = 0; i < count; i++)
	{
		//Check if we can pull a card, if not, restore cards.
		if (m_HeldOperands.empty())
		{
			RestoreDiscardedOperands();
		}

		//Add front card to hand.
		handToFill.push_back(&m_HeldOperands.front());
		//Discard the same card.
		m_DiscardedOperands.push_front(m_HeldOperands.front());
		//Remove from front of held cards.
		m_HeldOperands.pop_front();
	}
}

void Deck::ShuffleOperandCards()
{
	unsigned long now = time(nullptr);
	std::shuffle(m_HeldOperands.begin(), m_HeldOperands.end(), std::default_random_engine(now));
}

void Deck::ShuffleNumbersCards()
{
	unsigned long now = time(nullptr);
	std::shuffle(m_HeldNumbers.begin(), m_HeldNumbers.end(), std::default_random_engine(now));
}

void Deck::ClearDeck()
{
	//Clear existing cards.
	RestoreDiscardedCards();
	
	while (!m_HeldOperands.empty())
	{
		m_HeldOperands.pop_front();
	}

	while (!m_HeldNumbers.empty())
	{
		m_HeldNumbers.pop_front();
	}
}

void Deck::ResetDeck()
{
	ClearDeck();

	m_HeldOperands.push_front(OperandCard(OPERAND_TYPE::ADDITION));
	m_HeldOperands.push_front(OperandCard(OPERAND_TYPE::ADDITION));
	m_HeldOperands.push_front(OperandCard(OPERAND_TYPE::SUBTRACTION));
	m_HeldOperands.push_front(OperandCard(OPERAND_TYPE::SUBTRACTION));
	m_HeldOperands.push_front(OperandCard(OPERAND_TYPE::MULTIPLICATION));
	m_HeldOperands.push_front(OperandCard(OPERAND_TYPE::MULTIPLICATION));
	m_HeldOperands.push_front(OperandCard(OPERAND_TYPE::DIVISION));
	m_HeldOperands.push_front(OperandCard(OPERAND_TYPE::DIVISION));
	ShuffleOperandCards();

	m_HeldNumbers.push_front(NumberCard(20));
	m_HeldNumbers.push_front(NumberCard(15));
	m_HeldNumbers.push_front(NumberCard(10));
	m_HeldNumbers.push_front(NumberCard(10));
	m_HeldNumbers.push_front(NumberCard(9));
	m_HeldNumbers.push_front(NumberCard(8));
	m_HeldNumbers.push_front(NumberCard(7));
	m_HeldNumbers.push_front(NumberCard(6));
	m_HeldNumbers.push_front(NumberCard(5));
	m_HeldNumbers.push_front(NumberCard(5));
	m_HeldNumbers.push_front(NumberCard(4));
	m_HeldNumbers.push_front(NumberCard(4));
	m_HeldNumbers.push_front(NumberCard(3));
	m_HeldNumbers.push_front(NumberCard(3));
	m_HeldNumbers.push_front(NumberCard(2));
	m_HeldNumbers.push_front(NumberCard(2));
	m_HeldNumbers.push_front(NumberCard(2));
	m_HeldNumbers.push_front(NumberCard(1));
	m_HeldNumbers.push_front(NumberCard(1));
	m_HeldNumbers.push_front(NumberCard(1));
	ShuffleNumbersCards();
}

void Deck::RestoreDiscardedOperands()
{
	while (!m_DiscardedOperands.empty())
	{
		m_HeldOperands.push_front(m_DiscardedOperands.front());
		m_DiscardedOperands.pop_front();
	}
	ShuffleOperandCards();
}

void Deck::RestoreDiscardedNumbers()
{
	while (!m_DiscaredNumbers.empty())
	{
		m_HeldNumbers.push_front(m_DiscaredNumbers.front());
		m_DiscaredNumbers.pop_front();
	}
	ShuffleNumbersCards();
}

void Deck::RestoreDiscardedCards()
{
	RestoreDiscardedNumbers();
	RestoreDiscardedOperands();	
}

