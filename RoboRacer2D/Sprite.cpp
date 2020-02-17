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
}

Sprite::~Sprite()
{
	delete[] textures;
}

void Sprite::Update(const float deltaTime)
{
	float dt = deltaTime;

	if (active)
	{
		elapsed += dt;

		if (elapsed >= delay)
		{
			currentFrame++;

			if (currentFrame >= numberFrames)
				currentFrame = 0;
			elapsed = 0.0f;
		}
		position.x = position.x + velocity * dt;
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












