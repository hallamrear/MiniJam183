#include "pch.h"
#include "BattleScene.h"
#include <Graphics/Texture.h>
#include <System/Services.h>
#include <Gameplay/Enemies/Ghost.h>
#include <System/Collision.h>
#include <System/SceneManager.h>

constexpr const float c_AvatarDrawWidth = 256.0f;
constexpr const float c_AvatarDrawHeight = 256.0f;
constexpr const float c_NumbersCardDrawWidth = 128.0f;
constexpr const float c_NumbersCardDrawHeight = 183.0f;
constexpr const float c_OperandCardDrawWidth = c_NumbersCardDrawWidth / 2.0f;
constexpr const float c_OperandCardDrawHeight = c_NumbersCardDrawHeight / 2.0f;
constexpr const float c_SelectedOperandCardWidth = (c_NumbersCardDrawWidth / 5) * 3;
constexpr const float c_SelectedOperandCardHeight = (c_NumbersCardDrawHeight / 5) * 3;
constexpr const float c_SelectedNumberCardWidth = (c_NumbersCardDrawWidth / 5) * 4;
constexpr const float c_SelectedNumberCardHeight = (c_NumbersCardDrawHeight / 5) * 4;
constexpr const float c_GapBetweenSelectedCardElements = 16.0f;
constexpr const float c_GapBetweenNumbersCards = 32.0f;
constexpr const float c_GapBetweenOperandsCards = c_GapBetweenNumbersCards / 2.0f;
constexpr const float c_HealthBarGapFromAvatar = 16.0f;
constexpr const float c_GapBetweenCardsAndButton = 32.0f;
constexpr const float c_GapBetweenEquationButtons = 32.0f;
constexpr const float c_ButtonWidth = 256.0f;
constexpr const float c_ButtonHeight = 64.0f;
constexpr const float c_PlayerAttackAnimationLength = 5.0f;
constexpr const float c_EnemyAttackAnimationLength = 5.0f;
constexpr const float c_EnemyDyingAnimationLength = 5.0f;

constexpr const float TARGET_RESOLUTION_X = 1920;
constexpr const float TARGET_RESOLUTION_Y = 1080;

BattleScene::BattleScene(SceneManager& manager) : Scene(manager), m_Player(Services::GetPlayer())
{
	m_WindowSizeScalingX = 0.0f;
	m_WindowSizeScalingY = 0.0f;

	m_Enemy = nullptr;
	m_LeftClickDown = false;
	m_MouseX = -1;
	m_MouseY = -1;

	SDL_Window& window = Services::GetWindow();
	m_WindowCenterX = 0;
	m_WindowCenterY = 0;
	m_WindowWidth = 0;
	m_WindowHeight = 0;

	m_AttackAnimationTimerElapsed = 0.0f;
	m_BattleState = BATTLE_STATE::PLAYER_MOVE;
	m_CanPickCard = true;

	m_CharacterDrawRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_EnemyDrawRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_SelectedCardDrawRects[0] = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_SelectedCardDrawRects[1] = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_SelectedCardDrawRects[2] = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };

	m_PlayerHealthBar = new ProgressBar(SDL_FRect{ (INITIAL_WINDOW_WIDTH / 4) * 1 - 100.0f, (INITIAL_WINDOW_HEIGHT / 4) * 3, 200.0f, 20.0f });
	m_EnemyHealthBar  = new ProgressBar(SDL_FRect{ (INITIAL_WINDOW_WIDTH / 4) * 3 - 100.0f, (INITIAL_WINDOW_HEIGHT / 4) * 3, 200.0f, 20.0f });

	m_OperandHandDrawRects = std::vector<SDL_FRect>();
	m_NumbersHandDrawRects = std::vector<SDL_FRect>();

	m_DiscardEquationButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_SubmitButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_ClearEquationButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_ExitButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };

	m_SelectedNumbersForEquation[0] = nullptr;
	m_SelectedNumbersForEquation[1] = nullptr;
	m_SelectedOperandForEquation = nullptr;

	m_NumberCardTexture = nullptr;
	Texture::LoadPNG("Content/TestNumberCard.png", m_NumberCardTexture);

	m_OperandCardTexture = nullptr;
	Texture::LoadPNG("Content/TestOperandCard.png", m_OperandCardTexture);

	m_MissingCardTexture = nullptr;
	Texture::LoadPNG("Content/MissingCard.png", m_MissingCardTexture);
}

BattleScene::~BattleScene()
{
	m_SelectedNumbersForEquation[0] = nullptr;
	m_SelectedNumbersForEquation[1] = nullptr;
	m_SelectedOperandForEquation = nullptr;

	if (m_NumberCardTexture != nullptr)
	{
		SDL_DestroyTexture(m_NumberCardTexture);
		m_NumberCardTexture = nullptr;
	}

	if (m_OperandCardTexture != nullptr)
	{
		SDL_DestroyTexture(m_OperandCardTexture);
		m_OperandCardTexture = nullptr;
	}

	if (m_MissingCardTexture != nullptr)
	{
		SDL_DestroyTexture(m_MissingCardTexture);
		m_MissingCardTexture = nullptr;
	}
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
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			m_LeftClickDown = true;
			m_MouseX = e.button.x;
			m_MouseY = e.button.y;
		}
	}
	break;

	case SDL_EVENT_MOUSE_BUTTON_UP:
	{
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			m_CanPickCard = true;
			m_LeftClickDown = false;
			m_MouseX = -1;
			m_MouseY = -1;
		}
	}
	break;

	case SDL_EVENT_KEY_DOWN:
	{
		switch (e.key.key)
		{
		case SDLK_KP_1: { m_BattleState = BATTLE_STATE::PLAYER_MOVE;} break;
		case SDLK_KP_2: { m_BattleState = BATTLE_STATE::PLAYER_ATTACK_ANIMATION;} break;
		case SDLK_KP_3: { m_BattleState = BATTLE_STATE::ENEMY_MOVE;} break;
		case SDLK_KP_4: { m_BattleState = BATTLE_STATE::ENEMY_ATTACK_ANIMATION; } break;
		case SDLK_KP_5: { m_BattleState = BATTLE_STATE::ENEMY_DYING_ANIMATION; } break;
		case SDLK_KP_6: { m_BattleState = BATTLE_STATE::BATTLE_END_SCREEN; } break;

		case SDLK_O: { m_Enemy->Heal(1); } break;
		case SDLK_P: { m_Enemy->TakeDamage(1); } break;
		case SDLK_L: { m_Player.TakeDamage(1); } break;
		case SDLK_K: { m_Player.Heal(1); } break;

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
	m_OperandHandDrawRects.clear();
	m_NumbersHandDrawRects.clear();
}

void BattleScene::Update(const float& deltaTime)
{
	SDL_Window& window = Services::GetWindow();
	SDL_GetWindowSize(&window, &m_WindowWidth, &m_WindowHeight);
	m_WindowCenterX = m_WindowWidth / 2; 
	m_WindowCenterY = m_WindowHeight / 2;

	m_WindowSizeScalingX = m_WindowWidth / TARGET_RESOLUTION_X;
	m_WindowSizeScalingY = m_WindowHeight / TARGET_RESOLUTION_Y;

	switch (m_BattleState)
	{
		case BattleScene::PLAYER_MOVE:
		{
			if (m_LeftClickDown && m_CanPickCard)
			{
				CheckForClickCollisions();
			}
		}
			break;

		case BattleScene::PLAYER_ATTACK_ANIMATION:
		{
			m_AttackAnimationTimerElapsed += deltaTime;

			if (m_AttackAnimationTimerElapsed >= c_PlayerAttackAnimationLength)
			{
				m_AttackAnimationTimerElapsed = 0.0f;
				m_BattleState = BattleScene::ENEMY_MOVE;
			}
		}
			break;

		case BattleScene::ENEMY_MOVE:
		{
			m_AttackAnimationTimerElapsed += deltaTime;

			if (m_AttackAnimationTimerElapsed >= 2.0f)
			{
				m_AttackAnimationTimerElapsed = 0.0f;
				m_Player.TakeDamage(rand() % 10 + 1);
				m_BattleState = BattleScene::ENEMY_ATTACK_ANIMATION;
			}
		}
			break;

		case BattleScene::ENEMY_ATTACK_ANIMATION:
		{
			m_AttackAnimationTimerElapsed += deltaTime;

			if (m_AttackAnimationTimerElapsed >= c_EnemyAttackAnimationLength)
			{
				m_AttackAnimationTimerElapsed = 0.0f;

				m_BattleState = BattleScene::PLAYER_MOVE;

				m_Player.DrawNumberCardsIntoHand(2);
				m_Player.DrawOperandCardsIntoHand(1);
			}
		}
			break;

		case BATTLE_STATE::ENEMY_DYING_ANIMATION:
		{
			m_AttackAnimationTimerElapsed += deltaTime;

			if (m_AttackAnimationTimerElapsed >= c_EnemyDyingAnimationLength)
			{
				m_AttackAnimationTimerElapsed = 0.0f;
				m_BattleState = BATTLE_STATE::BATTLE_END_SCREEN;
			}
		}
		break;

		case BATTLE_STATE::BATTLE_END_SCREEN:
		{
			//Check shop button press

			//Check quit button press

			int padding = 200;
			m_ExitButtonRect = SDL_FRect{ (float)padding, (float)padding, (float)m_WindowWidth - (2 * padding), (float)m_WindowHeight - (2 * padding) };

			if (m_LeftClickDown)
			{
				if (Collision::PointInRect(m_MouseX, m_MouseY, m_ExitButtonRect))
				{
					m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_MAIN_MENU);
				}
			}
		}
		break;

		default:
			break;
	}

	CalculateUpdatedDrawPositions(deltaTime);
}

SDL_FRect* rect = nullptr;

void BattleScene::CheckForClickCollisions()
{
	if (Collision::PointInRect(m_MouseX, m_MouseY, m_ClearEquationButtonRect))
	{
		ClearEquation();
		return;
	}

	if (Collision::PointInRect(m_MouseX, m_MouseY, m_SubmitButtonRect))
	{
		if (CheckForValidEquation())
		{
			ApplyEquation();
			ClearEquation();
		}
		return;
	}

	if (Collision::PointInRect(m_MouseX, m_MouseY, m_DiscardEquationButtonRect))
	{
		if (CheckForValidEquation())
		{
			DiscardEquationCards();
			ClearEquation();
		}
		return;
	}

	bool foundClick = false;
	Card* foundCard = nullptr;
	rect = nullptr;

	for (size_t i = 0; i < m_OperandHandDrawRects.size(); i++)
	{
		if (foundClick)
			break;

		if (Collision::PointInRect(m_MouseX, m_MouseY, m_OperandHandDrawRects[i]))
		{
			foundCard = m_Player.GetOperandHand()[i];
			rect = &m_OperandHandDrawRects[i];
			foundClick = true;
		}
	}

	for (size_t i = 0; i < m_NumbersHandDrawRects.size(); i++)
	{
		if (foundClick)
			break;

		if (Collision::PointInRect(m_MouseX, m_MouseY, m_NumbersHandDrawRects[i]))
		{
			foundCard = m_Player.GetNumbersHand()[i];
			rect = &m_NumbersHandDrawRects[i];
			foundClick = true;
		}
	}

	if (foundCard != nullptr)
	{
		m_CanPickCard = false;
		AddCardToEquation(foundCard);
	}

}

bool BattleScene::CheckForValidEquation()
{
	return ((m_SelectedNumbersForEquation[0] != nullptr) && (m_SelectedNumbersForEquation[1] != nullptr) && (m_SelectedOperandForEquation != nullptr));
}

void BattleScene::ApplyEquation()
{
	int x = 0;
	int y = 0;
	
	//This should be valid because CheckForValidEquation() makes sure theyre valid pointers.
	x = m_SelectedNumbersForEquation[0]->GetIntValue();
	y = m_SelectedNumbersForEquation[1]->GetIntValue();

	int output = 0;

	switch (m_SelectedOperandForEquation->GetOperand())
	{
		case OPERAND_TYPE::ADDITION:		{ output = x + y; } break;
		case OPERAND_TYPE::SUBTRACTION:		{ output = x - y; } break;
		case OPERAND_TYPE::DIVISION:		{ output = x / y; } break;
		case OPERAND_TYPE::MULTIPLICATION:  { output = x * y; } break;

		default:
			assert(false);
			break;
	}

	m_Enemy->TakeDamage(output);

	if (m_Enemy->GetIsAlive() == false)
	{
		m_BattleState = BATTLE_STATE::ENEMY_DYING_ANIMATION;
	}
	else
	{
		m_BattleState = BATTLE_STATE::PLAYER_ATTACK_ANIMATION;
	}

	m_Player.RemoveCardFromHand(m_SelectedNumbersForEquation[0]);
	m_Player.RemoveCardFromHand(m_SelectedNumbersForEquation[1]);
	m_Player.RemoveCardFromHand(m_SelectedOperandForEquation);
}

void BattleScene::DiscardEquationCards()
{
	m_Player.RemoveCardFromHand(m_SelectedNumbersForEquation[0]);
	m_Player.RemoveCardFromHand(m_SelectedNumbersForEquation[1]);
	m_Player.RemoveCardFromHand(m_SelectedOperandForEquation);

	m_BattleState = BATTLE_STATE::ENEMY_MOVE;
}

void BattleScene::ClearEquation()
{
	m_SelectedNumbersForEquation[0] = nullptr;
	m_SelectedNumbersForEquation[1] = nullptr;
	m_SelectedOperandForEquation = nullptr;
}

void BattleScene::CalculateUpdatedDrawPositions(const float& deltaTime)
{
	CalculateCardHandDrawPositions(deltaTime, m_WindowCenterX, m_WindowHeight);
	CalculateUpdatedAvatarDrawPositions(deltaTime, m_WindowCenterX, m_WindowCenterY);
}

void BattleScene::CalculateSelectedCardRectPositions(const float& deltaTime, const float& rootPositionX, const float& rootPositionY)
{
	const float equationNumberCardRatio = c_SelectedNumberCardHeight / c_SelectedNumberCardWidth;
	const float equationNumberCardScaledWidth = c_SelectedNumberCardWidth * m_WindowSizeScalingX;
	const float equationNumberCardScaledHeight = equationNumberCardScaledWidth * equationNumberCardRatio;

	const float equationOperandCardRatio = c_SelectedOperandCardHeight / c_SelectedOperandCardWidth;
	const float equationOperandCardScaledWidth = c_SelectedOperandCardWidth * m_WindowSizeScalingX;
	const float equationOperandCardScaledHeight = equationOperandCardScaledWidth * equationOperandCardRatio;

	float drawPositionY = rootPositionY - c_GapBetweenCardsAndButton - c_SelectedNumberCardHeight;

	m_SelectedCardDrawRects[0] = SDL_FRect{ rootPositionX - (equationNumberCardScaledWidth / 2) - c_GapBetweenSelectedCardElements - equationNumberCardScaledWidth, drawPositionY - (equationNumberCardScaledHeight / 2), equationNumberCardScaledWidth, equationNumberCardScaledHeight };
	m_SelectedCardDrawRects[1] = SDL_FRect{ rootPositionX + (equationNumberCardScaledWidth / 2) + c_GapBetweenSelectedCardElements, drawPositionY - (equationNumberCardScaledHeight / 2), equationNumberCardScaledWidth, equationNumberCardScaledHeight };
	m_SelectedCardDrawRects[2] = SDL_FRect{ rootPositionX - (equationOperandCardScaledWidth / 2), drawPositionY - (equationOperandCardScaledHeight / 2), equationOperandCardScaledWidth, equationOperandCardScaledHeight};
}

void BattleScene::CalculateEquationButtonRectPositions(const float& deltaTime, const float& rootPositionX, const float& rootPositionY)
{
	const float buttonSizeRatio = c_ButtonHeight / c_ButtonWidth;
	const float scaledGap = m_WindowSizeScalingX * c_GapBetweenCardsAndButton;
	const float scaledWidth = c_ButtonWidth * m_WindowSizeScalingX;
	const float scaledHeight = scaledWidth * buttonSizeRatio;

	float drawPositionY = rootPositionY - scaledGap - c_ButtonHeight;

	m_DiscardEquationButtonRect.w = scaledWidth;
	m_DiscardEquationButtonRect.h = scaledHeight;
	m_DiscardEquationButtonRect.x = rootPositionX - (scaledWidth / 2) - scaledGap - scaledWidth;
	m_DiscardEquationButtonRect.y = drawPositionY;

	m_ClearEquationButtonRect.w = scaledWidth;
	m_ClearEquationButtonRect.h = scaledHeight;
	m_ClearEquationButtonRect.x = rootPositionX - (scaledWidth / 2);
	m_ClearEquationButtonRect.y = drawPositionY;


	m_SubmitButtonRect.w = scaledWidth;
	m_SubmitButtonRect.h = scaledHeight;
	m_SubmitButtonRect.x = rootPositionX + (scaledWidth / 2) + scaledGap;
	m_SubmitButtonRect.y = drawPositionY;

	CalculateSelectedCardRectPositions(deltaTime, m_WindowCenterX, drawPositionY);
}

void BattleScene::CalculateCardHandDrawPositions(const float& deltaTime, const float& rootPositionX, const float& rootPositionY)
{
	m_OperandHandDrawRects.clear();
	m_NumbersHandDrawRects.clear();

	int numbersCount = m_Player.GetNumbersHand().size();

	const float numbersCardDrawSizeRatio = c_NumbersCardDrawHeight / c_NumbersCardDrawWidth;
	const float operandCardDrawSizeRatio = c_OperandCardDrawHeight / c_OperandCardDrawWidth;

	const float scaledNumbersCardDrawSizeX = c_NumbersCardDrawWidth * m_WindowSizeScalingX;
	const float scaledNumbersCardDrawSizeY = scaledNumbersCardDrawSizeX * numbersCardDrawSizeRatio;

	const float scaledOperandCardDrawSizeX = c_OperandCardDrawWidth * m_WindowSizeScalingX;
	const float scaledOperandCardDrawSizeY = scaledOperandCardDrawSizeX * operandCardDrawSizeRatio;

	int totalWidth = numbersCount * scaledNumbersCardDrawSizeX + (numbersCount * c_GapBetweenNumbersCards);
	int drawPositionX = rootPositionX - (totalWidth / 2) + (c_GapBetweenNumbersCards / 2);
	int drawPositionY = rootPositionY - c_GapBetweenNumbersCards - scaledOperandCardDrawSizeY - c_GapBetweenNumbersCards - scaledNumbersCardDrawSizeY;

	for (size_t i = 0; i < numbersCount; i++)
	{
		SDL_FRect drawRect;
		drawRect.x = drawPositionX;
		drawRect.y = drawPositionY;
		drawRect.w = scaledNumbersCardDrawSizeX;
		drawRect.h = scaledNumbersCardDrawSizeY;

		m_NumbersHandDrawRects.push_back(drawRect);

		drawPositionX += scaledNumbersCardDrawSizeX;
		drawPositionX += c_GapBetweenNumbersCards;
	}

	int operandCount = m_Player.GetOperandHand().size();

	totalWidth = operandCount * scaledOperandCardDrawSizeX + (operandCount * c_GapBetweenOperandsCards);
	drawPositionX = rootPositionX - (totalWidth / 2) + (c_GapBetweenOperandsCards / 2);

	for (size_t i = 0; i < operandCount; i++)
	{
		SDL_FRect drawRect;
		drawRect.x = drawPositionX;
		drawRect.y = drawPositionY + scaledNumbersCardDrawSizeY + (c_GapBetweenNumbersCards);
		drawRect.w = scaledOperandCardDrawSizeX;
		drawRect.h = scaledOperandCardDrawSizeY;

		m_OperandHandDrawRects.push_back(drawRect);

		drawPositionX += scaledOperandCardDrawSizeX;
		drawPositionX += c_GapBetweenOperandsCards;
	}

	CalculateEquationButtonRectPositions(deltaTime, rootPositionX, drawPositionY);
}

void BattleScene::CalculateUpdatedAvatarDrawPositions(const float& deltaTime, const float& rootPositionX, const float& rootPositionY)
{
	const float avatarDrawSizeRatio = c_AvatarDrawHeight / c_AvatarDrawWidth;
	const float scaledAvatarDrawWidth = c_AvatarDrawWidth * m_WindowSizeScalingX;
	const float scaledAvatarDrawHeight = scaledAvatarDrawWidth * avatarDrawSizeRatio;

	m_CharacterDrawRect.x = rootPositionX / 2 - (scaledAvatarDrawWidth / 2);
	m_CharacterDrawRect.y = rootPositionY / 2 - (scaledAvatarDrawHeight / 2);
	m_CharacterDrawRect.w = scaledAvatarDrawWidth;
	m_CharacterDrawRect.h = scaledAvatarDrawHeight;

	m_EnemyDrawRect.x = ((rootPositionX / 2) * 3) - (scaledAvatarDrawWidth / 2);
	m_EnemyDrawRect.y = ((rootPositionY / 2)) - (scaledAvatarDrawHeight / 2);
	m_EnemyDrawRect.w = scaledAvatarDrawWidth;
	m_EnemyDrawRect.h = scaledAvatarDrawHeight;

	if (m_PlayerHealthBar != nullptr)
	{
		SDL_FRect newRect = m_PlayerHealthBar->GetDimensions();
		newRect.w = scaledAvatarDrawWidth;
		newRect.h = scaledAvatarDrawWidth * 0.15f;
		newRect.x = m_CharacterDrawRect.x + (scaledAvatarDrawWidth / 2) - (newRect.w / 2);
		newRect.y = m_CharacterDrawRect.y + scaledAvatarDrawHeight + c_HealthBarGapFromAvatar;
		m_PlayerHealthBar->SetDimensions(newRect);

		float value = (float)m_Player.GetCurrentHealth() / (float)m_Player.GetMaxHealth();
		m_PlayerHealthBar->SetProgressValue(value);
		m_PlayerHealthBar->Update(deltaTime);
	}

	if (m_EnemyHealthBar != nullptr && m_Enemy != nullptr)
	{
		SDL_FRect newRect = m_EnemyHealthBar->GetDimensions();
		newRect.w = scaledAvatarDrawWidth;
		newRect.h = scaledAvatarDrawWidth * 0.15f;
		newRect.x = m_EnemyDrawRect.x + (scaledAvatarDrawWidth / 2) - (newRect.w / 2);
		newRect.y = m_EnemyDrawRect.y + scaledAvatarDrawHeight + c_HealthBarGapFromAvatar;
		m_EnemyHealthBar->SetDimensions(newRect);

		float value = (float)m_Enemy->GetCurrentHealth() / (float)m_Enemy->GetMaxHealth();
		m_EnemyHealthBar->SetProgressValue(value);
		m_EnemyHealthBar->Update(deltaTime);
	}
}

void BattleScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);

	SDL_RenderDebugTextFormat(&renderer, 10, 30, "Numbers: %i Operands: %i", (int)m_Player.GetNumbersHand().size(), (int)m_Player.GetOperandHand().size());

	/*int splitCount = 4;
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
	}*/

	RenderCharacters(renderer);

	switch (m_BattleState)
	{
		case BattleScene::PLAYER_MOVE:
		{
			SDL_RenderDebugText(&renderer, 10, 10, "BATTLE_STATE::PLAYER_MOVE");

			RenderEquation(renderer);

			RenderCardHands(renderer);

			//Render buttons
			SDL_SetRenderDrawColorFloat(&renderer, 0.0f, 1.0f, 0.0f, 1.0f);
			SDL_RenderRect(&renderer, &m_ClearEquationButtonRect);
			SDL_RenderDebugText(&renderer, m_ClearEquationButtonRect.x, m_ClearEquationButtonRect.y + m_ClearEquationButtonRect.h / 2, "Clear Equation");

			SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 0.0f, 1.0f);
			SDL_RenderRect(&renderer, &m_SubmitButtonRect);
			SDL_RenderDebugText(&renderer, m_SubmitButtonRect.x, m_SubmitButtonRect.y + m_SubmitButtonRect.h / 2, "Submit Equation");

			SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 0.0f, 0.0f, 1.0f);
			SDL_RenderRect(&renderer, &m_DiscardEquationButtonRect);
			SDL_RenderDebugText(&renderer, m_DiscardEquationButtonRect.x, m_DiscardEquationButtonRect.y + m_DiscardEquationButtonRect.h / 2, "Discard Equation");
		}
			break;

		case BattleScene::PLAYER_ATTACK_ANIMATION:
			SDL_RenderDebugText(&renderer, 10, 10, "BATTLE_STATE::PLAYER_ATTACK_ANIMATION");
			break;

		case BattleScene::ENEMY_MOVE:
			SDL_RenderDebugText(&renderer, 10, 10, "BATTLE_STATE::ENEMY_MOVE");
			break;

		case BattleScene::ENEMY_ATTACK_ANIMATION:
			SDL_RenderDebugText(&renderer, 10, 10, "BATTLE_STATE::ENEMY_ATTACK_ANIMATION");
			break;

		case BATTLE_STATE::ENEMY_DYING_ANIMATION:
		{
			SDL_RenderDebugText(&renderer, 10, 10, "BATTLE_STATE::ENEMY_DYING_ANIMATION");
		}
		break;

		case BATTLE_STATE::BATTLE_END_SCREEN:
		{
			SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
			SDL_RenderDebugText(&renderer, 10, 10, "BATTLE_STATE::BATTLE_END_SCREEN");
			SDL_RenderFillRect(&renderer, &m_ExitButtonRect);
			SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 0.0f, 0.0f, 1.0f);
			SDL_RenderDebugText(&renderer, m_ExitButtonRect.x + 64, m_ExitButtonRect.y + 64, "EXIT BUTTON TO MENU");
		}
		break;

		default:
			break;
	}
}

void BattleScene::RenderEquation(SDL_Renderer& renderer) const
{
	std::string c_x = "?";
	std::string c_op = "?";
	std::string c_y = "?";
	std::string c_out = "?";

	if (m_SelectedNumbersForEquation[0] != nullptr)
	{
		c_x = std::to_string(m_SelectedNumbersForEquation[0]->GetIntValue());
		SDL_RenderTexture(&renderer, &m_Player.GetDeck().GetNumberCardTexture(m_SelectedNumbersForEquation[0]->GetValue()), nullptr, &m_SelectedCardDrawRects[0]);
	}
	else
	{
		SDL_RenderTexture(&renderer, m_MissingCardTexture, nullptr, &m_SelectedCardDrawRects[0]);
	}

	if (m_SelectedNumbersForEquation[1] != nullptr)
	{
		c_y = std::to_string(m_SelectedNumbersForEquation[1]->GetIntValue());
		SDL_RenderTexture(&renderer, &m_Player.GetDeck().GetNumberCardTexture(m_SelectedNumbersForEquation[1]->GetValue()), nullptr, &m_SelectedCardDrawRects[1]);
	}
	else
	{
		SDL_RenderTexture(&renderer, m_MissingCardTexture, nullptr, &m_SelectedCardDrawRects[1]);
	}

	if (m_SelectedOperandForEquation != nullptr)
	{
		const OPERAND_TYPE& type = m_SelectedOperandForEquation->GetOperand();

		switch (type)
		{
		case OPERAND_TYPE::ADDITION: { c_op = "+"; } break;
		case OPERAND_TYPE::SUBTRACTION: { c_op = "-"; } break;
		case OPERAND_TYPE::DIVISION: { c_op = "/"; } break;
		case OPERAND_TYPE::MULTIPLICATION: { c_op = "*"; } break;

		default:
			assert(false);
			break;
		}

		SDL_RenderTexture(&renderer, &m_Player.GetDeck().GetOperandCardTexture(type), nullptr, &m_SelectedCardDrawRects[2]);
	}
	else
	{
		SDL_RenderTexture(&renderer, m_MissingCardTexture, nullptr, &m_SelectedCardDrawRects[2]);
	}

	if (m_SelectedNumbersForEquation[0] != nullptr && m_SelectedNumbersForEquation[1] != nullptr && m_SelectedOperandForEquation != nullptr)
	{
		//This should be valid because CheckForValidEquation() makes sure theyre valid pointers.
		int x = m_SelectedNumbersForEquation[0]->GetIntValue();
		int y = m_SelectedNumbersForEquation[1]->GetIntValue();
		int output = 0;

		switch (m_SelectedOperandForEquation->GetOperand())
		{
		case OPERAND_TYPE::ADDITION: { output = x + y; } break;
		case OPERAND_TYPE::SUBTRACTION: { output = x - y; } break;
		case OPERAND_TYPE::DIVISION: { output = x / y; } break;
		case OPERAND_TYPE::MULTIPLICATION: { output = x * y; } break;

		default:
			assert(false);
			break;
		}
		c_out = std::to_string(output);
	}

	SDL_RenderDebugTextFormat(&renderer, 10, 20, "%s %s %s = %s", c_x.c_str(), c_op.c_str(), c_y.c_str(), c_out.c_str());
}

void BattleScene::AddCardToEquation(Card* card)
{
	switch (card->GetCardType())
	{
	case CARD_TYPE::NUMBER_CARD:
	{
		if (m_SelectedNumbersForEquation[0] == nullptr)
		{
			if (m_SelectedNumbersForEquation[1] != card)
			{
				m_SelectedNumbersForEquation[0] = (NumberCard*)card;
				return;
			}
		}
		
		if (m_SelectedNumbersForEquation[1] == nullptr)
		{
			if (m_SelectedNumbersForEquation[0] != card)
			{
				m_SelectedNumbersForEquation[1] = (NumberCard*)card;
				return;
			}
		}
		//else
		//{
		//	NO SPACE TO ADD CARD
		//}
	}
	break;

	case CARD_TYPE::OPERAND_CARD:
	{
		if (m_SelectedOperandForEquation == nullptr)
		{
			m_SelectedOperandForEquation = (OperandCard*)card;
		}
	}
	break;

	default:
		break;
	}
	

	
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
	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);

	const std::vector<NumberCard*>& numbersHand = m_Player.GetNumbersHand();
	int numbersCount = numbersHand.size();

	for (size_t i = 0; i < numbersCount; i++)
	{
		std::string str = std::to_string(numbersHand[i]->GetIntValue());

		if (m_NumberCardTexture != nullptr)
		{
			SDL_RenderTexture(&renderer, &m_Player.GetDeck().GetNumberCardTexture(numbersHand[i]->GetValue()), nullptr, &m_NumbersHandDrawRects[i]);
		}
		else
		{
			SDL_RenderRect(&renderer, &m_NumbersHandDrawRects[i]);
		}

		SDL_RenderDebugText(&renderer, m_NumbersHandDrawRects[i].x + m_NumbersHandDrawRects[i].w / 2 - 4, m_NumbersHandDrawRects[i].y + m_NumbersHandDrawRects[i].h / 2 - 4, str.c_str());
	}

	const std::vector<OperandCard*>& operandHand = m_Player.GetOperandHand();
	int operandCount = operandHand.size();
	for (size_t i = 0; i < operandCount; i++)
	{
		std::string str = "";

		const OPERAND_TYPE type = operandHand[i]->GetOperand();

		switch (type)
		{
		case ADDITION: { str = "+"; } break;
		case SUBTRACTION: { str = "-"; } break;
		case MULTIPLICATION: { str = "*"; } break;
		case DIVISION: { str = "/"; } break;

		default:
			str = "ERROR";
			break;
		}

		if (m_OperandCardTexture != nullptr)
		{
			SDL_RenderTexture(&renderer, &m_Player.GetDeck().GetOperandCardTexture(type), nullptr, &m_OperandHandDrawRects[i]);
		}
		else
		{
			SDL_RenderRect(&renderer, &m_OperandHandDrawRects[i]);
		}

		SDL_RenderDebugText(&renderer, m_OperandHandDrawRects[i].x + m_OperandHandDrawRects[i].w / 2 - 4, m_OperandHandDrawRects[i].y + m_OperandHandDrawRects[i].h / 2 - 4, str.c_str());
	}
}

void BattleScene::SetupNewBattle()
{
	m_BattleState = BATTLE_STATE::PLAYER_MOVE;

	m_Enemy = new Ghost();

	m_Player.Heal(100000);
	m_Player.GetDeck().ResetDeck();
	m_Player.EmptyHands();

	m_Player.DrawNumberCardsIntoHand(6);
	m_Player.DrawOperandCardsIntoHand(4);
}