#pragma once
#include <Scenes/Scene.h>
#include <Graphics/ProgressBar.h>
#include <Gameplay/Player/Player.h>
#include <Gameplay/Enemies/Enemy.h>

class Animation;
class Input;

class BattleScene : public Scene
{
private:
	enum BATTLE_STATE
	{
		PLAYER_MOVE,
		PLAYER_ATTACK_ANIMATION,
		ENEMY_MOVE,
		ENEMY_ATTACK_ANIMATION,
		ENEMY_DYING_ANIMATION,
		PLAYER_DYING_ANIMATION,
		BATTLE_END_SCREEN,
	} m_BattleState;

	NumberCard* m_SelectedNumbersForEquation[2];
	OperandCard* m_SelectedOperandForEquation;
	float m_EnemyPseudoThinkingTime;

	ProgressBar* m_PlayerHealthBar;
	ProgressBar* m_EnemyHealthBar;
	Player& m_Player;
	Enemy* m_Enemy;
	float m_EnemyPseudoThinkingTimeElapsed;

	SDL_Texture* m_GoToMapButtonTexture;
	SDL_Texture* m_ExitButtonTexture;
	SDL_Texture* m_MissingCardTexture;
	SDL_Texture* m_PlayerDeathTextTexture;
	SDL_Texture* m_EnemyDeathTextTexture;
	SDL_Texture* m_DiscardButtonTexture;
	SDL_Texture* m_SubmitButtonTexture;
	SDL_Texture* m_ClearButtonTexture;

	SDL_FRect m_SelectedCardDrawRects[3];
	std::vector<SDL_FRect> m_OperandHandDrawRects;
	std::vector<SDL_FRect> m_NumbersHandDrawRects;
	SDL_FRect m_CharacterDrawRect;
	SDL_FRect m_EnemyDrawRect;
	int m_WindowCenterX = 0;
	int m_WindowCenterY = 0;
	int m_WindowHeight = 0;
	int m_WindowWidth = 0;
	int m_LastDamageRoll = 0;
	float m_WindowSizeScalingX;
	float m_WindowSizeScalingY;

	bool m_CanPickCard;

	void SetupNewBattle();
	void RenderCharacterHealthBar(SDL_Renderer& renderer) const;
	void RenderEquation(SDL_Renderer& renderer) const;
	void RenderCardHands(SDL_Renderer& renderer) const;
	void CalculateUpdatedDrawPositions(const float& deltaTime);
	void CalculateUpdatedAvatarDrawPositions(const float& deltaTime, const float& rootPositionX, const float& rootPositionY);
	void CalculateCardHandDrawPositions(const float& deltaTime, const float& rootPositionX, const float& rootPositionY);
	void CalculateSelectedCardRectPositions(const float& deltaTime, const float& rootPositionX, const float& rootPositionY);
	void CalculateEquationButtonRectPositions(const float& deltaTime, const float& rootPositionX, const float& rootPositionY);
	void ChooseRandomPlayerAttackAnimation();

	void DiscardEquationCards();
	void CheckForClickCollisions();
	bool CheckForValidEquation() const;
	void AddCardToEquation(Card* card);
	void ClearEquation();
	void ApplyEquation();

	SDL_FRect m_ExitButtonRect;
	SDL_FRect m_GoToMapButtonRect;
	SDL_FRect m_DiscardEquationButtonRect;
	SDL_FRect m_SubmitButtonRect;
	SDL_FRect m_ClearEquationButtonRect;
	SDL_FRect m_EndScreenMessageRect;

public:
	BattleScene(SceneManager& manager);
	~BattleScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

