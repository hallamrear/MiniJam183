#include "pch.h"
#include "Deck.h"
#include <Graphics/Texture.h>
#include <Graphics/Text.h>
#include <System/FontRenderer.h>
#include <System/Services.h>
#include <SDL3_ttf/SDL_ttf.h>

void Deck::LoadTextures()
{
	DestroyTextures();

	FontRenderer& fr = Services::GetFontRenderer();

	for (int i = 0; i < 10; i++)
	{
		std::string numStr = std::to_string(i);
		TTF_Text* text = nullptr;

		if (Text::LoadText(numStr.c_str(), text))
		{
			m_NumberTextures.insert({ i, text });
		}
	}

	Texture::LoadPNG("Content\\Cards\\OperandSheet.png", m_OperandTexture);
	Texture::LoadPNG("Content\\Cards\\Blank_Card.png", m_BlankCardTexture);
}

void Deck::DestroyTextures()
{
	if (m_OperandTexture != nullptr)
	{
		SDL_DestroyTexture(m_OperandTexture);
		m_OperandTexture = nullptr;
	}

	for (auto& itr : m_NumberTextures)
	{
		if (itr.second != nullptr)
		{
			TTF_DestroyText(itr.second);
			itr.second = nullptr;
		}
	}
	m_NumberTextures.clear();
}

Deck::Deck()
{
	m_HeldNumbers = std::deque<NumberCard>();
	m_DiscardedNumbers = std::deque<NumberCard>();
	m_HeldOperands = std::deque<OperandCard>();
	m_DiscardedOperands = std::deque<OperandCard>();
	m_NumberTextures = std::unordered_map<int, TTF_Text*>();

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

		throw;
	}
	break;

	case NUMBER_CARD:
	{
		NumberCard* numCard = dynamic_cast<NumberCard*>(card);

		if (numCard)
		{
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

		throw;
	}
		break;
	default:
		throw;
		break;
	}

	throw;
}

TTF_Text& Deck::GetNumberTextTexture(const NumberCard& numCard)
{
	auto found = m_NumberTextures.find(numCard.GetValue());

	if (found != m_NumberTextures.end())
	{
		return *found->second;
	}

	FontRenderer& fr = Services::GetFontRenderer();
	std::string numStr = std::to_string(numCard.GetValue());
	TTF_Text* text = nullptr;
	
	
	if (Text::LoadText(numStr.c_str(), text))
	{
		m_NumberTextures.insert({ numCard.GetValue(), text});
		return *text;
	}
}

SDL_Texture& Deck::GetOperandCardTexture() const
{
	assert(m_OperandTexture);
	return *m_OperandTexture;
}

SDL_Texture& Deck::GetBlankCardTexture() const
{
	assert(m_BlankCardTexture);
	return *m_BlankCardTexture;
}

