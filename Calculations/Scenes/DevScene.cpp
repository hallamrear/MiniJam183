#include "pch.h"
#include "DevScene.h"
#include <Gameplay/Player/Player.h>
#include <Graphics/Text.h>

int count = 0;

DevScene::DevScene(SceneManager& manager) : Scene(manager)
{
	m_DeckCards = std::vector<Card*>();
	m_OperandCards = std::vector<OperandCard*>();
	m_LeftNumberCardSet = std::vector<NumberCard*>();
	m_RightNumberCardSet = std::vector<NumberCard*>();
	m_RightCardPreviewIndex = 0;
	m_LeftCardPreviewIndex = 0;

	m_Text = nullptr;
	Text::LoadText("Undefined", m_Text);
}

DevScene::~DevScene()
{
	if (m_Text != nullptr)
	{
		Text::Destroy(m_Text);
		m_Text = nullptr;
	}

	m_DeckCards.clear();
	m_OperandCards.clear();
	m_LeftNumberCardSet.clear();
}

void DevScene::OnEnter()
{
	UpdateCardArrangement();
}

void DevScene::UpdateCardArrangement()
{
	std::string str = "UNDEFINED";

	if (m_DeckCards.size() <= 0)
	{
		count = 0;
	}
	else
	{
		if (count < 0)
		{
			count += m_DeckCards.size();
		}

		count = count % m_DeckCards.size();

		if (m_DeckCards[count]->GetCardType() == CARD_TYPE::NUMBER_CARD)
		{
			NumberCard* nc = dynamic_cast<NumberCard*>(m_DeckCards[count]);
			if (nc)
			{
				str = "Number card : " + std::to_string(nc->GetValue());
			}
		}
		else //Operand
		{
			OperandCard* oc = dynamic_cast<OperandCard*>(m_DeckCards[count]);
			if (oc)
			{
				str = "Operand card : ";
				str += oc->GetOperandCharacter();
			}
		}

	}

	str = std::to_string(count) + " -> " + str;

	Text::SetText(str.c_str(), m_Text);
}

void DevScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_EVENT_KEY_DOWN:
	{
		switch (e.key.key)
		{
		case SDLK_0: { ResetConstructedCard(); }break;
		
		case SDLK_1: { count++; UpdateCardArrangement(); }	break;
		case SDLK_2: { count--; UpdateCardArrangement(); }	break;

		}
	}
	break;

	default:
		break;
	}
}

void DevScene::OnExit()
{
	m_DeckCards.clear();

	if (m_Text != nullptr)
	{
		Text::Destroy(m_Text);
		m_Text = nullptr;
	}
}

void DevScene::Update(const float& deltaTime)
{
	m_DeckCards.clear();
	m_OperandCards.clear();
	m_LeftNumberCardSet.clear();

	m_Player.GetDeck().GetAllCardsInDeck(m_DeckCards);
	m_Player.GetDeck().GetNumbersCardsInDeck(m_LeftNumberCardSet);
	m_Player.GetDeck().GetOperandCardsInDeck(m_OperandCards);

	UpdateCardArrangement();

	if (m_DeckCards.size() > 0)
	{
		int row = 0;

		for (size_t i = 0; i < m_DeckCards.size(); i++)
		{
			//m_CardRects.push_back(SDL_FRect{ 64.0f, 64.0f, 64.0f, 64.0f });
		}
	}
}

void DevScene::Render(SDL_Renderer& renderer) const
{
	if (m_Text)
	{
		TTF_DrawRendererText(m_Text, 10, 10);
	}
}

void DevScene::ResetConstructedCard()
{
	m_CardInConstruction = NumberCard(INT_MAX);
}