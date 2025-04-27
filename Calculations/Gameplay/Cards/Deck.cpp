#include "pch.h"
#include "Deck.h"
#include <Graphics/Texture.h>

void Deck::LoadTextures()
{
	DestroyTextures();
	
	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::ONE, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/1.png", m_NumberCardTextures[NUMBER_CARD_VALUE::ONE]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::TWO, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/2.png", m_NumberCardTextures[NUMBER_CARD_VALUE::TWO]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::THREE, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/3.png", m_NumberCardTextures[NUMBER_CARD_VALUE::THREE]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::FOUR, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/4.png", m_NumberCardTextures[NUMBER_CARD_VALUE::FOUR]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::FIVE, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/5.png", m_NumberCardTextures[NUMBER_CARD_VALUE::FIVE]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::SIX, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/6.png", m_NumberCardTextures[NUMBER_CARD_VALUE::SIX]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::SEVEN, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/7.png", m_NumberCardTextures[NUMBER_CARD_VALUE::SEVEN]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::EIGHT, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/8.png", m_NumberCardTextures[NUMBER_CARD_VALUE::EIGHT]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::NINE, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/9.png", m_NumberCardTextures[NUMBER_CARD_VALUE::NINE]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::TEN, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/10.png", m_NumberCardTextures[NUMBER_CARD_VALUE::TEN]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::TWELVE, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/12.png", m_NumberCardTextures[NUMBER_CARD_VALUE::TWELVE]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::FIFTEEN, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/15.png", m_NumberCardTextures[NUMBER_CARD_VALUE::FIFTEEN]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::EIGHTEEN, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/18.png", m_NumberCardTextures[NUMBER_CARD_VALUE::EIGHTEEN]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::TWENTY, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/20.png", m_NumberCardTextures[NUMBER_CARD_VALUE::TWENTY]);

	m_NumberCardTextures.insert(std::make_pair(NUMBER_CARD_VALUE::FIFTY, nullptr));
	Texture::LoadPNG("Content/Cards/Numbers/50.png", m_NumberCardTextures[NUMBER_CARD_VALUE::FIFTY]);



	m_OperandCardTextures.resize(4, nullptr);
	Texture::LoadPNG("Content/Cards/Operands/Addition.png", m_OperandCardTextures[OPERAND_TYPE::ADDITION]);
	Texture::LoadPNG("Content/Cards/Operands/Subtraction.png", m_OperandCardTextures[OPERAND_TYPE::SUBTRACTION]);
	Texture::LoadPNG("Content/Cards/Operands/Multiplication.png", m_OperandCardTextures[OPERAND_TYPE::MULTIPLICATION]);
	Texture::LoadPNG("Content/Cards/Operands/Division.png", m_OperandCardTextures[OPERAND_TYPE::DIVISION]);
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
	m_DiscaredNumbers = std::deque<NumberCard>();
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

	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)20));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)15));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)10));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)10));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)9));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)8));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)7));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)6));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)5));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)5));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)4));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)4));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)3));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)3));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)2));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)2));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)2));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)1));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)1));
	m_HeldNumbers.push_front(NumberCard((NUMBER_CARD_VALUE)1));
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


SDL_Texture& Deck::GetNumberCardTexture(const NUMBER_CARD_VALUE& value) const
{
	int index = (int)value;
	assert(index > 0 || index < m_NumberCardTextures.size());
	return *m_NumberCardTextures.at(index);
}

SDL_Texture& Deck::GetOperandCardTexture(const OPERAND_TYPE& type) const
{
	int index = (int)type;
	assert(index > 0 || index < m_OperandCardTextures.size());
	return *m_OperandCardTextures[index];
}

