#include "pch.h"
#include "Animation.h"

AnimationDetails::AnimationDetails()
{
	FrameCount = 0;
	IsLooping = false;
	HasFinished = false;
	Duration = 0.0f;
}

AnimationDetails::AnimationDetails(const unsigned int& frameCount, const float& duration, const bool& isLooping, const bool& hasFinished)
{
	FrameCount = frameCount;
	IsLooping = isLooping;
	HasFinished = hasFinished;
	Duration = duration;
}

AnimationDetails::~AnimationDetails()
{
	FrameCount = 0;
	IsLooping = false;
	HasFinished = false;
	Duration = 0.0f;
}

AnimationController::AnimationController(const std::string& sheetPath, const std::vector<AnimationDetails>& animationDetails)
{
	m_AnimationSheet = nullptr;
	Texture::LoadPNG(sheetPath.c_str(), m_AnimationSheet);
	m_AnimationDetails = animationDetails;
	m_AnimationCount = m_AnimationDetails.size();
	Texture::QueryTexture(m_AnimationSheet, m_TextureWidth, m_TextureHeight);
	m_CurrentAnimationIndex = 0;
	m_CurrentFrame = 0;
	m_TimeElapsed = 0.0f;
	m_FrameSizeX = 0;
	m_FrameSizeY = 0;
	m_TimeBetweenFrames = m_AnimationDetails[m_CurrentAnimationIndex].Duration / m_AnimationDetails[m_CurrentAnimationIndex].FrameCount;
}

AnimationController::~AnimationController()
{
	m_AnimationSheet = nullptr;

	if (m_AnimationSheet)
	{
		SDL_DestroyTexture(m_AnimationSheet);
		m_AnimationSheet = nullptr;
	}
}

SDL_Texture* AnimationController::GetSpriteSheet() const
{
	return m_AnimationSheet;
}

bool AnimationController::HasFinished()
{
	m_AnimationDetails[m_CurrentAnimationIndex].HasFinished = false;
}

void AnimationController::Start()
{
	m_AnimationDetails[m_CurrentAnimationIndex].HasFinished = false;
	m_CurrentFrame = 0;
	m_TimeElapsed = 0.0f;
}

void AnimationController::SetAnimationId(const unsigned int& animation)
{
	m_CurrentAnimationIndex = animation;
	m_CurrentFrame = 0;
	m_TimeElapsed = 0.0f;
	m_TimeBetweenFrames = m_AnimationDetails[m_CurrentAnimationIndex].Duration / m_AnimationDetails[m_CurrentAnimationIndex].FrameCount;
}

const unsigned int& AnimationController::GetCurrentAnimationId() const
{
	return m_CurrentAnimationIndex;
}

const AnimationDetails& AnimationController::GetCurrentAnimationDetails() const
{
	return m_AnimationDetails[m_CurrentAnimationIndex];
}

const int& AnimationController::GetFrameSizeX() const
{
	return m_FrameSizeX;
}

const int& AnimationController::GetFrameSizeY() const
{
	return m_FrameSizeY;
}

void AnimationController::Update(float DeltaTime)
{
	if (m_AnimationSheet != nullptr)
	{
		AnimationDetails& currentAnimation = m_AnimationDetails[m_CurrentAnimationIndex];
		m_FrameSizeX = m_TextureWidth / (float)(currentAnimation.FrameCount);
		m_FrameSizeY = m_TextureHeight / (float)(m_AnimationCount);

		if (currentAnimation.HasFinished == false)
		{
			m_TimeElapsed += DeltaTime;

			if (m_TimeElapsed > currentAnimation.Duration)
			{
				if (currentAnimation.IsLooping)
				{
					m_TimeElapsed = 0.0f;
					m_CurrentFrame = 0;
				}
				else
				{
					m_TimeElapsed = 0.0f;
					m_CurrentFrame = 0;
					currentAnimation.HasFinished = true;
				}
			}
			else
			{
				m_CurrentFrame = (unsigned int)(trunc(m_TimeElapsed / m_TimeBetweenFrames));
			}
		}
	}
}

void AnimationController::Render(SDL_Renderer& renderer, const SDL_FRect& dstRect, const bool& flipped)
{
	if (m_AnimationSheet != nullptr)
	{
		SDL_FRect srcRect;
		srcRect.x = ((int)m_FrameSizeX * m_CurrentFrame);
		srcRect.y = ((int)m_FrameSizeY * m_CurrentAnimationIndex);
		srcRect.w = (int)m_FrameSizeX;
		srcRect.h = (int)m_FrameSizeY;

		SDL_FlipMode flip = SDL_FLIP_NONE;

		if (flipped)
		{
			flip = SDL_FLIP_HORIZONTAL;
		}

		SDL_RenderTextureRotated(&renderer, m_AnimationSheet, &srcRect, &dstRect, 0.0F, nullptr, flip);
		
	}
}