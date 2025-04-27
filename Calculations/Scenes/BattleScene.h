#pragma once
#include <Scenes/Scene.h>
#include <Graphics/ProgressBar.h>
#include <Gameplay/Player/Player.h>
#include <Gameplay/Enemies/Enemy.h>

class BattleScene : public Scene
{
private:
	NumberCard* m_SelectedNumbersForEquation[2];
	OperandCard* m_SelectedOperandForEquation;

	ProgressBar* m_PlayerHealthBar;
	ProgressBar* m_EnemyHealthBar;
	Player& m_Player;
	Enemy* m_Enemy;

	std::vector<SDL_FRect> m_OperandHandDrawRects;
	std::vector<SDL_FRect> m_NumbersHandDrawRects;
	SDL_FRect m_CharacterDrawRect;
	SDL_FRect m_EnemyDrawRect;
	int m_WindowCenterX = 0;
	int m_WindowCenterY = 0;
	int m_WindowHeight = 0;
	int m_WindowWidth = 0;
	bool m_LeftClickDown;
	int m_MouseX;
	int m_MouseY;

	void SetupNewBattle();
	void RenderCharacters(SDL_Renderer& renderer) const;
	void RenderCardHands(SDL_Renderer& renderer) const;
	void CalculateUpdatedDrawPositions(const float& deltaTime);
	void CalculateUpdatedAvatarDrawPositions(const float& deltaTime);
	void CalculateCardHandDrawPositions(const float& deltaTime);
	void CalculateSelectedCardRectPositions(const float& deltaTime);
	void CalculateEquationButtonRectPositions(const float& deltaTime);
	void CheckForClickCollisions();
	bool CheckForValidEquation();
	void AddCardToEquation(Card* card);
	void ClearEquation();
	void ApplyEquation();

	SDL_FRect m_SubmitButtonRect;
	SDL_FRect m_ClearEquationButtonRect;

public:
	BattleScene();
	~BattleScene();

	virtual void OnEnter() override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnExit() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Render(SDL_Renderer& renderer) const override;
};

