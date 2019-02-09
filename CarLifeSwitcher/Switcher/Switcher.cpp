// Switcher.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Switcher.h"
#include "../Consts.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE			g_hInst;			// 当前实例
//屏幕大小
int g_ScreenWidth;
int g_ScreenHeight;
//工具条矩形
RECT rc;
int g_ButtonWidth = 60;
int g_ButtonHeight= 60;
int g_ButtonTotalWidth = g_ButtonWidth / 2;
//工具条是否隐藏
bool g_ToolbarHiden = false;
//主界面是否隐藏
bool g_MainScreenHiden = false;

HWND hBtnSwitcher;
HWND hBtnCarLife;

// 此代码模块中包含的函数的前向声明:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleToolbar(HWND);
void PlayButtonSound();

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPTSTR    lpCmdLine,
				   int       nCmdShow)
{
	MSG msg;

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(2));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SWITCHER));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	g_hInst = hInstance; // 将实例句柄存储在全局变量中

	if (!MyRegisterClass(hInstance, STRING_APP_SWITCHER_TITLE))
	{
		return FALSE;
	}

	hWnd = CreateWindow(STRING_APP_SWITCHER_TITLE, STRING_APP_SWITCHER_TITLE, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	//获取屏幕大小
	g_ScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	g_ScreenHeight= GetSystemMetrics(SM_CYFULLSCREEN);

	//设置窗口大小和位置
	ToggleToolbar(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void ToggleToolbar(HWND hWnd)
{
	if(g_ToolbarHiden)
	{		
		rc.left = g_ScreenWidth - g_ButtonTotalWidth;
		SetWindowText(hBtnSwitcher,L">");
	}
	else
	{
		rc.left = g_ScreenWidth - (g_ButtonHeight / 2);		
		SetWindowText(hBtnSwitcher,L"<");
	}
	g_ToolbarHiden = !g_ToolbarHiden;
	if(rc.left<0)
		rc.left=0;
	rc.top = (g_ScreenHeight - g_ButtonHeight) / 2;
	if(rc.top<0)
		rc.top=0;
	rc.bottom = rc.top + g_ButtonHeight;	
	rc.right = rc.left + g_ButtonTotalWidth;
	MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
}

void PlayButtonSound()
{
	PlaySound(STRING_SOUND_FULL_FILE_NAME,NULL,SND_FILENAME|SND_ASYNC);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;


	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// 分析菜单选择:
		switch (wmId)
		{
		case IDB_SWITCHER:
			PlayButtonSound();
			ToggleToolbar(hWnd);			
			break;
		case IDB_CARLIFE:
			PlayButtonSound();
			HWND hTemp;
			if(g_MainScreenHiden)
			{
				//显示主界面
				hTemp = FindWindow(NULL,STRING_CHERY_WINDOW_TITLE);
				if(!hTemp)
					MessageBox(hWnd,L"未找到主窗口句柄！",STRING_APP_SWITCHER_TITLE,MB_OK);
				else
					ShowWindow(hTemp,SW_SHOW);
				//隐藏CarLife
				hTemp = FindWindow(NULL,STRING_CARLIFE_WINDOW_TITLE);
				if(hTemp)
					ShowWindow(hTemp,SW_HIDE);
				SetWindowText(hBtnCarLife,STRING_BTN_CARLIFE_TITLE);
			}
			else
			{
				//显示CarLife
				hTemp = FindWindow(NULL,STRING_CARLIFE_WINDOW_TITLE);
				if(!hTemp)
					CreateProcess(STRING_CARLIFE_FULL_FILE_NAME,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
				else				
					ShowWindow(hTemp,SW_SHOW);
				//隐藏主界面
				hTemp = FindWindow(NULL,STRING_CHERY_WINDOW_TITLE);
				if(!hTemp)
					MessageBox(hWnd,L"未找到主窗口句柄！",STRING_APP_SWITCHER_TITLE,MB_OK);
				else
					ShowWindow(hTemp,SW_HIDE);
				SetWindowText(hBtnCarLife,STRING_BTN_CHERYLINK_TITLE);
			}
			g_MainScreenHiden = !g_MainScreenHiden;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		//创建按钮
		g_ButtonTotalWidth = 0;

		hBtnSwitcher = CreateWindow(L"Button", L"<", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			g_ButtonTotalWidth, 0, g_ButtonWidth / 2, g_ButtonWidth,
			hWnd, (HMENU)IDB_SWITCHER, (HINSTANCE)hWnd, NULL);
		g_ButtonTotalWidth +=g_ButtonWidth / 2;

		hBtnCarLife = CreateWindow(L"Button", STRING_BTN_CARLIFE_TITLE, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			g_ButtonTotalWidth, 0, g_ButtonWidth, g_ButtonWidth,
			hWnd, (HMENU)IDB_CARLIFE, (HINSTANCE)hWnd, NULL);
		g_ButtonTotalWidth +=g_ButtonWidth;

		//设置定时器
		SetTimer(hWnd,1,100,NULL);
		break;
	case WM_TIMER:
		//置顶
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd,1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
