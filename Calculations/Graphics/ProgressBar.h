#pragma once
class ProgressBar
{
private:
	SDL_FRect m_Dimensions;
	SDL_FColor m_BackgroundColour;
	SDL_FColor m_FilledColour;
	float m_FillLevel;
	bool m_IsHorizontal;
	
public:
	ProgressBar(const SDL_FRect& dimensions, const float& fillLevel = 1.0f, const bool& isHorizontal = true);
	~ProgressBar();

	void SetBackgroundColour(const SDL_FColor& colour);
	void SetFilledColour(const SDL_FColor& colour);
	/// <summary>
	/// Sets the fill amount of the progress bar.
	/// </summary>
	/// <param name="progressLevel">float value representing the fill amount, scaled from 0-1</param>
	void SetProgressValue(const float& progressLevel);

	const SDL_FRect& GetDimensions() const;
	const void SetDimensions(const SDL_FRect& dimensions);

	void Update(const float& deltaTime);
	void Render(SDL_Renderer& renderer) const;
};

