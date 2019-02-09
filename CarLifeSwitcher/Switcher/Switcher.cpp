// Switcher.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Switcher.h"
#include "../Consts.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE			g_hInst;			// ��ǰʵ��
//��Ļ��С
int g_ScreenWidth;
int g_ScreenHeight;
//����������
RECT rc;
int g_ButtonWidth = 60;
int g_ButtonHeight= 60;
int g_ButtonTotalWidth = g_ButtonWidth / 2;
//�������Ƿ�����
bool g_ToolbarHiden = false;
//�������Ƿ�����
bool g_MainScreenHiden = false;

HWND hBtnSwitcher;
HWND hBtnCarLife;

// �˴���ģ���а����ĺ�����ǰ������:
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

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(2));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	g_hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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

	//��ȡ��Ļ��С
	g_ScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	g_ScreenHeight= GetSystemMetrics(SM_CYFULLSCREEN);

	//���ô��ڴ�С��λ��
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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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
				//��ʾ������
				hTemp = FindWindow(NULL,STRING_CHERY_WINDOW_TITLE);
				if(!hTemp)
					MessageBox(hWnd,L"δ�ҵ������ھ����",STRING_APP_SWITCHER_TITLE,MB_OK);
				else
					ShowWindow(hTemp,SW_SHOW);
				//����CarLife
				hTemp = FindWindow(NULL,STRING_CARLIFE_WINDOW_TITLE);
				if(hTemp)
					ShowWindow(hTemp,SW_HIDE);
				SetWindowText(hBtnCarLife,STRING_BTN_CARLIFE_TITLE);
			}
			else
			{
				//��ʾCarLife
				hTemp = FindWindow(NULL,STRING_CARLIFE_WINDOW_TITLE);
				if(!hTemp)
					CreateProcess(STRING_CARLIFE_FULL_FILE_NAME,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
				else				
					ShowWindow(hTemp,SW_SHOW);
				//����������
				hTemp = FindWindow(NULL,STRING_CHERY_WINDOW_TITLE);
				if(!hTemp)
					MessageBox(hWnd,L"δ�ҵ������ھ����",STRING_APP_SWITCHER_TITLE,MB_OK);
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
		//������ť
		g_ButtonTotalWidth = 0;

		hBtnSwitcher = CreateWindow(L"Button", L"<", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			g_ButtonTotalWidth, 0, g_ButtonWidth / 2, g_ButtonWidth,
			hWnd, (HMENU)IDB_SWITCHER, (HINSTANCE)hWnd, NULL);
		g_ButtonTotalWidth +=g_ButtonWidth / 2;

		hBtnCarLife = CreateWindow(L"Button", STRING_BTN_CARLIFE_TITLE, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			g_ButtonTotalWidth, 0, g_ButtonWidth, g_ButtonWidth,
			hWnd, (HMENU)IDB_CARLIFE, (HINSTANCE)hWnd, NULL);
		g_ButtonTotalWidth +=g_ButtonWidth;

		//���ö�ʱ��
		SetTimer(hWnd,1,100,NULL);
		break;
	case WM_TIMER:
		//�ö�
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
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
