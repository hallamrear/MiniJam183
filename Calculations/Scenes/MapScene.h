#pragma once
#include <Scenes/Scene.h>
#include <Gameplay/World/WorldMap.h>

class MapScene : public Scene
{
private:
	WorldMap& m_WorldMap;
	SDL_Texture* m_EncounterAtlas;
	SDL_Texture* m_CrossTexture;
	SDL_Texture* m_PlayerIconTexture;
	const float c_EncounterImageWidth;
	const float c_EncounterImageHeight;

	int m_SelectedNodeIndex;
	float m_ButtonPressCooldown;
	bool m_CanSelectButton;
	SDL_FRect m_NodeButtons[c_MapLength * c_MapWidth];
	void RecalculateButtonRects();

	std::vector<const MapNode*> m_PossibleMapMovements;

public:
	MapScene(SceneManager& manager);
	~MapScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

