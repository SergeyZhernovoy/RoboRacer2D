#pragma once
#include <Windows.h>
#include "Sprite.h"
class Input
{
public:
	enum Key
	{
		K_ESC = VK_ESCAPE,
		K_SPACE = VK_SPACE,
		K_LEFT = VK_LEFT,
		K_RIGHT = VK_RIGHT,
		K_UP = VK_UP,
		K_DOWN = VK_DOWN,
		K_W = 87,
		K_A = 65,
		K_S = 83,
		K_D = 68,
		K_Q = 81,
		K_ENTER = VK_RETURN,
		K_LB = VK_LBUTTON,
		K_RB = VK_RBUTTON

	};

	enum Command
	{
		CM_INVALID,
		CM_LEFT,
		CM_RIGHT,
		CM_STOP,
		CM_UP,
		CM_DOWN,
		CM_QUIT,
		CM_UI
	};

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

private:
	Sprite** uiElements;
	unsigned int uiCount;
protected:
	Command command;
	HWND hWnd;

public:

	Input(const HWND hWnd);
	~Input();

	void Update(const float deltaTime);

	const Command GetCommand() const { return command; }

	void AddUIElement(Sprite* element);
	const bool CheckForClick(Sprite* element) const;

};

