#include "pch.h"
#include "ShopScene.h"
#include <System/Services.h>
#include <System/Collision.h>
#include <Gameplay/Player/Player.h>
#include <System/SceneManager.h>
#include <System/Input.h>
#include <Graphics/Texture.h>

constexpr const int c_LargeCardRollChance = 25;
constexpr const int c_NumberCardRollChance = 70;

ShopScene::ShopScene(SceneManager& manager) : Scene(manager), m_Player(Services::GetPlayer())
{
	m_ExitBattleButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_IncreaseNumbersHandButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_IncreaseOperandsHandButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_ShopCentreRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };

	for (size_t i = 0; i < c_ShopSlotCountTotal; i++)
	{
		m_ShopSlotRects[i] = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
		m_ShopItems[i] = ShopItem();
	}

	m_WindowWidth = 0;
	m_WindowHeight = 0;
	m_WindowCentreX = 0;
	m_WindowCentreY = 0;
	m_IncreaseNumbersHandCost = 0;
	m_IncreaseOperandHandCost = 0;
	m_CanClickButtons = false;

	m_CardBoughtTexture = nullptr;
	Texture::LoadPNG("Content/Shop/PurchasedCardOverlay.png", m_CardBoughtTexture);
}

ShopScene::~ShopScene()
{
	if (m_CardBoughtTexture != nullptr)
	{
		SDL_DestroyTexture(m_CardBoughtTexture);
		m_CardBoughtTexture = nullptr;
	}
}

void ShopScene::OnEnter()
{
	RegenerateShop();
}

void ShopScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
		default:
			break;
	}
}

void ShopScene::OnExit()
{

}

void ShopScene::RegenerateShop()
{
	m_CanClickButtons = false;
	m_IncreaseNumbersHandCost = m_Player.GetNumbersHandSize();
	m_IncreaseOperandHandCost = m_Player.GetOperandHandSize();

	for (size_t i = 0; i < c_ShopSlotCountTotal; i++)
	{
		m_ShopItems[i].Reset();
		m_ShopItems[i].GenerateRandomItem();
	}
}

void ShopScene::Update(const float& deltaTime)
{
	if (m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON) == false)
	{
		m_CanClickButtons = true;
	}

	SDL_Window& window = Services::GetWindow();
	SDL_GetWindowSize(&window, &m_WindowWidth, &m_WindowHeight);
	m_WindowCentreX = m_WindowWidth / 2;
	m_WindowCentreY = m_WindowHeight / 2;

	CalculateShopRects();
	CalculateButtonRects();
	CheckButtonClicks();
}

void ShopScene::CheckButtonClicks()
{
	if (m_CanClickButtons && m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON))
	{
		for (size_t i = 0; i < c_ShopSlotCountTotal; i++)
		{
			if (m_ShopItems[i].HasBeenPurchased() == false)
			{
				if (Collision::PointInRect(m_InputManager.GetMouseX(), m_InputManager.GetMouseY(), m_ShopSlotRects[i]))
				{
					int cost = 5;
					if (m_ShopItems[i].IsNumberCard())
					{
						cost = m_ShopItems[i].GetGeneratedNumberCard().GetIntValue();
					}

					if (m_Player.GetGoldCount() - cost >= 0)
					{
						m_ShopItems[i].SetPurchased(true);
						m_Player.DecreaseGold(cost);
						if (m_ShopItems[i].IsNumberCard())
						{
							m_Player.GetDeck().AddCard(m_ShopItems[i].GetGeneratedNumberCard());
						}
						else
						{
							m_Player.GetDeck().AddCard(m_ShopItems[i].GetGeneratedOperandCard());
						}
					}
				}
			}	
		}

		if (Collision::PointInRect(m_InputManager.GetMouseX(), m_InputManager.GetMouseY(), m_IncreaseNumbersHandButtonRect))
		{
			if (m_Player.GetGoldCount() - m_IncreaseNumbersHandCost >= 0)
			{
				m_Player.DecreaseGold(m_IncreaseNumbersHandCost);
				m_Player.IncreaseNumbersHandSize(1);
				m_IncreaseNumbersHandCost = m_Player.GetNumbersHandSize();
			}

			m_CanClickButtons = false;
		}
		else if (Collision::PointInRect(m_InputManager.GetMouseX(), m_InputManager.GetMouseY(), m_IncreaseOperandsHandButtonRect))
		{
			if (m_Player.GetGoldCount() - m_IncreaseOperandHandCost >= 0)
			{
				m_Player.DecreaseGold(m_IncreaseOperandHandCost);
				m_Player.IncreaseOperandsHandSize(1);
				m_IncreaseOperandHandCost = m_Player.GetOperandHandSize();
			}

			m_CanClickButtons = false;
		}
		else if (Collision::PointInRect(m_InputManager.GetMouseX(), m_InputManager.GetMouseY(), m_ExitBattleButtonRect))
		{
			m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_BATTLE);
		}
	}
}

void ShopScene::CalculateShopRects()
{
	const float windowScaling = m_WindowWidth / TARGET_RESOLUTION_X;
	const float c_ShopSlotSpacingX = 16.0f;
	const float c_ShopSlotSpacingY = 16.0f;

	m_ShopCentreRect.w = (m_WindowWidth / 3) * 2;
	m_ShopCentreRect.h = (m_WindowHeight / 3) * 2;
	m_ShopCentreRect.x = m_WindowCentreX - (m_ShopCentreRect.w / 2);
	m_ShopCentreRect.y = m_WindowCentreY - (m_ShopCentreRect.h / 2);

	const float fullSlotWidth =  (m_ShopCentreRect.w - (2 * c_ShopSlotSpacingX)) / c_ShopSlotCountX;
	const float fullSlotHeight = (m_ShopCentreRect.h - (2 * c_ShopSlotSpacingY)) / c_ShopSlotCountY;
	const float fullSlotRatio = fullSlotHeight / fullSlotWidth;

	for (size_t i = 0; i < c_ShopSlotCountX; i++)
	{
		for (size_t j = 0; j < c_ShopSlotCountY; j++)
		{
			const float fullSlotPosX = m_ShopCentreRect.x + (i * fullSlotWidth) + (i * c_ShopSlotSpacingX);
			const float fullSlotPosY = m_ShopCentreRect.y + (j * fullSlotHeight) + (j * c_ShopSlotSpacingY);
			const float scaledCardWidth = ((fullSlotWidth / 3) * 2) * fullSlotRatio;
			const float scaledCardHeight = scaledCardWidth * c_CardDrawSizeRatio;

			m_ShopSlotRects[(j * c_ShopSlotCountX) + i].w = scaledCardWidth;
			m_ShopSlotRects[(j * c_ShopSlotCountX) + i].h = scaledCardHeight;
			m_ShopSlotRects[(j * c_ShopSlotCountX) + i].x = fullSlotPosX + (scaledCardWidth) - c_ShopSlotSpacingX;
			m_ShopSlotRects[(j * c_ShopSlotCountX) + i].y = fullSlotPosY;
		}
	}
}

void ShopScene::CalculateButtonRects()
{
	const float buttonSizeRatio = c_ExitButtonHeight / c_ExitButtonWidth;
	const float windowScaling = m_WindowWidth / TARGET_RESOLUTION_X;
	const float scaledWidth = c_ExitButtonWidth * windowScaling;
	const float scaledHeight = scaledWidth * buttonSizeRatio;

	const float buttonWidth = (m_ShopCentreRect.w / 2) - (2 * c_ButtonPadding);
	const float buttonHeight = (m_WindowHeight / 3) * 0.33;

	m_IncreaseNumbersHandButtonRect.w = buttonWidth;
	m_IncreaseNumbersHandButtonRect.h = buttonHeight;
	m_IncreaseNumbersHandButtonRect.x = m_ShopCentreRect.x + c_ButtonPadding;
	m_IncreaseNumbersHandButtonRect.y = m_ShopCentreRect.y + m_ShopCentreRect.h + c_ButtonPadding;

	m_IncreaseOperandsHandButtonRect.w = buttonWidth;
	m_IncreaseOperandsHandButtonRect.h = buttonHeight;
	m_IncreaseOperandsHandButtonRect.x = m_IncreaseNumbersHandButtonRect.x + buttonWidth + (2 * c_ButtonPadding);
	m_IncreaseOperandsHandButtonRect.y = m_IncreaseNumbersHandButtonRect.y;

	m_ExitBattleButtonRect.x = m_WindowWidth - c_ButtonPadding - scaledWidth;
	m_ExitBattleButtonRect.y = m_WindowHeight - c_ButtonPadding - scaledHeight;
	m_ExitBattleButtonRect.w = scaledWidth;
	m_ExitBattleButtonRect.h = scaledHeight;
}

void ShopScene::Render(SDL_Renderer& renderer) const
{
	std::string str = "";

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	#ifdef _DEBUG
		SDL_RenderDebugText(&renderer, 10, 10, "SCENE_IDENTIFIER::SHOP_SCENE");
	#endif

	for (size_t i = 0; i < c_ShopSlotCountTotal; i++)
	{
		SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
		SDL_RenderRect(&renderer, &m_ShopSlotRects[i]);

		if (m_ShopItems[i].IsNumberCard())
		{
			SDL_RenderTexture(&renderer, &m_Player.GetDeck().GetNumberCardTexture(m_ShopItems[i].GetGeneratedNumberCard().GetValue()), nullptr, &m_ShopSlotRects[i]);
		}
		else
		{
			SDL_RenderTexture(&renderer, &m_Player.GetDeck().GetOperandCardTexture(m_ShopItems[i].GetGeneratedOperandCard().GetOperand()), nullptr, &m_ShopSlotRects[i]);
		}

		if (m_ShopItems[i].HasBeenPurchased())
		{
			SDL_RenderTexture(&renderer, m_CardBoughtTexture, nullptr, &m_ShopSlotRects[i]);
		}
	}

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderRect(&renderer, &m_IncreaseOperandsHandButtonRect);
	str = "Increase Operand Hand - Cost: " + std::to_string(m_IncreaseOperandHandCost);
	int y_pos = m_IncreaseOperandsHandButtonRect.y - m_IncreaseOperandsHandButtonRect.h / 2 + 4;
	SDL_RenderDebugText(&renderer, m_IncreaseOperandsHandButtonRect.x + 10, m_IncreaseOperandsHandButtonRect.y + 10, str.c_str());

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderRect(&renderer, &m_IncreaseNumbersHandButtonRect);
	str = "Increase Numbers Hand - Cost: " + std::to_string(m_IncreaseNumbersHandCost);
	SDL_RenderDebugText(&renderer, m_IncreaseNumbersHandButtonRect.x + 10, m_IncreaseNumbersHandButtonRect.y + 10, str.c_str());

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderRect(&renderer, &m_ShopCentreRect);

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderRect(&renderer, &m_ExitBattleButtonRect);
	SDL_RenderDebugText(&renderer, m_ExitBattleButtonRect.x + 10, m_ExitBattleButtonRect.y + 10, "TO NEXT BATTLE");


	SDL_RenderDebugTextFormat(&renderer, 10, 20, "Player Wins: %i", m_Player.GetWinCount());
	SDL_RenderDebugTextFormat(&renderer, 10, 30, "Player Gold: %i", m_Player.GetGoldCount());
	SDL_RenderDebugTextFormat(&renderer, 10, 40, "Operands cost 5 gold each and Numbers cost their respective value.");
}

ShopItem::ShopItem()
{
	m_HasBeenPurchased = false;
	m_UseNumberCard = true;
	m_OperandCard = OperandCard(OPERAND_TYPE::ADDITION);
	m_NumberCard = NumberCard(NUMBER_CARD_VALUE::ONE);
}

ShopItem::~ShopItem()
{

}

void ShopItem::SetIsNumberCard(const bool& state)
{
	m_UseNumberCard = state;
}

const bool& ShopItem::IsNumberCard() const
{
	return m_UseNumberCard;
}

const bool& ShopItem::HasBeenPurchased() const
{
	return m_HasBeenPurchased;
}

void ShopItem::SetPurchased(const bool& state)
{
	m_HasBeenPurchased = state;
}

void ShopItem::GenerateRandomItem()
{
	OPERAND_TYPE opType = (OPERAND_TYPE)(rand() % 4);
	m_OperandCard = OperandCard(opType);

	bool isLargeNumberCard = (rand() % 100) > (100 - c_LargeCardRollChance);

	NUMBER_CARD_VALUE numValue = NUMBER_CARD_VALUE::ONE;

	if (isLargeNumberCard)
	{
		int index = rand() % c_LargeNumberCardCount;

		switch (index)
		{
		case 0: { numValue = NUMBER_CARD_VALUE::TWELVE; } break;
		case 1: { numValue = NUMBER_CARD_VALUE::FIFTEEN; } break;
		case 2: { numValue = NUMBER_CARD_VALUE::EIGHTEEN; } break;
		case 3: { numValue = NUMBER_CARD_VALUE::TWENTY; } break;
		case 4: { numValue = NUMBER_CARD_VALUE::FIFTY; } break;

		default:
			break;
		}
	}
	else
	{
		numValue = NUMBER_CARD_VALUE(rand() % 10 + 1);
	}

	m_NumberCard = NumberCard(numValue);

	bool isNumberCard = (rand() % 100) < c_NumberCardRollChance;
	SetIsNumberCard(isNumberCard);
}

void ShopItem::Reset()
{
	SetPurchased(false);
	SetIsNumberCard(true);
}

const OperandCard& ShopItem::GetGeneratedOperandCard() const
{
	return m_OperandCard;
}

const NumberCard& ShopItem::GetGeneratedNumberCard() const
{
	return m_NumberCard;
}
