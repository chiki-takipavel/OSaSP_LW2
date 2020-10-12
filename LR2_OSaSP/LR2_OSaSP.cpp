// LR2_OSaSP.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "LR2_OSaSP.h"

#define MAX_LOADSTRING 100

using namespace std;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

CONST INT numOfColumns = 4;
CONST string filePath = "test.txt";
CONST POINT minSize = { 500, 500 };
BOOL isRead;

vector<string> list;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL ArrayInitialization();
VOID DrawTable(HWND hWnd, RECT wndRect);
VOID DrawLine(HDC hdc, COLORREF color, int x1, int y1, int x2, int y2);
VOID DrawVerticalTableLines(HDC hdc, COLORREF color, INT cellSizeX, INT tableSizeY);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	setlocale(LC_ALL, "ru-RU");
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LR2OSASP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR2OSASP));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR2OSASP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
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
	case WM_CREATE:
	{
		isRead = ArrayInitialization();
	}
	break;
	case WM_PAINT:
	{
		if (isRead)
		{
			RECT wndRect;
			GetClientRect(hWnd, &wndRect);
			DrawTable(hWnd, wndRect);
		}
	}
	break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* minmaxPtr;
		minmaxPtr = (MINMAXINFO*)lParam;
		minmaxPtr->ptMinTrackSize = minSize;
		break;
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

BOOL ArrayInitialization()
{
	ifstream fin;
	fin.open(filePath);
	if (!fin.is_open())
		return false;
	string str;
	while (getline(fin, str))
		list.push_back(str);
	fin.close();
	return true;
}

VOID DrawLine(HDC hdc, COLORREF color, int x1, int y1, int x2, int y2)
{
	HPEN pen = CreatePen(PS_INSIDEFRAME, 2, color);
	POINT pt;
	SelectObject(hdc, pen);
	MoveToEx(hdc, x1, y1, &pt);
	LineTo(hdc, x2, y2);
	DeleteObject(pen);
}

VOID DrawVerticalTableLines(HDC hdc, COLORREF color, INT cellSizeX, INT tableSizeY)
{
	for (int i = 1; i < numOfColumns; i++) {
		DrawLine(hdc, color, i * cellSizeX, 0, i * cellSizeX, tableSizeY);
	}
}

VOID DrawTable(HWND hWnd, RECT wndRect)
{
	INT			padding = 5,
				maxRowHight = 0,
				numOfRows,
				sizeOfColumn,
				sizeOfList;
	PAINTSTRUCT ps;
	RECT		rect, cellForText;
	HBRUSH		brush;
	COLORREF	colorText = RGB(0, 0, 0),
				colorBack = RGB(255, 255, 255),
				colorLine = RGB(255, 0, 0);
	HDC hdc = BeginPaint(hWnd, &ps);

	brush = CreateSolidBrush(colorBack);
	SelectObject(hdc, brush);
	Rectangle(hdc, wndRect.left, wndRect.top, wndRect.right, wndRect.bottom);
	DeleteObject(brush);

	sizeOfList = list.size();
	numOfRows = (sizeOfList % numOfColumns) ? sizeOfList / numOfColumns + 1 : sizeOfList / numOfColumns;

	sizeOfColumn = wndRect.right / numOfColumns;

	for (int i = 0; i < numOfRows; i++) {

		rect.top = maxRowHight;

		for (int j = 0; j < numOfColumns && numOfColumns * i + j < sizeOfList; j++) {

			rect.left = sizeOfColumn * j;
			rect.right = wndRect.right / numOfColumns * (j + 1);

			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, colorText);

			cellForText.top = rect.top + padding;
			cellForText.right = rect.right - padding;
			cellForText.left = rect.left + padding;

			const CHAR* cellText = list[numOfColumns * i + j].c_str();

			DrawTextA(hdc, (LPCSTR)cellText, strlen(cellText), &cellForText, DT_CALCRECT | DT_WORDBREAK);
			cellForText.right = rect.right - padding;
			rect.bottom = cellForText.bottom + padding;

			DrawTextA(hdc, (LPCSTR)cellText, strlen(cellText), &cellForText, DT_WORDBREAK | DT_CENTER | DT_NOCLIP);

			if (rect.bottom > maxRowHight)
				maxRowHight = rect.bottom;
		}

		DrawLine(hdc, colorLine, 0, maxRowHight, wndRect.right, maxRowHight);
	}

	DrawVerticalTableLines(hdc, colorLine, sizeOfColumn, maxRowHight);

	SetBkMode(hdc, OPAQUE);  // Переустанавливаем режим фона в его состояние по умолчанию. 
	EndPaint(hWnd, &ps);
}
