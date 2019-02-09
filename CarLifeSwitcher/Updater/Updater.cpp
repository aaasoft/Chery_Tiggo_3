// Updater.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Updater.h"
#include "../Consts.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE			g_hInst;			// ��ǰʵ��
int g_ButtonWidth = 80;
int g_ButtonHeight= 80;
int g_ButtonTotalWidth = g_ButtonWidth / 2;

// �˴���ģ���а����ĺ�����ǰ������:
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
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UPDATER));
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
	BOOL bRet;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDB_CLOSE_SWITCHER:
			HWND hTemp;
			hTemp = FindWindow(NULL,STRING_APP_SWITCHER_TITLE);
			if(!hTemp)
			{
				MessageBox(hWnd,L"δ�ҵ��л������ڣ�",STRING_APP_UPDATER_TITLE,MB_OK);
				break;
			}
			if(!DestroyWindow(hTemp))
				MessageBox(hWnd,L"�ر��л�������ʧ�ܣ�",STRING_APP_UPDATER_TITLE,MB_OK);
			break;
		case IDB_COPY_HCLINK:
			//��ɾ��
			DeleteFile(STRING_DES_HCLINK_CONF_FULL_FILE_NAME);
			//�ٸ���
			bRet = CopyFile(STRING_SRC_HCLINK_CONF_FULL_FILE_NAME,STRING_DES_HCLINK_CONF_FULL_FILE_NAME,true);
			if(bRet)
				MessageBox(hWnd,L"�����������ɹ���",STRING_APP_UPDATER_TITLE,MB_OK);
			else
				MessageBox(hWnd,L"����������ʧ�ܣ�",STRING_APP_UPDATER_TITLE,MB_OK);
			break;
		case IDB_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		//������ť
		g_ButtonTotalWidth = 0;

		CreateWindow(L"Button", L"�ر��л���", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			g_ButtonTotalWidth, 0, g_ButtonWidth , g_ButtonWidth,
			hWnd, (HMENU)IDB_CLOSE_SWITCHER, (HINSTANCE)hWnd, NULL);
		g_ButtonTotalWidth +=g_ButtonWidth;

		CreateWindow(L"Button", L"���ÿ�������", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			g_ButtonTotalWidth, 0, g_ButtonWidth, g_ButtonWidth,
			hWnd, (HMENU)IDB_COPY_HCLINK, (HINSTANCE)hWnd, NULL);
		g_ButtonTotalWidth +=g_ButtonWidth;

		CreateWindow(L"Button", L"�˳�", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			g_ButtonTotalWidth, 0, g_ButtonWidth, g_ButtonWidth,
			hWnd, (HMENU)IDB_EXIT, (HINSTANCE)hWnd, NULL);
		g_ButtonTotalWidth +=g_ButtonWidth;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
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