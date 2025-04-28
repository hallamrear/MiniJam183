#pragma once

#include "Texture.h"

class Animation
{
	float m_TimeBetweenFrames;
	float m_TimeElapsed;
	float m_Duration;
	unsigned int m_CurrentFrame;
	SDL_Texture* m_AnimationSheet;
	bool m_IsLooping;
	unsigned int m_CurrentAnimation;
	unsigned int m_TotalFrames;

	//If the animation does not loop, this will set to true when it has finished playing.
	bool m_HasFinished;
	int m_FrameSizeX;
	int m_FrameSizeY;

public:

	Animation(const std::string& sheetPath, const unsigned int& numberOfAnimations, const unsigned int& frameCount, const float& duration, const bool& looping);
	~Animation();

	bool HasFinished();
	void Start();
	void SetAnimation(unsigned int animation);
	const unsigned int GetCurrentAnimationId();
	const int& GetFrameSizeX() const;
	const int& GetFrameSizeY() const;
	void Update(float DeltaTime);
	void Render(SDL_Renderer& renderer, const SDL_FRect& dstRect, const bool& flipped);
};