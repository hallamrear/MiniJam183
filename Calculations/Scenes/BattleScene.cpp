#include "pch.h"
#include "BattleScene.h"
#include <Graphics/Texture.h>
#include <System/Services.h>
#include <Gameplay/Enemies/Ghost.h>

constexpr const float c_AvatarDrawSize = 256.0f;
constexpr const float c_NumbersCardDrawSize = 128.0f;
constexpr const float c_OperandCardDrawSize = c_NumbersCardDrawSize / 2.0f;
constexpr const float c_GapBetweenNumbersCards = 32.0f;
constexpr const float c_GapBetweenOperandsCards = c_GapBetweenNumbersCards / 2.0f;
constexpr const float c_HealthBarGapFromAvatar = 16.0f;

BattleScene::BattleScene() : Scene(), m_Player(Services::GetPlayer())
{
	m_Enemy = nullptr;

	SDL_Window& window = Services::GetWindow();
	m_WindowCenterX = 0;
	m_WindowCenterY = 0;
	m_WindowWidth = 0;
	m_WindowHeight = 0;

	m_CharacterDrawRect = SDL_FRect{ 0.0f };
	m_EnemyDrawRect = SDL_FRect{ 0.0f };

	m_PlayerHealthBar = new ProgressBar(SDL_FRect{ (INITIAL_WINDOW_WIDTH / 4) * 1 - 100.0f, (INITIAL_WINDOW_HEIGHT / 4) * 3, 200.0f, 20.0f });
	m_EnemyHealthBar  = new ProgressBar(SDL_FRect{ (INITIAL_WINDOW_WIDTH / 4) * 3 - 100.0f, (INITIAL_WINDOW_HEIGHT / 4) * 3, 200.0f, 20.0f });
}

BattleScene::~BattleScene()
{

}

void BattleScene::OnEnter()
{
	SetupNewBattle();

	if (m_PlayerHealthBar != nullptr)
	{
		float value = m_Player.GetCurrentHealth() / m_Player.GetMaxHealth();
		m_PlayerHealthBar->SetProgressValue(value);
	}

	if (m_EnemyHealthBar != nullptr && m_Enemy != nullptr)
	{
		float value = m_Enemy->GetCurrentHealth() / m_Enemy->GetMaxHealth();
		m_EnemyHealthBar->SetProgressValue(value);
	}
}

void BattleScene::HandleEvent(const SDL_Event& e)
{
	static int x = 5;
	static int y = 5;

	switch (e.type)
	{
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	{

	}
	break;



	case SDL_EVENT_KEY_DOWN:
	{
		switch (e.key.key)
		{

		case SDLK_G:
		{
			y--;
			m_Player.EmptyOperandHand();
			m_Player.DrawOperandCardsIntoHand(y);
		}
		break;

		case SDLK_H:
		{
			y++;
			m_Player.EmptyOperandHand();
			m_Player.DrawOperandCardsIntoHand(y);
		}
		break;

		case SDLK_T:
		{
			x--;
			m_Player.EmptyNumberHand();
			m_Player.DrawNumberCardsIntoHand(x);
		}
		break;

		case SDLK_Y:
		{
			x++;
			m_Player.EmptyNumberHand();
			m_Player.DrawNumberCardsIntoHand(x);
		}
		break;


		case SDLK_O:
		{
			m_Enemy->Heal(1);
		}
		break;

		case SDLK_P:
		{
			m_Enemy->TakeDamage(1);
		}
		break;

		case SDLK_L:
		{
			m_Player.TakeDamage(1);
		}
		break;

		case SDLK_K:
		{
			m_Player.Heal(1);
		}
		break;

		default:
			break;
		}
	}
	break;

	default:
		break;
	}
}

void BattleScene::OnExit()
{

}

void BattleScene::Update(const float& deltaTime)
{
	SDL_Window& window = Services::GetWindow();
	SDL_GetWindowSize(&window, &m_WindowWidth, &m_WindowHeight);
	m_WindowCenterX = m_WindowWidth / 2; 
	m_WindowCenterY = m_WindowHeight / 2;

	CalculateUpdatedDrawPositions(deltaTime);
















}

void BattleScene::CalculateUpdatedDrawPositions(const float& deltaTime)
{
	m_CharacterDrawRect.x = m_WindowCenterX / 2 - (c_AvatarDrawSize / 2);
	m_CharacterDrawRect.y = m_WindowCenterY / 2 - (c_AvatarDrawSize / 2);
	m_CharacterDrawRect.w = c_AvatarDrawSize;
	m_CharacterDrawRect.h = c_AvatarDrawSize;

	m_EnemyDrawRect.x = ((m_WindowCenterX / 2) * 3) - (c_AvatarDrawSize / 2);
	m_EnemyDrawRect.y = ((m_WindowCenterY / 2)) - (c_AvatarDrawSize / 2);
	m_EnemyDrawRect.w = c_AvatarDrawSize;
	m_EnemyDrawRect.h = c_AvatarDrawSize;

	if (m_PlayerHealthBar != nullptr)
	{
		SDL_FRect newRect = m_PlayerHealthBar->GetDimensions();
		newRect.x = m_CharacterDrawRect.x + (c_AvatarDrawSize / 2) - (newRect.w / 2);
		newRect.y = m_CharacterDrawRect.y + c_AvatarDrawSize + c_HealthBarGapFromAvatar;
		m_PlayerHealthBar->SetDimensions(newRect);

		float value = (float)m_Player.GetCurrentHealth() / (float)m_Player.GetMaxHealth();
		m_PlayerHealthBar->SetProgressValue(value);
		m_PlayerHealthBar->Update(deltaTime);
	}

	if (m_EnemyHealthBar != nullptr && m_Enemy != nullptr)
	{
		SDL_FRect newRect = m_EnemyHealthBar->GetDimensions();
		newRect.x = m_EnemyDrawRect.x + (c_AvatarDrawSize / 2) - (newRect.w / 2);
		newRect.y = m_EnemyDrawRect.y + c_AvatarDrawSize + c_HealthBarGapFromAvatar;
		m_EnemyHealthBar->SetDimensions(newRect);

		float value = (float)m_Enemy->GetCurrentHealth() / (float)m_Enemy->GetMaxHealth();
		m_EnemyHealthBar->SetProgressValue(value);
		m_EnemyHealthBar->Update(deltaTime);
	}
}

void BattleScene::Render(SDL_Renderer& renderer) const
{

	RenderCharacters(renderer);
	RenderCardHands(renderer);
}

void BattleScene::RenderCharacters(SDL_Renderer& renderer) const
{
	if (m_EnemyHealthBar != nullptr && m_Enemy != nullptr)
	{
		if (m_Enemy->GetTexture() != nullptr)
		{
			SDL_RenderTexture(&renderer, m_Enemy->GetTexture(), nullptr, &m_EnemyDrawRect);
		}

		m_EnemyHealthBar->Render(renderer);
	}

	if (m_PlayerHealthBar != nullptr)
	{
		if (m_Player.GetTexture() != nullptr)
		{
			SDL_RenderTexture(&renderer, m_Player.GetTexture(), nullptr, &m_CharacterDrawRect);
		}

		m_PlayerHealthBar->Render(renderer);
	}
}

void BattleScene::RenderCardHands(SDL_Renderer& renderer) const
{
	int splitCount = 4;
	int splitX = m_WindowWidth / splitCount;
	int splitY = m_WindowHeight / splitCount;
	for (size_t i = 0; i < splitCount; i++)
	{
		for (size_t j = 0; j < splitCount; j++)
		{
			SDL_FRect rect;
			rect.w = splitX;
			rect.h = splitY;
			rect.x = i * splitX;
			rect.y = j * splitY;

			SDL_RenderRect(&renderer, &rect);
		}
	}












	const std::vector<NumberCard*>& numbersHand = m_Player.GetNumbersHand();
	int numbersCount = numbersHand.size();

	int totalWidth = numbersCount * c_NumbersCardDrawSize + (numbersCount * c_GapBetweenNumbersCards);
	int drawPositionX = m_WindowCenterX - (totalWidth / 2) + (c_GapBetweenNumbersCards / 2);
	int drawPositionY = (((m_WindowHeight / 3) * 2) + m_WindowHeight) / 2 - c_NumbersCardDrawSize;

	for (size_t i = 0; i < numbersHand.size(); i++)
	{
		SDL_FRect drawRect;
		drawRect.x = drawPositionX;
		drawRect.y = drawPositionY;
		drawRect.w = c_NumbersCardDrawSize;
		drawRect.h = c_NumbersCardDrawSize;

		SDL_RenderRect(&renderer, &drawRect);

		drawPositionX += c_NumbersCardDrawSize;
		drawPositionX += c_GapBetweenNumbersCards;
	}

	const std::vector<OperandCard*>& operandHand = m_Player.GetOperandHand();
	int operandCount = operandHand.size();

	totalWidth = operandCount * c_OperandCardDrawSize + (operandCount * c_GapBetweenOperandsCards);
	drawPositionX = m_WindowCenterX - (totalWidth / 2) + (c_GapBetweenOperandsCards / 2);

	for (size_t i = 0; i < operandHand.size(); i++)
	{
		SDL_FRect drawRect;
		drawRect.x = drawPositionX;
		drawRect.y = drawPositionY + c_NumbersCardDrawSize + (c_OperandCardDrawSize / 2);
		drawRect.w = c_OperandCardDrawSize;
		drawRect.h = c_OperandCardDrawSize;

		SDL_RenderRect(&renderer, &drawRect);

		drawPositionX += c_OperandCardDrawSize;
		drawPositionX += c_GapBetweenOperandsCards;
	}
}

void BattleScene::SetupNewBattle()
{
	m_Enemy = new Ghost();

	m_Player.GetDeck().ResetDeck();
	m_Player.EmptyHands();

	m_Player.DrawNumberCardsIntoHand(6);
	m_Player.DrawOperandCardsIntoHand(4);
}