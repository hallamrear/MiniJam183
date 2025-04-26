#include "pch.h"
#include "BattleScene.h"
#include <Graphics/Texture.h>
#include <System/Services.h>
#include <Gameplay/Enemies/Ghost.h>

BattleScene::BattleScene() : Scene(), m_Player(Services::GetPlayer())
{
	m_Enemy = nullptr;

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
	switch (e.type)
	{
	case SDL_EVENT_KEY_DOWN:
	{
		switch (e.key.key)
		{
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
	if (m_PlayerHealthBar != nullptr)
	{
		float value = (float)m_Player.GetCurrentHealth() / (float)m_Player.GetMaxHealth();
		m_PlayerHealthBar->SetProgressValue(value);
		m_PlayerHealthBar->Update(deltaTime);
	}

	if (m_EnemyHealthBar != nullptr && m_Enemy != nullptr)
	{
		float value = (float)m_Enemy->GetCurrentHealth() / (float)m_Enemy->GetMaxHealth();
		m_EnemyHealthBar->SetProgressValue(value);
		m_EnemyHealthBar->Update(deltaTime);
	}
}

void BattleScene::Render(SDL_Renderer& renderer) const
{
	if (m_EnemyHealthBar != nullptr && m_Enemy != nullptr)
	{
		SDL_FRect enemyTextureLocation = m_EnemyHealthBar->GetDimensions();
		enemyTextureLocation.y -= 164.0f;
		enemyTextureLocation.w = 128.0f;
		enemyTextureLocation.h = 128.0f;

		if (m_Enemy->GetTexture() != nullptr)
		{
			SDL_RenderTexture(&renderer, m_Enemy->GetTexture(), nullptr, &enemyTextureLocation);
		}

		m_EnemyHealthBar->Render(renderer);
	}

	if (m_PlayerHealthBar != nullptr)
	{
		SDL_FRect playerTextureLocation = m_PlayerHealthBar->GetDimensions();
		playerTextureLocation.y -= 164.0f;
		playerTextureLocation.w = 128.0f;
		playerTextureLocation.h = 128.0f;

		if (m_Player.GetTexture() != nullptr)
		{
			SDL_RenderTexture(&renderer, m_Player.GetTexture(), nullptr, &playerTextureLocation);
		}

		m_PlayerHealthBar->Render(renderer);
	}
}

void BattleScene::SetupNewBattle()
{
	m_Enemy = new Ghost();
}