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
	const float c_MapScrollSpeed;
	const float c_MapEdgePadding;
	const float c_ButtonPressCooldown;

	SDL_Texture* m_BackgroundTexture;
	SDL_Texture* m_StartNodeTexture;
	SDL_FRect m_StartNodeDrawRect;
	SDL_Texture* m_EndNodeTexture;
	SDL_FRect m_EndNodeDrawRect;

	int m_SelectedNodeIndex;
	float m_ButtonPressCooldownTimer;
	bool m_CanSelectButton;
	bool m_IsButtonRectsDirty;
	SDL_FRect m_NodeButtons[c_MapLength * c_MapWidth];
	void RecalculateButtonRects();

	float m_MapScrollOffset;
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

