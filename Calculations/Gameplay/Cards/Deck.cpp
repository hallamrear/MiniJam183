#include "pch.h"
#include "Deck.h"
#include <Graphics/Texture.h>

void Deck::LoadTextures()
{
	DestroyTextures();
	
	m_NumberCardTextures.insert(std::make_pair(1, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\1.png", m_NumberCardTextures[1]);

	m_NumberCardTextures.insert(std::make_pair(2, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\2.png", m_NumberCardTextures[2]);

	m_NumberCardTextures.insert(std::make_pair(3, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\3.png", m_NumberCardTextures[3]);

	m_NumberCardTextures.insert(std::make_pair(4, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\4.png", m_NumberCardTextures[4]);

	m_NumberCardTextures.insert(std::make_pair(5, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\5.png", m_NumberCardTextures[5]);

	m_NumberCardTextures.insert(std::make_pair(6, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\6.png", m_NumberCardTextures[6]);

	m_NumberCardTextures.insert(std::make_pair(7, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\7.png", m_NumberCardTextures[7]);

	m_NumberCardTextures.insert(std::make_pair(8, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\8.png", m_NumberCardTextures[8]);

	m_NumberCardTextures.insert(std::make_pair(9, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\9.png", m_NumberCardTextures[9]);

	m_NumberCardTextures.insert(std::make_pair(10, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\10.png", m_NumberCardTextures[10]);

	m_NumberCardTextures.insert(std::make_pair(12, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\12.png", m_NumberCardTextures[12]);

	m_NumberCardTextures.insert(std::make_pair(15, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\15.png", m_NumberCardTextures[15]);

	m_NumberCardTextures.insert(std::make_pair(18, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\18.png", m_NumberCardTextures[18]);

	m_NumberCardTextures.insert(std::make_pair(20, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\20.png", m_NumberCardTextures[20]);

	m_NumberCardTextures.insert(std::make_pair(50, nullptr));
	Texture::LoadPNG("Content\\Cards\\Numbers\\50.png", m_NumberCardTextures[50]);

	m_OperandCardTextures.resize(4, nullptr);
	Texture::LoadPNG("Content\\Cards\\Operands\\Addition.png", m_OperandCardTextures[OPERAND_TYPE::ADDITION]);
	Texture::LoadPNG("Content\\Cards\\Operands\\Subtraction.png", m_OperandCardTextures[OPERAND_TYPE::SUBTRACTION]);
	Texture::LoadPNG("Content\\Cards\\Operands\\Multiplication.png", m_OperandCardTextures[OPERAND_TYPE::MULTIPLICATION]);
	Texture::LoadPNG("Content\\Cards\\Operands\\Division.png", m_OperandCardTextures[OPERAND_TYPE::DIVISION]);
}

void Deck::DestroyTextures()
{
	for (auto& itr : m_NumberCardTextures)
	{
		if (itr.second != nullptr)
		{
			SDL_DestroyTexture(itr.second);
			itr.second = nullptr;
		}
	}
	m_NumberCardTextures.clear();

	for (size_t i = 0; i < m_OperandCardTextures.size(); i++)
	{
		if (m_OperandCardTextures[i] != nullptr)
		{
			SDL_DestroyTexture(m_OperandCardTextures[i]);
			m_OperandCardTextures[i] = nullptr;
		}
	}
	m_OperandCardTextures.clear();
}

Deck::Deck()
{
	m_HeldNumbers = std::deque<NumberCard>();
	m_DiscardedNumbers = std::deque<NumberCard>();
	m_HeldOperands = std::deque<OperandCard>();
	m_DiscardedOperands = std::deque<OperandCard>();
	m_NumberCardTextures = std::unordered_map<int, SDL_Texture*>();
	m_OperandCardTextures = std::vector<SDL_Texture*>();

	LoadTextures();
}

Deck::~Deck()
{
	ClearDeck();
	DestroyTextures();
}

void Deck::GetAllCardsInDeck(std::vector<Card*>& vectorToFill)
{
	vectorToFill.clear();

	for (auto& itr : m_HeldNumbers)
	{
		vectorToFill.push_back(&itr);
	}

	for (auto& itr : m_DiscardedNumbers)
	{
		vectorToFill.push_back(&itr);
	}

	for (auto& itr : m_HeldOperands)
	{
		vectorToFill.push_back(&itr);
	}

	for (auto& itr : m_DiscardedOperands)
	{
		vectorToFill.push_back(&itr);
	}
}

void Deck::GetNumbersCardsInDeck(std::vector<NumberCard*>& numbersCards)
{
	numbersCards.clear();

	for (auto& itr : m_HeldNumbers)
	{
		numbersCards.push_back(&itr);
	}

	for (auto& itr : m_DiscardedNumbers)
	{
		numbersCards.push_back(&itr);
	}
}

void Deck::GetOperandCardsInDeck(std::vector<OperandCard*>& operandCards)
{
	operandCards.clear();

	for (auto& itr : m_HeldOperands)
	{
		operandCards.push_back(&itr);
	}

	for (auto& itr : m_DiscardedOperands)
	{
		operandCards.push_back(&itr);
	}
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
		m_DiscardedNumbers.push_front(m_HeldNumbers.front());
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

void Deck::AddCard(const NumberCard& card)
{
	m_HeldNumbers.push_front(card);
}

void Deck::AddCard(const OperandCard& card)
{
	m_HeldOperands.push_front(card);
}

void Deck::ShuffleNumbersCards()
{
	unsigned long now = time(nullptr);
	std::shuffle(m_HeldNumbers.begin(), m_HeldNumbers.end(), std::default_random_engine(now));
}

void Deck::ClearDeck()
{
	//Put all discard cards into use.
	RestoreDiscardedCards();
	
	//Clear everything.
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
	ShuffleOperandCards();

	/*
	m_HeldNumbers.push_front(NumberCard(4));
	m_HeldNumbers.push_front(NumberCard(4));
	m_HeldNumbers.push_front(NumberCard(3));
	m_HeldNumbers.push_front(NumberCard(3));
	m_HeldNumbers.push_front(NumberCard(2));
	m_HeldNumbers.push_front(NumberCard(2));
	m_HeldNumbers.push_front(NumberCard(1));
	m_HeldNumbers.push_front(NumberCard(1));
	*/

	m_HeldNumbers.push_front(NumberCard(4));
	m_HeldNumbers.push_front(NumberCard(3));
	m_HeldNumbers.push_front(NumberCard(2));
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
	while (!m_DiscardedNumbers.empty())
	{
		m_HeldNumbers.push_front(m_DiscardedNumbers.front());
		m_DiscardedNumbers.pop_front();
	}
	ShuffleNumbersCards();
}

void Deck::RestoreDiscardedCards()
{
	RestoreDiscardedNumbers();
	RestoreDiscardedOperands();	
}

void Deck::RemoveSpecificCard(Card* card)
{
	if (card == nullptr)
		return;

	CARD_TYPE type = card->GetCardType();

	switch (type)
	{
	case OPERAND_CARD:
	{
		OperandCard* opCard = dynamic_cast<OperandCard*>(card);

		if (opCard)
		{
			for (size_t i = 0; i < m_HeldOperands.size(); i++)
			{
				if (&m_HeldOperands[i] == opCard)
				{
					m_HeldOperands.erase(m_HeldOperands.begin() + i);
					return;
				}
			}

			for (size_t i = 0; i < m_DiscardedOperands.size(); i++)
			{
				if (&m_DiscardedOperands[i] == opCard)
				{
					m_DiscardedOperands.erase(m_DiscardedOperands.begin() + i);
					return;
				}
			}
		}
	}
	break;

	case NUMBER_CARD:
	{
		NumberCard* numCard = dynamic_cast<NumberCard*>(card);

		for (size_t i = 0; i < m_HeldNumbers.size(); i++)
		{
			if (&m_HeldNumbers[i] == numCard)
			{
				m_HeldNumbers.erase(m_HeldNumbers.begin() + i);
				return;
			}
		}

		for (size_t i = 0; i < m_DiscardedNumbers.size(); i++)
		{
			if (&m_DiscardedNumbers[i] == numCard)
			{
				m_DiscardedNumbers.erase(m_DiscardedNumbers.begin() + i);
				return;
			}
		}
	}
		break;
	default:
		throw;
		break;
	}

	throw;
}

SDL_Texture& Deck::GetNumberCardTexture(const NumberCard& numCard) const
{
	int index = numCard.GetValue();
	assert(index > 0);
	auto found = m_NumberCardTextures.find(numCard.GetValue());
	assert(found != m_NumberCardTextures.end());
	return *found->second;
}

SDL_Texture& Deck::GetOperandCardTexture(const OperandCard& opCard) const
{
	int index = (int)opCard.GetOperand();
	assert(index > 0 || index < m_OperandCardTextures.size());
	return *m_OperandCardTextures[index];
}

