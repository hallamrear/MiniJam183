#pragma once
#include <unordered_map>
#include <Scenes/Scene.h>

enum SCENE_IDENTIFIER
{
	SCENE_MAIN_MENU = 0,
	SCENE_BATTLE,
	SCENE_MAP,
	SCENE_SHOP,
};

class SceneManager
{
private:
	std::unordered_map<SCENE_IDENTIFIER, Scene*> m_SceneList;
	Scene* m_CurrentScene;

public:
	SceneManager();
	~SceneManager();

	void PassEventToScene(const SDL_Event& e);
	void ChangeScene(const SCENE_IDENTIFIER& targetScene);

	void Update(const float& deltaTime);
	void Render(SDL_Renderer& renderer) const;
};

