#include "pch.h"
#include "Scene.h"
#include "System/SceneManager.h"

Scene::Scene(SceneManager& sceneManager) : m_SceneManager(sceneManager)
{

}

Scene::~Scene()
{

}

void Scene::OnEnter()
{

}

void Scene::HandleEvent(const SDL_Event& e)
{

}

void Scene::OnExit()
{

}

void Scene::Update(const float& deltaTime)
{

}

void Scene::Render(SDL_Renderer& renderer) const
{

}
