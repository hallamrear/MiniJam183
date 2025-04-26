#include "pch.h"
#include "Ghost.h"
#include <Graphics/Texture.h>

Ghost::Ghost()
{
	Texture::LoadPNG("Content/cat2.png", m_Texture);
}

Ghost::~Ghost()
{
}
