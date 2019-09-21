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
	/* ������, ������� ����� ��������� ��������� �� �������� ����� �������*/
	GLuint* m_textures;
	/* ������ �������� - ���������� � ���� � ������������� ������ ��� ����� �������� ����������� � ������� */
	uint32 m_textureIndex;
	/* ���������� � ���� � ������������� ������ ��� ����� ���� ������� ���� �������������*/
	uint32 m_currentFrame;
	/* ������ ����� ����� ������ ��� ��������*/
	uint32 m_numberOfFrames;
	/* ����� ������, ������� �������� ����� ������� ��������, ��������� �������������� �������� ��������*/
	GLfloat m_animationDelay;
	/* ���������� ������� ������� ������ � ����� ��������*/
	GLfloat m_animationElapsed;
	/* ������� �������*/
	Point m_position;
	/* ������� �������*/
	Size m_size;
	/* �������� �������*/
	GLfloat m_velocity;
	/* ���� �������, ��� ������ ����� ��� �� ����� ���� ���������� ������������ � ������� ���������*/
	bool m_isCollideable;
	/* ���� �������, ��� ����������� ������ ���� ��������� �������������*/
	bool m_flipHorizontal;
	/* ���� �������, ��� ����������� ������ ���� ��������� �����������*/
	bool m_flipVertical;
	/* ���� �������, ��� ������ ����� � ����*/
	bool m_isVisible;
	/* ���� �������, ��� ������ �������. ���� ���� ������� � ����, �� � ������� �������� � ������� �� ����� ����������*/
	bool m_isActive;
	/* ���� �������, ��� ����������� ������ ������������ ������������*/
	bool m_useTransparency;
	/* ���� ������, ��� ����������� ��� ���� ��������*/
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