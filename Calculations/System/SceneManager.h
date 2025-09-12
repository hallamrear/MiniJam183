#pragma once
#include <unordered_map>
#include <Scenes/Scene.h>

enum SCENE_IDENTIFIER
{
	SCENE_INTRO = 0,
	SCENE_MAIN_MENU,
	SCENE_BATTLE,
	SCENE_MAP,
	SCENE_SHOP,
	SCENE_REST,
	SCENE_RANDOM_EVENT,
	SCENE_DEBUG_TEST
};

class SceneManager
{
private:
	bool m_IsPendingSceneChange;
	SCENE_IDENTIFIER m_PendingScene;
	std::unordered_map<SCENE_IDENTIFIER, Scene*> m_SceneList;
	Scene* m_CurrentScene;

	void ApplyPendingSceneChange();

public:
	SceneManager();
	~SceneManager();

	void PassEventToScene(const SDL_Event& e);
	void ChangeScene(const SCENE_IDENTIFIER& targetScene);
	Scene* GetSceneByIdentifier(const SCENE_IDENTIFIER& requestedSceneIdentifier);

	void Update(const float& deltaTime);
	void Render(SDL_Renderer& renderer) const;
};

