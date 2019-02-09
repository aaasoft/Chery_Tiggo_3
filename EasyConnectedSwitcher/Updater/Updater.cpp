// Updater.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Updater.h"
#include "../Consts.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE			g_hInst;			// 当前实例
int g_ButtonWidth = 80;
int g_ButtonHeight= 80;
int g_ButtonTotalWidth = g_ButtonWidth / 2;

// 此代码模块中包含的函数的前向声明:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UPDATER));
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

	if (!MyRegisterClass(hInstance, STRING_APP_UPDATER_TITLE))
	{
		return FALSE;
	}

	hWnd = CreateWindow(STRING_APP_UPDATER_TITLE, STRING_APP_UPDATER_TITLE, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
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
	BOOL bRet;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// 分析菜单选择:
		switch (wmId)
		{
		case IDB_CLOSE_SWITCHER:
			HWND hTemp;
			hTemp = FindWindow(NULL,STRING_APP_SWITCHER_TITLE);
			if(!hTemp)
			{
				MessageBox(hWnd,L"未找到切换器窗口！",STRING_APP_UPDATER_TITLE,MB_OK);
				break;
			}
			if(!DestroyWindow(hTemp))
				MessageBox(hWnd,L"关闭切换器窗口失败！",STRING_APP_UPDATER_TITLE,MB_OK);
			break;
		case IDB_COPY_HCLINK:
			//先删除
			DeleteFile(STRING_DES_HCLINK_CONF_FULL_FILE_NAME);
			//再复制
			bRet = CopyFile(STRING_SRC_HCLINK_CONF_FULL_FILE_NAME,STRING_DES_HCLINK_CONF_FULL_FILE_NAME,true);
			if(bRet)
				MessageBox(hWnd,L"设置自启动成功！",STRING_APP_UPDATER_TITLE,MB_OK);
			else
				MessageBox(hWnd,L"设置自启动失败！",STRING_APP_UPDATER_TITLE,MB_OK);
			break;
		case IDB_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		//创建按钮
		g_ButtonTotalWidth = 0;

		CreateWindow(L"Button", L"关闭切换器", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			g_ButtonTotalWidth, 0, g_ButtonWidth , g_ButtonWidth,
			hWnd, (HMENU)IDB_CLOSE_SWITCHER, (HINSTANCE)hWnd, NULL);
		g_ButtonTotalWidth +=g_ButtonWidth;

		CreateWindow(L"Button", L"设置开机启动", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			g_ButtonTotalWidth, 0, g_ButtonWidth, g_ButtonWidth,
			hWnd, (HMENU)IDB_COPY_HCLINK, (HINSTANCE)hWnd, NULL);
		g_ButtonTotalWidth +=g_ButtonWidth;

		CreateWindow(L"Button", L"退出", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			g_ButtonTotalWidth, 0, g_ButtonWidth, g_ButtonWidth,
			hWnd, (HMENU)IDB_EXIT, (HINSTANCE)hWnd, NULL);
		g_ButtonTotalWidth +=g_ButtonWidth;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}