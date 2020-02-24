#pragma once
#include <Windows.h>
#include <gl\GL.h>


class Sprite
{
public:

	struct Point
	{
		GLfloat x;
		GLfloat y;
	};
	struct Size
	{
		GLfloat width;
		GLfloat height;
	};
	struct Rect
	{
		GLfloat top;
		GLfloat bottom;
		GLfloat left;
		GLfloat right;
	};

	enum SpriteState
	{
		UP,
		DOWN
	};

protected:

	GLuint* textures;
	GLuint textureIndex;
	
	GLuint currentFrame;
	GLuint numberFrames;
	GLfloat delay;
	GLfloat elapsed;
	
	Point position;
	Size size;
	GLfloat velocity;

	bool collideable;
	bool flipHorizontal;
	bool flipVertical;
	bool visible;
	bool active;
	bool transparency;
	bool spriteSheet;
	bool clicked;

	Rect collision;

	int value;

	float radius;
	Point center;

public:

	Sprite(const GLuint numberTextures);
	~Sprite();

	void Update(const float deltaTime);
	void Render();

	const bool AddTexture(const char* fileName, const bool transparency = true);
	const GLuint GetCurrentFrame();

	void SetPosition(const GLfloat x, const GLfloat y) { position.x = x; position.y = y; }
	void SetPosition(const Point position) { this->position = position; }

	const Point GetPosition() const { return position; }

	const Size GetSize() const { return size; }
	void SetFrameSize(const GLfloat width, const GLfloat height) { size.height = height; size.width = width; }

	void SetNumberOfFrames(const GLuint frames) { numberFrames = frames; }
	
	const bool GetCollideable() const { return collideable; }
	void SetCollideable(const bool value) { collideable = value; }
	
	const bool GetFlipHorizontal() const { return flipHorizontal; }
	void SetFlipHorizontal(const bool value) { flipHorizontal = value; }

	const bool GetFlipVertical() const { return flipVertical; }
	void SetFlipVertical(const bool value) { flipVertical = value; }

	const bool GetActive() const { return active; }
	void SetActive(const bool value) { active = value; }

	const bool GetVisible() const { return visible; }
	void SetVisible(const bool value) { visible = value; }

	const bool GetVelocity() const { return velocity; }
	void SetVelocity(const GLfloat value) { velocity = value; }

	void UseTransparency(const bool value) { transparency = value; }
		  
	void Jump(SpriteState state);
	void SetClicked(const bool value) { clicked = value; }
	const bool GetClicked() const { return clicked; }

	const Rect GetCollisionRect() const;
	void SetCollisionRectOffset(const Rect rect) { collision = rect; }

	const int GetValue() const { return value; }
	void SetValue(const int value) { this->value = value; }

	void SetRadius(const GLfloat radius) { this->radius = radius; }
	const float GetRadius() const { return radius; }

	void SetCenter(const Point center) { this->center = center; }
	const Point GetCenter() const;
	const bool IntersectsCircle(const Sprite* sprite) const;
	const bool IntersectRect(const Sprite* sprite) const;

}; 

