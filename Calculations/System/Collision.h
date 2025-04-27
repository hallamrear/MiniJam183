#pragma once

namespace Collision
{
	inline static const bool PointInRect(const int& X, const int& Y, const SDL_FRect& rect)
	{
		//Check if the point is less than max and greater than min
		return (X > rect.x && X < (rect.x + rect.w) && Y > rect.y && Y < (rect.y + rect.h));
	}
};

