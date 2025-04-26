#include "pch.h"
#include "ProgressBar.h"

ProgressBar::ProgressBar(const SDL_FRect& dimensions, const float& fillLevel, const bool& isHorizontal)
{
	m_BackgroundColour.r = 1.0f;
	m_BackgroundColour.g = 0.0f;
	m_BackgroundColour.b = 0.0f;
	m_BackgroundColour.a = 1.0f;

	m_FilledColour.r = 0.0f;
	m_FilledColour.g = 1.0f;
	m_FilledColour.b = 0.0f;
	m_FilledColour.a = 1.0f;

	m_Dimensions = dimensions;
	m_IsHorizontal = isHorizontal;

	SetProgressValue(fillLevel);
}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::SetBackgroundColour(const SDL_FColor& colour)
{
	m_BackgroundColour = colour;
}

void ProgressBar::SetFilledColour(const SDL_FColor& colour)
{
	m_FilledColour = colour;
}

void ProgressBar::SetProgressValue(const float& progressLevel)
{
	m_FillLevel = progressLevel;
	m_FillLevel = SDL_max(m_FillLevel, 0.0f);
	m_FillLevel = SDL_min(m_FillLevel, 1.0f);
}

const SDL_FRect& ProgressBar::GetDimensions() const
{
	return m_Dimensions;
}

void ProgressBar::Update(const float& deltaTime)
{
	
}

void ProgressBar::Render(SDL_Renderer& renderer) const
{
	//Draw Background
	SDL_SetRenderDrawColorFloat(&renderer, m_BackgroundColour.r, m_BackgroundColour.g, m_BackgroundColour.b, m_BackgroundColour.a);
	SDL_RenderFillRect(&renderer, &m_Dimensions);

	//Draw Foreground
	SDL_FRect fillRect = m_Dimensions;
	
	if (m_IsHorizontal)
	{
		fillRect.w *= m_FillLevel;
	}
	else
	{
		fillRect.h *= m_FillLevel;
	}

	SDL_SetRenderDrawColorFloat(&renderer, m_FilledColour.r, m_FilledColour.g, m_FilledColour.b, m_FilledColour.a);
	SDL_RenderFillRect(&renderer, &fillRect);
}
