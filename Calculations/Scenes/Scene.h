#pragma once

class SceneManager;

class Scene
{
protected:
	SceneManager& m_SceneManager;
	Scene(SceneManager& sceneManager);

public:
	virtual ~Scene() = 0;

	virtual void OnEnter();
	virtual void HandleEvent(const SDL_Event& e);
	virtual void OnExit();
	virtual void Update(const float& deltaTime);
	virtual void Render(SDL_Renderer& renderer) const;
};

