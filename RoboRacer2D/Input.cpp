#include "Input.h"

Input::Input(const HWND hWnd)
{
	command = Command::CM_STOP;
	this->hWnd = hWnd;
	uiElements = new Sprite * [10];
	uiCount = 0;
}

Input::~Input()
{}

void Input::Update(const float deltaTime)
{
	for (unsigned int i = 0; i < uiCount; i++)
	{
		Sprite* element = uiElements[i];
		if (element->GetActive() == true)
		{
			if (CheckForClick(element))
			{
				element->SetClicked(true);
				command = Input::Command::CM_UI;
				return;
			}
		}
	}

	command = Command::CM_STOP;
	
	if (KEYDOWN(Key::K_LEFT) || KEYDOWN(Key::K_A))
	{
		command = Command::CM_LEFT;
	}

	if (KEYDOWN(Key::K_RIGHT) || KEYDOWN(Key::K_D))
	{
		command = Command::CM_RIGHT;
	}

	if (KEYDOWN(Key::K_UP) || KEYDOWN(Key::K_LB) || KEYDOWN(Key::K_W))
	{
		command = Command::CM_UP;
	}

	if (KEYDOWN(Key::K_DOWN) || KEYDOWN(Key::K_RB) || KEYDOWN(Key::K_S))
	{
		command = Command::CM_DOWN;
	}

	if (KEYDOWN(Key::K_ESC) || KEYDOWN(Key::K_Q))
	{
		command = Command::CM_QUIT;
	}
}

void Input::AddUIElement(Sprite* element)
{
	uiElements[uiCount++] = element;
}

const bool Input::CheckForClick(Sprite* element) const
{
	if (KEYDOWN(Key::K_LB))
	{
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);
		ScreenToClient(hWnd, &cursorPosition);
		float left = element->GetPosition().x;
		float right = element->GetPosition().x + element->GetSize().width;

		float top = element->GetPosition().y;
		float bottom = element->GetPosition().y + element->GetSize().height;

		if (cursorPosition.x >= left && cursorPosition.x <= right && cursorPosition.y >= top && cursorPosition.y <= bottom)
		{
			return true;
		}
		else
		{
			return false;
		}
		return false;
	}
}
