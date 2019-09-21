#pragma once
#include <gl\gl.h>

typedef unsigned int uint32;

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

protected: 
	/* ћассив, который будет содержать указатели на текстуры этого спрайта*/
	GLuint* m_textures;
	/* »ндекс текстуры - начинаетс€ с нул€ и увеличиваетс€ каждый раз когда текстура добавл€етс€ к спрайту */
	uint32 m_textureIndex;
	/* Ќачинаетс€ с нул€ и увеличиваетс€ каждый раз когда надо сделать кадр анимированным*/
	uint32 m_currentFrame;
	/* ’ранит общее число кадров дл€ анимации*/
	uint32 m_numberOfFrames;
	/* „исло секунд, которые проход€т между кадрами анимации, позвол€ет контролировать скорость анимации*/
	GLfloat m_animationDelay;
	/*  оличество времени которое прошло с кадра анимации*/
	GLfloat m_animationElapsed;
	/* ѕозици€ спрайта*/
	Point m_position;
	/* –азмеры спрайта*/
	Size m_size;
	/* —корость спрайта*/
	GLfloat m_velocity;
	/* ‘лаг говорит, что спрайт может или не может быть участников столкновений с другими спрайтами*/
	bool m_isCollideable;
	/* ‘лаг говорит, что изображение должно быть повернуто горизонтально*/
	bool m_flipHorizontal;
	/* ‘лаг говорит, что изображение должно быть повернуто вертикально*/
	bool m_flipVertical;
	/* ‘лаг говорит, что спрайт виден в игре*/
	bool m_isVisible;
	/* ‘лаг говорит, что спрайт активен. ≈сли флаг сброшен в ложь, то у спрайта анимаци€ и позици€ не будет измен€тьс€*/
	bool m_isActive;
	/* ‘лаг говорит, что изображение должно использовать прозрачность*/
	bool m_useTransparency;
	/* ‘лаг говори, что изображение это одна текстура*/
	bool m_isSpriteSheet;

public:
	Sprite(const uint32 m_pNUmberOfTextures);
	~Sprite();

	void Update(const float p_deltaTime);
	void Render();

	const bool AddTexture(const char* p_fileName, const bool p_useTransparency = true);
	const GLuint GetCurrencyFrame();
	
	const Point GetPosition() const { return m_position; }
	void SetPosition(const GLfloat p_x, const GLfloat p_y) { m_position.x = p_x; m_position.y = p_y; }
	void SetPosition(const Point p_position) { m_position = p_position; }
	void SetFrameSize(const GLfloat p_width, const GLfloat p_height) { m_size.width = p_width; m_size.height = p_height; }
	void SetVelocity(const GLfloat p_velocity) 	{ m_velocity = p_velocity; }
	void SetNumberOfFrames(const unsigned int p_frames) { m_numberOfFrames = p_frames; }
	const bool isCollideable() const { return m_isCollideable; }
	void IsCollideable(const bool p_value) { m_isCollideable = p_value; }
	void FlipHorizontal(const bool p_value) { m_flipHorizontal = p_value; }
	void FlipVertical(const bool p_value) { m_flipVertical = p_value; }
	void IsActive(const bool p_value) { m_isActive = p_value; }
	const bool IsActive() const { return m_isActive; }
	void IsVisible(const bool p_value) { m_isVisible = p_value; }
	const bool IsVisible() const { return m_isVisible; }
	void UseTransparency(const bool p_value) { m_useTransparency = p_value; }
};