#include "pch.h"
#include "Animation.h"

Animation::Animation(const std::string& sheetPath, const unsigned int& numberOfAnimations, const unsigned int& frameCount, const float& duration, const bool& looping)
{
	m_AnimationSheet = nullptr;
	Texture::LoadPNG(sheetPath.c_str(), m_AnimationSheet);
	m_TimeElapsed = 0.0f;
	m_IsLooping = looping;
	m_Duration = duration;
	m_TotalFrames = frameCount;
	m_TimeBetweenFrames = m_Duration / (float)(m_TotalFrames);

	float textureWidth  = 0.0f;
	float textureHeight = 0.0f;
	Texture::QueryTexture(m_AnimationSheet, textureWidth, textureHeight);
	m_FrameSizeX = textureWidth / (float)m_TotalFrames;
	m_FrameSizeY = textureHeight / (float)numberOfAnimations;
	m_HasFinished = false;
}

Animation::~Animation()
{
	m_AnimationSheet = nullptr;

	if (m_AnimationSheet)
	{
		SDL_DestroyTexture(m_AnimationSheet);
		m_AnimationSheet = nullptr;
	}
}

bool Animation::HasFinished()
{
	return m_HasFinished;
}

void Animation::Start()
{
	m_HasFinished = false;
	m_CurrentFrame = 0;
	m_TimeElapsed = 0.0f;
}

void Animation::SetAnimation(unsigned int animation)
{
	m_CurrentAnimation = animation;
}

const unsigned int Animation::GetCurrentAnimationId()
{
	return m_CurrentAnimation;
}

const int& Animation::GetFrameSizeX() const
{
	return m_FrameSizeX;
}

const int& Animation::GetFrameSizeY() const
{
	return m_FrameSizeY;
}

void Animation::Update(float DeltaTime)
{
	if (m_AnimationSheet)
	{
		if (m_HasFinished == false)
		{
			m_TimeElapsed += DeltaTime;

			if (m_TimeElapsed > m_Duration)
			{
				if (m_IsLooping)
				{
					m_TimeElapsed = 0.0f;
					m_CurrentFrame = 0;
				}
				else
				{
					m_TimeElapsed = 0.0f;
					m_CurrentFrame = 0;
					m_HasFinished = true;
				}
			}
			else
			{
				m_CurrentFrame = (unsigned int)(trunc(m_TimeElapsed / m_TimeBetweenFrames));
			}
		}
	}
}

void Animation::Render(SDL_Renderer& renderer, const SDL_FRect& dstRect, const bool& flipped)
{
	if (m_AnimationSheet != nullptr)
	{
		SDL_FRect srcRect;
		srcRect.x = ((int)m_FrameSizeX * m_CurrentFrame);
		srcRect.y = ((int)m_FrameSizeY * m_CurrentAnimation);
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
