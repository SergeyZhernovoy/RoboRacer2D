#include "Sprite.h"

const GLuint Sprite::GetCurrencyFrame()
{
	if (m_isSpriteSheet)
	{
		return m_textures[0];
	}
	else
	{
		return m_textures[m_currentFrame];
	}
}
