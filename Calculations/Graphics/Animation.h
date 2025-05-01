#pragma once
#include "Texture.h"

struct AnimationDetails
{
	unsigned int FrameCount;
	bool IsLooping;
	bool HasFinished;
	float Duration;

	AnimationDetails();
	AnimationDetails(const unsigned int& frameCount, const float& duration, const bool& isLooping, const bool& hasFinished = false);
	~AnimationDetails();
};

class AnimationController
{
	std::vector<AnimationDetails> m_AnimationDetails;
	float m_TimeBetweenFrames;
	float m_TimeElapsed;
	unsigned int m_CurrentFrame;
	SDL_Texture* m_AnimationSheet;
	unsigned int m_CurrentAnimationIndex;
	unsigned int m_AnimationCount;
	int m_FrameSizeX;
	int m_FrameSizeY;
	float m_TextureWidth = 0.0f;
	float m_TextureHeight = 0.0f;

public:

	AnimationController(const std::string& sheetPath, const std::vector<AnimationDetails>& animationDetails);
	~AnimationController();

	SDL_Texture* GetSpriteSheet() const;

	bool HasFinished();
	void Start();
	void SetAnimationId(const unsigned int& animation);
	const unsigned int& GetCurrentAnimationId() const;
	const AnimationDetails& GetCurrentAnimationDetails() const;
	const int& GetFrameSizeX() const;
	const int& GetFrameSizeY() const;
	void Update(float DeltaTime);
	void Render(SDL_Renderer& renderer, const SDL_FRect& dstRect, const bool& flipped);
};