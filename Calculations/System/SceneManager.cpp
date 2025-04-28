#include "pch.h"
#include "SceneManager.h"
#include <Scenes/MenuScene.h>
#include <Scenes/BattleScene.h>
#include <Scenes/ShopScene.h>
#include <Scenes/MapScene.h>

SceneManager::SceneManager()
{
	m_CurrentScene = nullptr;

	m_SceneList = std::unordered_map<SCENE_IDENTIFIER, Scene*>();
	m_SceneList.insert({ SCENE_IDENTIFIER::SCENE_MAIN_MENU, new MenuScene(*this) });
	m_SceneList.insert({ SCENE_IDENTIFIER::SCENE_BATTLE, new BattleScene(*this) });
	m_SceneList.insert({ SCENE_IDENTIFIER::SCENE_SHOP, new ShopScene(*this) });
	m_SceneList.insert({ SCENE_IDENTIFIER::SCENE_MAP, new MapScene(*this) });

	ChangeScene(SCENE_IDENTIFIER::SCENE_MAIN_MENU);
}

SceneManager::~SceneManager()
{
	for (auto& itr : m_SceneList)
	{
		delete itr.second;
		itr.second = nullptr;
	}

	m_SceneList.clear();
}

void SceneManager::PassEventToScene(const SDL_Event& e)
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->HandleEvent(e);
	}
}

void SceneManager::ChangeScene(const SCENE_IDENTIFIER& targetScene)
{
	m_IsPendingSceneChange = true;
	m_PendingScene = targetScene;
}

void SceneManager::ApplyPendingSceneChange()
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->OnExit();
	}

	m_CurrentScene = nullptr;

	std::unordered_map<SCENE_IDENTIFIER, Scene*>::iterator found = m_SceneList.find(m_PendingScene);
	if (found != m_SceneList.end())
	{
		if (found->second != nullptr)
		{
			m_CurrentScene = found->second;
			m_IsPendingSceneChange = false;
		}
	}

	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->OnEnter();
	}
}

void SceneManager::Update(const float& deltaTime)
{
	if (m_IsPendingSceneChange == true)
	{
		ApplyPendingSceneChange();
	}

	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->Update(deltaTime);
	}
}

void SceneManager::Render(SDL_Renderer& renderer) const
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->Render(renderer);
	}
	else
	{
		SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
		SDL_RenderDebugText(&renderer, 64, 64, "No scene selected in SceneManager.");
	}
}
