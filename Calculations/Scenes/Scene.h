#pragma once

class Scene
{
protected:
	Scene();

public:
	virtual ~Scene() = 0;

	virtual void OnEnter();
	virtual void HandleEvent(const SDL_Event& e);
	virtual void OnExit();
	virtual void Update(const float& deltaTime);
	virtual void Render(SDL_Renderer& renderer) const;
};

