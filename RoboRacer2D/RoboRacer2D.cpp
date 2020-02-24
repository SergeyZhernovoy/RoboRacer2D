// RoboRacer2D.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "RoboRacer2D.h"
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

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

Sprite* pickup;
Sprite* enemy;

Input* inputManager;

float uiTimer;
const float UI_THRESHOLD = .2f;
float pickupSpawnThreshold;
float pickupSpawnTimer;
float enemySpawnThreshold;
float enemySpawnTimer;

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
	Sprite::Point center;
	float radius;

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

	pickup = new Sprite(1);
	pickup->SetFrameSize(26.0f, 50.0f);
	pickup->SetNumberOfFrames(1);
	pickup->AddTexture("resources/oil.png");
	pickup->SetVisible(false);
	pickup->SetActive(false);
	pickup->SetValue(50);

	enemy = new Sprite(1);
	enemy->SetFrameSize(32.0f, 50.0f);
	enemy->SetNumberOfFrames(1);
	enemy->AddTexture("resources/water.png");
	enemy->SetVisible(false);
	enemy->SetActive(false);
	enemy->SetValue(-50);

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
    
	Sprite::Rect collision;
	collision.left = 34.0f;
	collision.right = -10.0f;
	collision.top = 0.0f;
	collision.bottom = 0.0f;
	robotLeft->SetCollisionRectOffset(collision);
	robotRight->SetCollisionRectOffset(collision);
	
	center.x = robotRight->GetSize().width / 2.0f;
	center.y = robotRight->GetSize().height / 2.0f;
	radius = (center.x + center.y) / 2.0f;

	robotRight->SetCenter(center);
	robotRight->SetRadius(radius);
	robotLeft->SetCenter(center);
	robotLeft->SetRadius(radius);

	center.x = pickup->GetSize().width / 2.0f;
	float yOffset = (pickup->GetSize().height / 4.0f) * 3.0f;
	center.y = yOffset;
	pickup->SetCenter(center);
	radius = pickup->GetSize().width / 2.0f;
	pickup->SetRadius(radius);

	robotLeft->SetCollideable(true);
	robotRight->SetCollideable(true);
	pickup->SetCollideable(true);
	enemy->SetCollideable(true);

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
	pickup->Render();
	enemy->Render();
    SwapBuffers(hDC);
}

void SpawnPickup(float deltaTime)
{
	if (pickup->GetVisible() == false)
	{
		pickupSpawnTimer += deltaTime;
		if (pickupSpawnTimer > pickupSpawnThreshold)
		{
			float marginX = pickup->GetSize().width;
			float marginY = pickup->GetSize().height;
			float spawnX = (rand() % (int)(screenWidth - (marginX * 2))) + marginX;
			float spawnY = screenHeight - ((rand() % (int)(player -> GetSize().height - (marginY * 1.5))) + marginY);
			pickup->SetPosition(spawnX, spawnY);
			pickup->SetVisible(true);
			pickup->SetActive(true);
			pickupSpawnTimer = 0.0f;
		}
	}
}

void SpawnEnemy(float deltaTime)
{
	if (enemy->GetVisible() == false)
	{
		enemySpawnTimer += deltaTime;
		if (enemySpawnTimer > enemySpawnThreshold)
		{
			float marginX = enemy->GetSize().width;
			float marginY = enemy->GetSize().height;
			float spawnX = (rand() % (int)(screenWidth - (marginX * 2))) + marginX;
			float spawnY = screenHeight - ((rand() % (int)(player->GetSize().height - (marginY * 1.5))) + marginY);
			enemy->SetPosition(spawnX, spawnY);
			enemy->SetVisible(true);
			enemy->SetActive(true);
		}
	}
}

void StartGame()
{
	gameState = GameState::GS_Runnig;
	uiTimer = .0f;
    inputManager = new Input(hWnd);
    LoadTextures();
	srand(time(NULL));
	pickupSpawnThreshold  = 15.0f;
	pickupSpawnTimer = 0.0f;
	enemySpawnThreshold = 7.0f;
	enemySpawnTimer = 0.0f;
}

void CheckCollisions()
{
	if (player->IntersectsCircle(pickup))
	{
		pickup->SetVisible(false);
		pickup->SetActive(false);
		player->SetValue(player->GetValue() + pickup->GetValue());
		pickupSpawnThreshold = 0.0f;
	}

	if (player->IntersectRect(enemy))
	{
		enemy->SetVisible(false);
		enemy->SetActive(false);
		player->SetValue(player->GetValue() + enemy->GetValue());
		enemySpawnTimer = 0.0f;
	}

}

void CheckBackground()
{
	float leftThreshold = 0.0f;
	float rightThreshold = -(background->GetSize().width - screenWidth);
	if (background->GetPosition().x > 0)
	{
		background->SetPosition(0.0f, background->GetPosition().y);
	}
	else if (background->GetPosition().x < rightThreshold)
	{
		background->SetPosition(rightThreshold, background -> GetPosition().y);
	}
}

void CheckBoundries(Sprite* sprite)
{
	Sprite::Rect check = sprite->GetCollisionRect();
	float offset;
	float x;
	float y;
	if (check.left < 0.0f)
	{
		sprite->SetVelocity(.0f);
		offset = check.left;
		x = sprite->GetPosition().x - offset;
		y = sprite->GetPosition().y;
		sprite->SetPosition(x, y);
	}
	else
	{
		if (check.right > screenWidth)
		{
			sprite->SetVelocity(.0f);
			offset = screenWidth - check.right;
			x = sprite->GetPosition().x + offset;
			y = sprite->GetPosition().y;
			sprite->SetPosition(x, y);
		}
	}

	if (check.top < 0.0f)
	{
		sprite->SetVelocity(.0f);
		offset = check.top;
		x = sprite->GetPosition().y - offset;
		y = sprite->GetPosition().x;
		sprite->SetPosition(x, y);
	}
	else
	{
		if (check.bottom > screenHeight)
		{
			sprite->SetVelocity(.0f);
			offset = screenHeight - check.bottom;
			x = sprite->GetPosition().y + offset;
			y = sprite->GetPosition().x;
			sprite->SetPosition(x, y);
		}
	}

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
					robotLeft->SetPosition(robotRight->GetPosition());
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
					robotRight->SetPosition(robotLeft->GetPosition());
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
	CheckBoundries(player);
	CheckBackground();
	if (gameState == GS_Runnig)
	{
		background->Update(deltaTime);
		robotLeft->Update(deltaTime);
		robotRight->Update(deltaTime);
		robotLeftStrip->Update(deltaTime);
		robotRightStrip->Update(deltaTime);

		pauseButton->Update(deltaTime);
		resumeButton->Update(deltaTime);

		pickup->Update(deltaTime);
		SpawnPickup(deltaTime);

		enemy->Update(deltaTime);
		SpawnEnemy(deltaTime);

		CheckCollisions();
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

