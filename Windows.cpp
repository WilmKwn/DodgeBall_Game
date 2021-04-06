#include <windows.h>
#include <stdio.h>
#include <windowsx.h>
#include <math.h>
#include <Windows.h>
#include <stdlib.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define FILE_OPTIONS 4

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44
#define VK_F 0x46

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);

HMENU hMenu;

HWND main_window_handle = NULL;
HWND hWnd;

PAINTSTRUCT ps;
HDC hdc;

HBRUSH red_brush = CreateSolidBrush(RGB(0, 225, 0));

HPEN yellow_pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 0));
HPEN green_pen = CreatePen(PS_SOLID, 0, RGB(0, 255, 0));
HPEN black_pen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
HPEN red_pen = CreatePen(PS_SOLID, 0, RGB(225, 0, 0));

char buffer0[80];
char buffer1[80];
char buffer2[80];
bool Flag = false;
bool Flag2 = false;
bool gameOver = FALSE;
bool gameOverd = FALSE;
int x = 350;
int y = 100;
int score_l = 0;
int score_r = 0;
double speed = 0.075;
double x1 = 735, x2 = 785;
double rl = 0, rr = 10;
double ru = 500, rd = 500;
double r2l = 0, r2r = 10;
double r2u = 500, r2d = 500;
double fl = 0, fu = 0;
double fr = 50, fd = 50;
double f2l = 735, f2u = 0;
double f2r = 785, f2d = 50;
double i = 0.0;
double j = 0.0;
double k = 0.0;
double l = 0.0;
int text = sprintf_s(buffer0, "SCORE: %d", score_l);
int text2 = sprintf_s(buffer1, "SCORE: %d", score_r);

void Death_L()
{
	int DEAD_L = sprintf_s(buffer2, "THE RIGHT BOX WINS");
	printf("\a");
	TextOut(hdc, 335, 0, buffer2, DEAD_L);
}
void Death_R()
{
	int DEAD_R = sprintf_s(buffer2, "THE LEFT BOX WINS");
	printf("\a");
	TextOut(hdc, 335, 0, buffer2, DEAD_R);
}

void Boundaries()
{
	// Bullet Boundary
	if (r2l < 0)
	{
		r2l = 0;
		r2r = 10;
		r2u = 500;
		r2d = 500;
	}
	if (rr > 785)
	{
		rl = 0;
		rr = 10;
		ru = 500;
		rd = 500;
	}

	// p1 Boundary
	if (i < 0)
		i++;
	if (i + 50 > 400)
		i--;
	if (j < 0)
		j++;
	if (j + 50 > 400)
		j--;
	if (fl < 0)
	{
		fl++;
		fr++;
	}
	if (fr > 400)
	{
		fr--;
		fl--;
	}
	if (fu < 0)
	{
		fu++;
		fd++;
	}
	if (fd > 400)
	{
		fu--;
		fd--;
	}
	// p2 Boundary
	if (l < 0)
		l++;
	if (l + 50 > 400)
		l--;
	if (k + 735 < 400)
		k++;
	if (k + 785 > 785)
		k--;
	if (f2l < 400)
	{
		f2l++;
		f2r++;
	}
	if (f2u < 0)
	{
		f2u++;
		f2d++;
	}
	if (f2d > 400)
	{
		f2u--;
		f2d--;
	}
	if (f2r > 785)
	{
		f2r--;
		f2l--;
	}
}
void Collisions()
{
	if (rr > k + 735 && rd < l + 50 && rd > l && rr < k + 785)
	{
		rr--; rl--;
		score_l++;
		text = sprintf_s(buffer0, "SCORE: %d", score_l);
		gameOver = TRUE;
		Death_R();
	}
	if (r2l < i + 50 && r2u < j + 50 && r2u > j && r2l > i)
	{
		r2l++; r2r++;
		score_r++;
		text2 = sprintf_s(buffer1, "SCORE: %d", score_r);
		gameOver = TRUE;
		Death_L();
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSW wc = { 0 };
	HWND hWnd = { 0 };

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;
	
	if (!RegisterClassW(&wc))
		return 0;

	CreateWindowW(L"myWindowClass", L"My DirectX Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 500, NULL, NULL, hInstance, NULL);

	MSG msg = { 0 };
	
	main_window_handle = hWnd;
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		SelectObject(hdc, yellow_pen);
		///////////Border///////////
		MoveToEx(hdc, 400, 0, NULL);
		LineTo(hdc, 400, 500);
		MoveToEx(hdc, 0, 400, NULL);
		LineTo(hdc, 800, 400);
		////////////BOX2///////////
		//////////Bullets///////////
		SelectObject(hdc, green_pen);
		Rectangle(hdc, rl, ru, rr, rd);
		Rectangle(hdc, r2l, r2u, r2r, r2d);
		//////////BOXES//////////////
		SelectObject(hdc, black_pen);
		Rectangle(hdc, fl, fu, fr, fd);
		Rectangle(hdc, f2l, f2u, f2r, f2d);
		////////////////////////////
		EndPaint(hWnd, &ps);

		Boundaries();
		Collisions();

		TextOut(hdc, 80, 410, buffer0, text);
		TextOut(hdc, 590, 410, buffer1, text2);

		if (!gameOver)
		{
			if (KEY_DOWN(VK_RIGHT))
			{
				k = k + speed;
				f2l = f2l + speed;
				f2r = f2r + speed;
			}
			if (KEY_DOWN(VK_LEFT))
			{
				k = k - speed;
				f2l = f2l - speed;
				f2r = f2r - speed;
			}
			if (KEY_DOWN(VK_UP))
			{
				l = l - speed;
				f2u = f2u - speed;
				f2d = f2d - speed;
			}
			if (KEY_DOWN(VK_DOWN))
			{
				l = l + speed;
				f2u = f2u + speed;
				f2d = f2d + speed;
			}
			if (KEY_DOWN(VK_D))
			{
				i = i + speed;
				fl = fl + speed;
				fr = fr + speed;
			}
			if (KEY_DOWN(VK_A))
			{
				i = i - speed;
				fl = fl - speed;
				fr = fr - speed;
			}
			if (KEY_DOWN(VK_W))
			{
				j = j - speed;
				fu = fu - speed;
				fd = fd - speed;
			}
			if (KEY_DOWN(VK_S))
			{
				j = j + speed;
				fu = fu + speed;
				fd = fd + speed;
			}
			if (KEY_DOWN(VK_SPACE))
			{
				rl = i + 20;
				ru = j + 20;
				rr = i + 30;
				rd = j + 30;
				Flag = true;
			}
			if (KEY_DOWN(VK_SHIFT))
			{
				r2l = k + 735 + 20;
				r2u = l + 20;
				r2r = k + 735 + 30;
				r2d = l + 30;
				Flag2 = true;
			}
		}
		if (Flag2)
		{
			r2l = r2l - 0.2;
			r2r = r2r - 0.2;
			SelectObject(hdc, black_pen);
			Rectangle(hdc, r2l + 0.2, r2u, r2r + 0.2, r2d);
		}
		if (Flag)
		{
			rl = rl + 0.2;
			rr = rr + 0.2;
			SelectObject(hdc, black_pen);
			Rectangle(hdc, rl - 0.2, ru, rr - 0.2, rd);
		}
	}
	return 0;
	return(msg.wParam);
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//EndPaint(hWnd, &ps);
		return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	default:
		break;
	}
	return DefWindowProcW(hWnd, msg, wp, lp);
}