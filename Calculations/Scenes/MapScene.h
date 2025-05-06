#pragma once
#include <Scenes/Scene.h>

class WorldMap;

class MapScene : public Scene
{
private:
	WorldMap& m_WorldMap;
	SDL_Texture* texture;

public:
	MapScene(SceneManager& manager);
	~MapScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

