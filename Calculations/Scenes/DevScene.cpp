#include "pch.h"
#include "DevScene.h"
#include <Gameplay/Player/Player.h>
#include <Graphics/Text.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <System/Input.h>
#include <System/Services.h>
#include <System/Collision.h>

DevScene::DevScene(SceneManager& manager) : Scene(manager), m_Window(Services::GetWindow()), m_ButtonSpacing(8.0f)
{
	m_DeckCards = std::vector<Card*>();
	m_OperandCardSet = std::vector<OperandCard*>();
	m_LeftNumberCardSet = std::vector<NumberCard*>();
	m_RightNumberCardSet = std::vector<NumberCard*>();
	m_LeftCardPreviewIndex = 0;
	m_RightCardPreviewIndex = 1;
	m_OperandCardPreviewIndex = 0;
	m_IsValidEquation = false;
	m_SelectedLeftCard = nullptr;
	m_SelectedOperandCard = nullptr;
	m_SelectedRightCard = nullptr; 

	m_TestPalettes =
	{
		/* Monitor Glow White */ { 0.94117647058823530f, 0.96470588235294120f, 0.94117647058823530f, 1.0f },
		/* Monitor Glow Black */ { 0.13333333333333333f, 0.13725490196078433f, 0.13725490196078433f, 1.0f },
		/* Oil 6 Palette */ { 0.98431372549019600f, 0.96078431372549020f, 0.96078431372549020f, 1.0f },
		/* Oil 6 Palette */ { 0.94901960784313720f, 0.82745098039215680f, 0.67058823529411760f, 1.0f },
		/* Oil 6 Palette */ { 0.77647058823529410f, 0.62352941176470590f, 0.64705882352941180f, 1.0f },
		/* Oil 6 Palette */ { 0.54509803921568620f, 0.42745098039215684f, 0.61176470588235300f, 1.0f },
		/* Oil 6 Palette */ { 0.28627450980392155f, 0.30196078431372547f, 0.49411764705882355f, 1.0f },
		/* Oil 6 Palette */ { 0.15294117647058825f, 0.15294117647058825f, 0.26666666666666666f, 1.0f }
	};

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
	m_OperandCardSet.clear();
	m_LeftNumberCardSet.clear();
}

void DevScene::OnEnter()
{
	UpdateCardAndButtonRects();
}

void DevScene::UpdateCardAndButtonRects()
{
	int w = 0, h = 0;
	SDL_GetWindowSizeInPixels(&m_Window, &w, &h);
	int centre_x = w / 2;
	int centre_y = h / 2;

	const int cardWidth = (w / 8);
	const int cardHeight = (w / 8) * 1.8f;
	const int halfCardWidth = cardWidth / 2;
	const int halfCardHeight = cardHeight / 2;
	const int buttonWidth = (halfCardWidth / 2) - m_ButtonSpacing;

	m_LeftNumberCardDrawRect.x = ((w / 4) * 1) - halfCardWidth;
	m_LeftNumberCardDrawRect.y = (centre_y)-halfCardHeight;
	m_LeftNumberCardDrawRect.w = cardWidth;
	m_LeftNumberCardDrawRect.h = cardHeight;
	m_LeftNumberCardSelectionBackwardButton.w = (float)buttonWidth;
	m_LeftNumberCardSelectionBackwardButton.h = (float)buttonWidth / 2;
	m_LeftNumberCardSelectionBackwardButton.x = m_LeftNumberCardDrawRect.x;
	m_LeftNumberCardSelectionBackwardButton.y = m_LeftNumberCardDrawRect.y + halfCardHeight + m_ButtonSpacing;
	m_LeftNumberCardSelectionForwardButton.w = (float)buttonWidth;
	m_LeftNumberCardSelectionForwardButton.h = (float)buttonWidth / 2;
	m_LeftNumberCardSelectionForwardButton.x = (m_LeftNumberCardDrawRect.x + m_LeftNumberCardDrawRect.w) - m_LeftNumberCardSelectionForwardButton.w;
	m_LeftNumberCardSelectionForwardButton.y = m_LeftNumberCardDrawRect.y + m_LeftNumberCardDrawRect.h + m_ButtonSpacing;


	m_RightNumberCardDrawRect.w = cardWidth;
	m_RightNumberCardDrawRect.h = cardHeight;
	m_RightNumberCardDrawRect.x = ((w / 4) * 2) - halfCardWidth;
	m_RightNumberCardDrawRect.y = (centre_y)-halfCardHeight;
	m_RightNumberCardSelectionBackwardButton.w = (float)buttonWidth;
	m_RightNumberCardSelectionBackwardButton.h = (float)buttonWidth / 2;
	m_RightNumberCardSelectionBackwardButton.x = m_RightNumberCardDrawRect.x;
	m_RightNumberCardSelectionBackwardButton.y = m_RightNumberCardDrawRect.y + halfCardHeight + m_ButtonSpacing;
	m_RightNumberCardSelectionForwardButton.w = (float)buttonWidth;
	m_RightNumberCardSelectionForwardButton.h = (float)buttonWidth / 2;
	m_RightNumberCardSelectionForwardButton.x = (m_RightNumberCardDrawRect.x + m_RightNumberCardDrawRect.w) - m_RightNumberCardSelectionForwardButton.w;
	m_RightNumberCardSelectionForwardButton.y = m_RightNumberCardDrawRect.y + m_RightNumberCardDrawRect.h + m_ButtonSpacing;

	m_ConstructedCardDrawRect.x = ((w / 4) * 3) - halfCardWidth;
	m_ConstructedCardDrawRect.y = centre_y - halfCardHeight;
	m_ConstructedCardDrawRect.w = cardWidth;
	m_ConstructedCardDrawRect.h = cardHeight;

	m_OperandCardDrawRect.x = ((m_LeftNumberCardDrawRect.x + m_RightNumberCardDrawRect.x) / 2.0f) + (halfCardWidth / 2);
	m_OperandCardDrawRect.y = ((m_LeftNumberCardDrawRect.y + m_RightNumberCardDrawRect.y) / 2.0f) + (halfCardHeight / 2);
	m_OperandCardDrawRect.w = halfCardWidth;
	m_OperandCardDrawRect.h = halfCardHeight;

	m_OperandSelectionBackwardButton.w = (float)buttonWidth;
	m_OperandSelectionBackwardButton.h = (float)buttonWidth / 2;
	m_OperandSelectionBackwardButton.x = m_OperandCardDrawRect.x;
	m_OperandSelectionBackwardButton.y = m_OperandCardDrawRect.y + halfCardHeight + m_ButtonSpacing;
	m_OperandSelectionForwardButton.w = (float)buttonWidth;
	m_OperandSelectionForwardButton.h = (float)buttonWidth / 2;
	m_OperandSelectionForwardButton.x = (m_OperandCardDrawRect.x + m_OperandCardDrawRect.w) - m_OperandSelectionForwardButton.w;
	m_OperandSelectionForwardButton.y = m_OperandCardDrawRect.y + m_OperandCardDrawRect.h + m_ButtonSpacing;

	m_EqualsSignDrawRect.x = ((m_ConstructedCardDrawRect.x + m_RightNumberCardDrawRect.x) / 2.0f) + (halfCardWidth / 2);
	m_EqualsSignDrawRect.y = (centre_y - (halfCardWidth / 2));
	m_EqualsSignDrawRect.w = halfCardWidth;
	m_EqualsSignDrawRect.h = halfCardWidth;

	if (m_IsValidEquation)
	{
		m_ConstructNewCardButtonRect.w = 128.0f;
		m_ConstructNewCardButtonRect.h = 64.0f;
		m_ConstructNewCardButtonRect.x = w - m_ConstructNewCardButtonRect.w - m_ButtonSpacing;
		m_ConstructNewCardButtonRect.y = h - m_ConstructNewCardButtonRect.h - m_ButtonSpacing;
	}
	else
	{
		m_ConstructNewCardButtonRect.w = 0.0f;
		m_ConstructNewCardButtonRect.h = 0.0f;
		m_ConstructNewCardButtonRect.x = -1.0f;
		m_ConstructNewCardButtonRect.y = -1.0f;
	}
}

void DevScene::DetermineIfValidEquation()
{
	m_SelectedLeftCard = m_LeftNumberCardSet[m_LeftCardPreviewIndex];
	m_SelectedOperandCard = m_OperandCardSet[m_OperandCardPreviewIndex];
	m_SelectedRightCard = m_RightNumberCardSet[m_RightCardPreviewIndex];

	if (m_SelectedLeftCard == nullptr || m_SelectedOperandCard == nullptr || m_SelectedRightCard == nullptr)
	{
		m_IsValidEquation = false;
	}
	else
	{
		m_IsValidEquation = true;
		m_CardInConstruction = NumberCard(m_SelectedLeftCard->GetValue(), m_SelectedRightCard->GetValue(), m_SelectedOperandCard->GetOperand());
	}
}

void DevScene::CheckForSelectionButtonClicks()
{
	bool mouseDown = m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON);
	int x = m_InputManager.GetMouseX();
	int y = m_InputManager.GetMouseY();

	if (Collision::PointInRect(x, y, m_LeftNumberCardSelectionForwardButton) && mouseDown)
	{
		IncrementLeftCardPreview();
		m_InputManager.ConsumeButtonPress(Input::MOUSE_BUTTON::LEFT_BUTTON);
	}

	if (Collision::PointInRect(x, y, m_LeftNumberCardSelectionBackwardButton) && mouseDown)
	{
		DecrementLeftCardPreview();
		m_InputManager.ConsumeButtonPress(Input::MOUSE_BUTTON::LEFT_BUTTON);
	}

	if (Collision::PointInRect(x, y, m_OperandSelectionForwardButton) && mouseDown)
	{
		IncrementOperandCardPreview();
		m_InputManager.ConsumeButtonPress(Input::MOUSE_BUTTON::LEFT_BUTTON);
	}

	if (Collision::PointInRect(x, y, m_OperandSelectionBackwardButton) && mouseDown)
	{
		DecrementOperandCardPreview();
		m_InputManager.ConsumeButtonPress(Input::MOUSE_BUTTON::LEFT_BUTTON);
	}

	if (Collision::PointInRect(x, y, m_RightNumberCardSelectionForwardButton) && mouseDown)
	{
		IncrementRightCardPreview();
		m_InputManager.ConsumeButtonPress(Input::MOUSE_BUTTON::LEFT_BUTTON);
	}

	if (Collision::PointInRect(x, y, m_RightNumberCardSelectionBackwardButton) && mouseDown)
	{
		DecrementRightCardPreview();
		m_InputManager.ConsumeButtonPress(Input::MOUSE_BUTTON::LEFT_BUTTON);
	}

	if (Collision::PointInRect(x, y, m_ConstructNewCardButtonRect) && mouseDown)
	{
		m_Player.GetDeck().AddCard(m_CardInConstruction);
		m_Player.GetDeck().RemoveSpecificCard(m_LeftNumberCardSet[m_LeftCardPreviewIndex]);
		m_Player.GetDeck().RemoveSpecificCard(m_OperandCardSet[m_OperandCardPreviewIndex]);
		m_Player.GetDeck().RemoveSpecificCard(m_RightNumberCardSet[m_RightCardPreviewIndex]);
		m_InputManager.ConsumeButtonPress(Input::MOUSE_BUTTON::LEFT_BUTTON);

		m_OperandCardPreviewIndex = 0;
		m_LeftCardPreviewIndex = 0;
		m_RightCardPreviewIndex = 0;
		IncrementRightCardPreview();

		UpdateCardAndButtonRects();

		printf("LEFT : Size %i - Index %i\n", m_LeftNumberCardSet.size(), m_LeftCardPreviewIndex);
		printf("RIGHT : Size %i - Index %i\n", m_RightNumberCardSet.size(), m_RightCardPreviewIndex);
	}
}

void DevScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
	case SDL_EVENT_WINDOW_RESIZED:
	{
		UpdateCardAndButtonRects();
	}
	break;

	case SDL_EVENT_KEY_DOWN:
	{
		switch (e.key.key)
		{
		case SDLK_0: { ResetCardConstruction(); }break;

		case SDLK_1: { IncrementLeftCardPreview(); }break;
		case SDLK_2: { DecrementLeftCardPreview(); }break;

		case SDLK_3: { IncrementOperandCardPreview(); }break;
		case SDLK_4: { DecrementOperandCardPreview(); }break;

		case SDLK_5: { IncrementRightCardPreview(); }break;
		case SDLK_6: { DecrementRightCardPreview(); }break;
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
	m_OperandCardSet.clear();
	m_LeftNumberCardSet.clear();

	m_Player.GetDeck().GetAllCardsInDeck(m_DeckCards);
	m_Player.GetDeck().GetNumbersCardsInDeck(m_LeftNumberCardSet);
	m_RightNumberCardSet = m_LeftNumberCardSet;
	m_Player.GetDeck().GetOperandCardsInDeck(m_OperandCardSet);

	UpdateCardAndButtonRects();
	CheckForSelectionButtonClicks();
	DetermineIfValidEquation();
}

void DevScene::Render(SDL_Renderer& renderer) const
{
	int w = 0, h = 0;
	SDL_GetWindowSizeInPixels(&m_Window, &w, &h);

	SDL_FRect rect;
	rect.y = h - 64.0f;
	rect.w = 64.0f; rect.h = 64.0f;
	for (size_t i = 0; i < m_TestPalettes.size(); i++)
	{
		rect.x = 64.0f * i;
		SDL_SetRenderDrawColorFloat(&renderer, m_TestPalettes[i].r, m_TestPalettes[i].g, m_TestPalettes[i].b, m_TestPalettes[i].a);
		SDL_RenderFillRect(&renderer, &rect);
	}

	std::string str = "UNDEFINED";

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 0.0f, 0.0f, 1.0f);
	 
	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);

	SDL_RenderRect(&renderer, &m_LeftNumberCardSelectionForwardButton);
	SDL_RenderRect(&renderer, &m_LeftNumberCardSelectionBackwardButton);
	SDL_RenderRect(&renderer, &m_LeftNumberCardDrawRect);
	if (m_SelectedLeftCard != nullptr)
	{
		str = std::to_string(m_SelectedLeftCard->GetValue());
		SDL_RenderDebugText(&renderer, m_LeftNumberCardDrawRect.x + (m_LeftNumberCardDrawRect.w / 2), m_LeftNumberCardDrawRect.y + (m_LeftNumberCardDrawRect.h / 2), str.c_str());
	}

	SDL_RenderRect(&renderer, &m_OperandSelectionForwardButton);
	SDL_RenderRect(&renderer, &m_OperandSelectionBackwardButton);
	SDL_RenderRect(&renderer, &m_OperandCardDrawRect);
	if (m_OperandCardSet.size() > 0)
	{
		OperandCard* opCard = dynamic_cast<OperandCard*>(m_OperandCardSet[m_OperandCardPreviewIndex]);

		str = "no op card selected";
		if (opCard)
		{
			str = opCard->GetOperandCharacter();
		}

		SDL_RenderDebugText(&renderer, m_OperandCardDrawRect.x + (m_OperandCardDrawRect.w / 2), m_OperandCardDrawRect.y +  (m_OperandCardDrawRect.h / 2), str.c_str());
	}

	SDL_RenderRect(&renderer, &m_RightNumberCardSelectionForwardButton);
	SDL_RenderRect(&renderer, &m_RightNumberCardSelectionBackwardButton);
	SDL_RenderRect(&renderer, &m_RightNumberCardDrawRect);
	if (m_SelectedRightCard != nullptr)
	{
		str = std::to_string(m_SelectedRightCard->GetValue());
		SDL_RenderDebugText(&renderer, m_RightNumberCardDrawRect.x + (m_RightNumberCardDrawRect.w / 2), m_RightNumberCardDrawRect.y + (m_RightNumberCardDrawRect.h / 2), str.c_str());
	}

	SDL_RenderRect(&renderer, &m_EqualsSignDrawRect);
	str = "=";
	SDL_RenderDebugText(&renderer, m_EqualsSignDrawRect.x + (m_EqualsSignDrawRect.w / 2), m_EqualsSignDrawRect.y + (m_EqualsSignDrawRect.h / 2), str.c_str());

	SDL_RenderRect(&renderer, &m_ConstructedCardDrawRect); 
	str = std::to_string(m_CardInConstruction.GetValue());
	SDL_RenderDebugText(&renderer, m_ConstructedCardDrawRect.x + (m_ConstructedCardDrawRect.w / 2), m_ConstructedCardDrawRect.y + (m_ConstructedCardDrawRect.h / 2), str.c_str());

	if (m_Text)
	{
		TTF_DrawRendererText(m_Text, 10, 10);
	}

	if (m_IsValidEquation)
	{
		SDL_RenderRect(&renderer, &m_ConstructNewCardButtonRect);
		SDL_RenderDebugText(&renderer, m_ConstructNewCardButtonRect.x, m_ConstructNewCardButtonRect.y + (m_ConstructNewCardButtonRect.h / 2), "CONSTRUCT CARD");
	}
}

void DevScene::IncrementLeftCardPreview()
{
	m_LeftCardPreviewIndex++;
	m_LeftCardPreviewIndex = m_LeftCardPreviewIndex % m_LeftNumberCardSet.size();

	if (m_LeftCardPreviewIndex == m_RightCardPreviewIndex)
	{
		m_LeftCardPreviewIndex++;
		m_LeftCardPreviewIndex = m_LeftCardPreviewIndex % m_LeftNumberCardSet.size();
	}

	printf("LEFT : Size %i - Index %i\n", m_LeftNumberCardSet.size(), m_LeftCardPreviewIndex);
	printf("RIGHT : Size %i - Index %i\n", m_RightNumberCardSet.size(), m_RightCardPreviewIndex);
}

void DevScene::DecrementLeftCardPreview()
{
	m_LeftCardPreviewIndex--;

	if (m_LeftCardPreviewIndex < 0)
	{
		m_LeftCardPreviewIndex += m_LeftNumberCardSet.size();
	}

	if (m_LeftCardPreviewIndex == m_RightCardPreviewIndex)
	{
		m_LeftCardPreviewIndex--;
	}

	if (m_LeftCardPreviewIndex < 0)
	{
		m_LeftCardPreviewIndex += m_LeftNumberCardSet.size();
	}

	printf("LEFT : Size %i - Index %i\n", m_LeftNumberCardSet.size(), m_LeftCardPreviewIndex);
	printf("RIGHT : Size %i - Index %i\n", m_RightNumberCardSet.size(), m_RightCardPreviewIndex);
}

void DevScene::IncrementOperandCardPreview()
{
	m_OperandCardPreviewIndex++;
	m_OperandCardPreviewIndex = m_OperandCardPreviewIndex % m_OperandCardSet.size();
}

void DevScene::DecrementOperandCardPreview()
{
	m_OperandCardPreviewIndex--;

	if (m_OperandCardPreviewIndex < 0)
	{
		m_OperandCardPreviewIndex += m_OperandCardSet.size();
	}
}

void DevScene::IncrementRightCardPreview()
{
	m_RightCardPreviewIndex++;
	m_RightCardPreviewIndex = m_RightCardPreviewIndex % m_RightNumberCardSet.size();

	if (m_RightCardPreviewIndex == m_LeftCardPreviewIndex)
	{
		m_RightCardPreviewIndex++;
		m_RightCardPreviewIndex = m_RightCardPreviewIndex % m_RightNumberCardSet.size();
	}

	printf("LEFT : Size %i - Index %i\n", m_LeftNumberCardSet.size(), m_LeftCardPreviewIndex);
	printf("RIGHT : Size %i - Index %i\n", m_RightNumberCardSet.size(), m_RightCardPreviewIndex);
}

void DevScene::DecrementRightCardPreview()
{
	m_RightCardPreviewIndex--;

	if (m_RightCardPreviewIndex < 0)
	{
		m_RightCardPreviewIndex += m_RightNumberCardSet.size();
	}

	if (m_RightCardPreviewIndex == m_LeftCardPreviewIndex)
	{
		m_RightCardPreviewIndex--;
	}

	if (m_RightCardPreviewIndex < 0)
	{
		m_RightCardPreviewIndex += m_RightNumberCardSet.size();
	}

	printf("LEFT : Size %i - Index %i\n", m_LeftNumberCardSet.size(), m_LeftCardPreviewIndex);
	printf("RIGHT : Size %i - Index %i\n", m_RightNumberCardSet.size(), m_RightCardPreviewIndex);
}

void DevScene::ResetCardConstruction()
{
	m_CardInConstruction = NumberCard(INT_MAX);
	m_SelectedLeftCard = nullptr;
	m_SelectedOperandCard = nullptr;
	m_SelectedRightCard = nullptr;
	DetermineIfValidEquation();
}