// RoboRacer2D.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "RoboRacer2D.h"
#include <Windows.h>

#include <gl\GL.h>
#include <gl\GLU.h>
#include "glut.h"

#include "Sprite.h"
#include "Input.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

HDC hDC = NULL;
HGLRC hRC = NULL;
HWND hWnd = NULL;

bool fullscreen = false;
GLfloat screenHeight;
GLfloat screenWidth;

Sprite* robotLeft;
Sprite* robotRight;
Sprite* robotRightStrip;
Sprite* robotLeftStrip;
Sprite* background;
Sprite* player;
Sprite* pauseButton;
Sprite* resumeButton;

Input* inputManager;

float uiTimer;
const float UI_THRESHOLD = .2f;

enum GameState
{
	GS_Runnig,
	GS_Paused
} gameState;

void ReSizeGLScene(const GLsizei width, const GLsizei height)
{
	GLsizei h = height;
	GLsizei w = width;
	if (h == 0) h = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

const bool InitGL()
{
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glDisable(GL_DEPTH_TEST);
	return true;
}

const bool LoadTextures()
{
    background = new Sprite(1);
    background->SetFrameSize(1877.0f, 600.0f);
    background->SetNumberOfFrames(1);
    background->AddTexture("resources/background.png", false);
   
    robotLeft = new Sprite(4);
    robotLeft->SetFrameSize(100.0f, 125.0f);
    robotLeft->SetNumberOfFrames(4);
    robotLeft->SetPosition(0, screenHeight - 130.0f);
    robotLeft->AddTexture("resources/robot_left_00.png");
    robotLeft->AddTexture("resources/robot_left_01.png");
    robotLeft->AddTexture("resources/robot_left_02.png");
    robotLeft->AddTexture("resources/robot_left_03.png");

    robotRight = new Sprite(4);
    robotRight->SetFrameSize(100.0f, 125.0f);
    robotRight->SetNumberOfFrames(4);
    robotRight->SetPosition(0, screenHeight - 130.0f);
    robotRight->AddTexture("resources/robot_right_00.png");
    robotRight->AddTexture("resources/robot_right_01.png");
    robotRight->AddTexture("resources/robot_right_02.png");
    robotRight->AddTexture("resources/robot_right_03.png");

    robotRightStrip = new Sprite(1);
    robotRightStrip->SetFrameSize(125.0f, 100.0f);
    robotRightStrip->SetNumberOfFrames(1);
    robotRightStrip->SetPosition(0, screenHeight - 130.0f);
    robotRightStrip->AddTexture("resources/robot_right_strip.png");

    robotLeftStrip = new Sprite(1);
    robotLeftStrip->SetFrameSize(125.0f, 100.0f);
    robotLeftStrip->SetNumberOfFrames(1);
    robotLeftStrip->SetPosition(0, screenHeight - 130.0f);
    robotLeftStrip->AddTexture("resources/robot_left_strip.png");

	pauseButton = new Sprite(1);
	pauseButton->SetFrameSize(75.0f, 38.0f);
	pauseButton->SetNumberOfFrames(1);
	pauseButton->SetPosition(5.0f, 5.0f);
	pauseButton->AddTexture("resources/pauseButton.png");
	pauseButton->SetActive(true);
	pauseButton->SetVisible(true);

	resumeButton = new Sprite(1);
	resumeButton->SetFrameSize(75.0f, 38.0f);
	resumeButton->SetNumberOfFrames(1);
	resumeButton->SetPosition(80.0f, 5.0f);
	resumeButton->AddTexture("resources/resumeButton.png");
	resumeButton->SetActive(false);
	resumeButton->SetVisible(false);

	inputManager->AddUIElement(resumeButton);
	inputManager->AddUIElement(pauseButton);

	background->SetVisible(true);
	background->SetActive(true);
	

    robotRight->SetActive(true);
    robotRight->SetVisible(true);
    robotRight->SetVelocity(50.0f);

    player = robotRight;
	player->SetActive(true);
    player->SetVisible(true);
    
    return true;
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    background->Render();
    robotLeft->Render();
    robotRight->Render();
    robotLeftStrip->Render();
    robotRightStrip->Render();
	pauseButton->Render();
	resumeButton->Render();

    SwapBuffers(hDC);
}

void StartGame()
{
	gameState = GS_Runnig;
	uiTimer = .0f;
    inputManager = new Input(hWnd);
    LoadTextures();
}

void ProcessInput(const float deltaTime)
{
	Input::Command command = inputManager->GetCommand();

	if (gameState == GS_Paused) 
	{ 
		command = Input::Command::CM_UI; 
	}
		
	uiTimer += deltaTime;
	if (uiTimer > UI_THRESHOLD)
	{
		uiTimer = 0.0f;
		switch (command)
		{
			case Input::Command::CM_STOP:
				player->SetVelocity(0.0f);
				background->SetVelocity(0.0f);
				break;

			case Input::Command::CM_LEFT:

				if (player == robotRight)
				{
					robotRight->SetActive(false);
					robotRight->SetVisible(false);
					robotRight->SetPosition(robotRight->GetPosition());
				}

				player = robotLeft;
				player->SetActive(true);
				player->SetVisible(true);
				player->SetVelocity(-50.0f);
				background->SetVelocity(50.0f);
				break;

			case Input::Command::CM_RIGHT:
				if (player == robotLeft)
				{
					robotLeft->SetActive(false);
					robotLeft->SetVisible(false);
					robotLeft->SetPosition(robotLeft->GetPosition());
				}

				player = robotRight;
				player->SetActive(true);
				player->SetVisible(true);
				player->SetVelocity(50.0f);
				background->SetVelocity(-50.0f);
				break;

			case Input::Command::CM_UP:
				player->Jump(Sprite::SpriteState::UP);
				break;

			case Input::Command::CM_DOWN:
				player->Jump(Sprite::SpriteState::DOWN);
				break;

			case Input::Command::CM_QUIT:
				PostQuitMessage(0);
				break;
			case Input::Command::CM_UI:
				if (pauseButton->GetClicked())
				{
					pauseButton->SetClicked(false);
					pauseButton->SetVisible(false);
					pauseButton->SetActive(false);

					resumeButton->SetVisible(true);
					resumeButton->SetActive(true);
					gameState = GS_Paused;
				}

				if (resumeButton->GetClicked())
				{
					resumeButton->SetClicked(false);
					resumeButton->SetVisible(false);
					resumeButton->SetActive(false);

					pauseButton->SetVisible(true);
					pauseButton->SetActive(true);
					gameState = GS_Runnig;
				}
			}
	}


  
}


void Update(const float deltaTime)
{
    inputManager->Update(deltaTime);
    ProcessInput(deltaTime);
	if (gameState == GS_Runnig)
	{
		background->Update(deltaTime);
		robotLeft->Update(deltaTime);
		robotRight->Update(deltaTime);
		robotLeftStrip->Update(deltaTime);
		robotRightStrip->Update(deltaTime);

		pauseButton->Update(deltaTime);
		resumeButton->Update(deltaTime);
	}
}

void GameLoop(const float deltaTime)
{
    Update(deltaTime);
    Render();
}

void EndGame()
{}

const bool CreateGLWindow(const char* p_title, const int p_width, const int p_height, const int p_bits, const bool p_fullscreenflag)
{
	GLuint		PixelFormat;
	WNDCLASS	wc;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		WindowRect;
	WindowRect.left = (long)0;
	WindowRect.right = (long)p_width;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)p_height;
	fullscreen = p_fullscreenflag;
	screenHeight = (GLfloat)p_height;
	screenWidth = (GLfloat)p_width;
	hInst = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL";
	if (!RegisterClass(&wc))
	{
		return false;
	}
	if (fullscreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = p_width;
		dmScreenSettings.dmPelsHeight = p_height;
		dmScreenSettings.dmBitsPerPel = p_bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			fullscreen = false;
		}
	}
	if (fullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(false);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	if (!(hWnd = CreateWindowEx(dwExStyle,
        "OpenGL",
		p_title,
		dwStyle |
		WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN,
		0, 0,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		NULL,
		NULL,
		hInst,
		NULL)))
	{
		return false;
	}
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		p_bits,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		16, 0, 0,
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};
	if (!(hDC = GetDC(hWnd)))
	{
		return false;
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		return false;
	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		return false;
	}
	if (!(hRC = wglCreateContext(hDC)))
	{
		return false;
	}
	if (!wglMakeCurrent(hDC, hRC))
	{
		return false;
	}
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	ReSizeGLScene(p_width, p_height);
	if (!InitGL())
	{
		return false;
	}
	return true;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ROBORACER2D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!CreateGLWindow("RoboRacer 2D",800, 600, 16, false))
    {
        return false;
    }


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ROBORACER2D));

    MSG msg;

    StartGame();
    int previousTime = glutGet(GLUT_ELAPSED_TIME);
    bool done = false;
    while (!done) 
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                done = true;
            }
            else 
            {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
            }
        }
		else
		{
            int currentTime = glutGet(GLUT_ELAPSED_TIME);
            float deltaTime = (float)(currentTime - previousTime) / 1000;
            previousTime = currentTime;
			GameLoop(deltaTime);
		}
    }
    EndGame();
    
    return (int) msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ROBORACER2D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ROBORACER2D);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

