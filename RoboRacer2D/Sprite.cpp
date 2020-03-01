#include "Sprite.h"
#include "SOIL.h"

Sprite::Sprite(const GLuint numberTextures)
{
	textures = new GLuint[numberTextures];
	textureIndex = 0;
	currentFrame = 0;
	numberFrames = 0;
	delay = 0.25f;
	elapsed = 0.25f;
	position.x = 0.0f;
	position.y = .0f;
	size.height = .0f;
	size.width = .0f;
	velocity = .0f;
	collideable = true;
	flipHorizontal = false;
	flipVertical = false;
	visible = false;
	active = false;
	spriteSheet = false;
	transparency = true;
	clicked = false;
	collision.left = .0f;
	collision.right = .0f;
	collision.top = .0f;
	collision.bottom = .0f;
	value = 0;
}

Sprite::~Sprite()
{
	for (int i = 0; i < textureIndex; i++)
	{
		glDeleteTextures(1, &textures[i]);
	}
	delete[] textures;
	textures = NULL;
}

void Sprite::Update(const float deltaTime)
{
	float dt = deltaTime;

	if (active)
	{
		elapsed += dt;

		if (elapsed >= delay)
		{
			if (++currentFrame >= numberFrames)
				currentFrame = 0;
			elapsed = 0.0f;
		}
		position.x += velocity * dt;
	}
}

void Sprite::Render()
{
	if (visible)
	{
		if (transparency)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		glBindTexture(GL_TEXTURE_2D, GetCurrentFrame());

		glBegin(GL_QUADS);

		GLfloat x = position.x;
		GLfloat y = position.y;

		GLfloat w = size.width;
		GLfloat h = size.height;

		GLfloat texWidth = (GLfloat)textureIndex / (GLfloat)numberFrames;
		GLfloat texHeight = 1.0f;
		GLfloat u = 0.0f;
		GLfloat v = 0.0f;

		if (textureIndex < numberFrames)
		{
			u = (GLfloat)currentFrame * texWidth;
		}

		glTexCoord2f(u, v);
		glVertex2f(x, y);

		glTexCoord2f(u + texWidth, v);
		glVertex2f(x + w, y);

		glTexCoord2f(u + texWidth, v + texHeight);
		glVertex2f(x + w, y + h);

		glTexCoord2f(u, v + texHeight);
		glVertex2f(x, y + h);

		glEnd();

		if (transparency)
		{
			glDisable(GL_BLEND);
		}

	}
}

const bool Sprite::AddTexture(const char* fileName, const bool useTransparency)
{
	GLuint texture = SOIL_load_OGL_texture(fileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	if (texture == 0)
	{
		return false;
	}

	textures[textureIndex++] = texture;

	if (textureIndex == 1 && numberFrames > 1)
	{
		spriteSheet = true;
	}
	else
	{
		spriteSheet = false;
	}
	transparency = useTransparency;
	return true;
}

const GLuint Sprite::GetCurrentFrame()
{
	if (spriteSheet)
	{
		return textures[0];
	}
	else
	{
		return textures[currentFrame];
	}
}

void Sprite::Jump(SpriteState state)
{
	if (state == SpriteState::DOWN)
	{
		if (position.y < 470.0f) position.y += 75.0f;
	} 
	else if (state == SpriteState::UP)
	{
		if (position.y >= 470.0f) position.y -= 75.0f;
	}

}

const Sprite::Rect Sprite::GetCollisionRect() const
{
	Rect rect;
	rect.left = position.x + collision.left;
	rect.right = position.x + collision.right + size.width;
	rect.top = position.y + collision.top;
	rect.bottom = position.y + collision.top + size.height;
	return rect;
}

const Sprite::Point Sprite::GetCenter() const
{
	Point point;
	point.x = this->GetPosition().x + center.x;
	point.y = this->GetPosition().y + center.y;
	return point;
}

const bool Sprite::IntersectsCircle(const Sprite* sprite) const
{
	if (this->GetCollideable() && sprite->GetCollideable() && this -> GetActive() && sprite->GetActive())
	{
		const Point p1 = this->GetCenter();
		const Point p2 = sprite->GetCenter();
		float y = p2.y - p1.y;
		float x = p2.x - p1.x;
		float d = x * x + y * y;
		float r1 = this->GetRadius() * this->GetRadius();
		float r2 = sprite->GetRadius() * sprite->GetRadius();
		if (d <= r1 + r2)
		{
			return true;
		}
	}
	return false;
}

const bool Sprite::IntersectRect(const Sprite* sprite) const
{
	if (this->GetCollideable() && sprite->GetCollideable() && this -> GetActive() && sprite->GetActive())
	{
		const Rect recta = this->GetCollisionRect();
		const Rect rectb = sprite->GetCollisionRect();
		if (recta.left >= rectb.left && recta.left <= rectb.right && recta.top >= rectb.top && recta.top <= rectb.bottom)
		{
			return true;
		}
		else 
			if (recta.right >= rectb.left && recta.right <= rectb.right && recta.top >= rectb.top && recta.top <= rectb.bottom)
			{
				return true;
			}
			else 
				if (recta.left >= rectb.left && recta.right <= rectb.right && recta.top < rectb.top && recta.bottom > rectb.bottom)
				{
					return true;
				}
				else 
					if (recta.top >= rectb.top && recta.bottom <= rectb.bottom && recta.left < rectb.left && recta.right > rectb.right)
					{
						return true;
					}
					else 
						if (rectb.left >= recta.left && rectb.left <= recta.right && rectb.top >= recta.top && rectb.top <= recta.bottom)
						{
							return true;
						}
						else 
							if (rectb.right >= recta.left && rectb.right <= recta.right && rectb.top >= recta.top && rectb.top <= recta.bottom)
							{
								return true;
							}
							else 
								if (rectb.left >= recta.left && rectb.right <= recta.right && rectb.top < recta.top && rectb.bottom > recta.bottom)
								{
									return true;
								}
								else 
									if (recta.top >= rectb.top && recta.bottom <= rectb.bottom && recta.left < rectb.left && recta.right > rectb.right)
									{
										return true;
									}
									else 
										if (rectb.top >= recta.top && rectb.bottom <= recta.bottom && rectb.left < recta.left && rectb.right > recta.right)
										{
											return true;
										}
	}
	return false;
}








