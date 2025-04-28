#include "pch.h"
#include "ShopScene.h"
#include <System/Services.h>
#include <System/Collision.h>
#include <Gameplay/Player/Player.h>
#include <System/SceneManager.h>

ShopScene::ShopScene(SceneManager& manager) : Scene(manager), m_Player(Services::GetPlayer())
{
	m_ExitBattleButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_IncreaseNumbersHandButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_IncreaseOperandsHandButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_ShopCentreRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };

	for (size_t i = 0; i < c_ShopSlotCountTotal; i++)
	{
		m_ShopSlotRects[i] = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	m_WindowWidth = 0;
	m_WindowHeight = 0;
	m_WindowCentreX = 0;
	m_WindowCentreY = 0;
	m_IncreaseHandCost = 0;
	m_MouseX = 0;
	m_MouseY = 0;
	m_IsLeftClickDown = false;
	m_CanClickButtons = true;
}

ShopScene::~ShopScene()
{
}

void ShopScene::OnEnter()
{
	RegenerateShop();
}

void ShopScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_EVENT_MOUSE_MOTION:
	{
		m_MouseX = e.button.x;
		m_MouseY = e.button.y;
	}
	break;

	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	{
		m_IsLeftClickDown = true;
	}
	break;

	case SDL_EVENT_MOUSE_BUTTON_UP:
	{
		m_IsLeftClickDown = false;
		m_CanClickButtons = true;
	}
	break;

	default:
		break;
	}
}

void ShopScene::OnExit()
{

}

void ShopScene::RegenerateShop()
{
	m_IncreaseHandCost = 3;
}

void ShopScene::Update(const float& deltaTime)
{
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
	float mouseX = 0;
	float mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (m_CanClickButtons && m_IsLeftClickDown)
	{
		if (Collision::PointInRect((int)mouseX, (int)mouseY, m_IncreaseNumbersHandButtonRect))
		{
			if (m_Player.GetGoldCount() - m_IncreaseHandCost > 0)
			{
				m_Player.DecreaseGold(m_IncreaseHandCost);
				m_Player.IncreaseNumbersHandSize(1);
				m_IncreaseHandCost += 3;
			}

			m_CanClickButtons = false;
		}
		else if (Collision::PointInRect((int)mouseX, (int)mouseY, m_IncreaseOperandsHandButtonRect))
		{
			if (m_Player.GetGoldCount() - m_IncreaseHandCost > 0)
			{
				m_Player.DecreaseGold(m_IncreaseHandCost);
				m_Player.IncreaseOperandsHandSize(1);
				m_IncreaseHandCost += 3;
			}

			m_CanClickButtons = false;
		}
		else if (Collision::PointInRect((int)mouseX, (int)mouseY, m_ExitBattleButtonRect))
		{
			m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_BATTLE);
		}
	}
}

void ShopScene::CalculateShopRects()
{
	m_ShopCentreRect.w = (m_WindowWidth / 3) * 2;
	m_ShopCentreRect.h = (m_WindowHeight / 3) * 2;
	m_ShopCentreRect.x = m_WindowCentreX - (m_ShopCentreRect.w / 2);
	m_ShopCentreRect.y = m_WindowCentreY - (m_ShopCentreRect.h / 2);

	const float slotWidth = m_ShopCentreRect.w / c_ShopSlotCountX;
	const float slotHeight = m_ShopCentreRect.h / c_ShopSlotCountY;

	for (size_t i = 0; i < c_ShopSlotCountX; i++)
	{
		for (size_t j = 0; j < c_ShopSlotCountY; j++)
		{
			m_ShopSlotRects[(j * c_ShopSlotCountX) + i].w = slotWidth;
			m_ShopSlotRects[(j * c_ShopSlotCountX) + i].h = slotHeight;
			m_ShopSlotRects[(j * c_ShopSlotCountX) + i].x = m_ShopCentreRect.x + (i * slotWidth);
			m_ShopSlotRects[(j * c_ShopSlotCountX) + i].y = m_ShopCentreRect.y + (j * slotHeight);
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

	for (size_t i = 0; i < c_ShopSlotCountTotal; i++)
	{
		SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 0.0f, 0.0f, 1.0f);
		SDL_RenderRect(&renderer, &m_ShopSlotRects[i]);
	}

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderRect(&renderer, &m_IncreaseOperandsHandButtonRect);
	str = "Increase Operand Hand - Cost: " + std::to_string(m_IncreaseHandCost);
	SDL_RenderDebugText(&renderer, m_IncreaseOperandsHandButtonRect.x + 10, m_IncreaseOperandsHandButtonRect.y + 10, str.c_str());

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderRect(&renderer, &m_IncreaseNumbersHandButtonRect);
	str = "Increase Numbers Hand - Cost: " + std::to_string(m_IncreaseHandCost);
	SDL_RenderDebugText(&renderer, m_IncreaseNumbersHandButtonRect.x + 10, m_IncreaseNumbersHandButtonRect.y + 10, str.c_str());

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderRect(&renderer, &m_ShopCentreRect);

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderRect(&renderer, &m_ExitBattleButtonRect);
	SDL_RenderDebugText(&renderer, m_ExitBattleButtonRect.x + 10, m_ExitBattleButtonRect.y + 10, "TO NEXT BATTLE");

	SDL_RenderDebugText(&renderer, 64, 64, "SHOP SCENE");
}
