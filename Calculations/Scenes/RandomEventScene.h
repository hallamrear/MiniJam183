#pragma once
#include <Scenes/Scene.h>

class AnimationController;

class RandomEventScene 
	: public Scene
{
private:
	const float c_ClickSafetyTimer = 0.5f;

	enum ENCOUNTER_TYPE : int
	{
		RANDOM_ENCOUNTER_UNKNOWN_OR_NOT_SET = -1,
		RANDOM_ENCOUNTER_NOTHING = 0,
		RANDOM_ENCOUNTER_FREE_FULL_HEAL = 1,
		RANDOM_ENCOUNTER_FREE_CARD = 2,
		RANDOM_ENCOUNTER_RANDOM_FREE_HEAL = 3,
		COUNT = 4,
	};

	AnimationController* m_RandomEventAtlas;
	SDL_FRect m_RandomEventImageDstRect;
	bool m_IsConsumed;
	float m_ClickCooldown;

	void GenerateRandomEncounter();
	void ApplyEncounterChanges();
	void DestroyRandomEncounter();
	ENCOUNTER_TYPE m_GeneratedEncounter;

public:
	RandomEventScene(SceneManager& manager);
	~RandomEventScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

