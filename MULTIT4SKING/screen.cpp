#include <windows.h>
#include "definsize.h"

// ��ũ�� ���� �ε���
static bool g_nScreenIndex;
// ��ũ�� ���� (����)
static HANDLE g_hScreen[2];

/// <summary>
/// ��ũ�� ũ�� ����
/// </summary>
/// <param name="Width">����</param>
/// <param name="Height">����</param>
void SetWindow(int Width, int Height) {
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
	SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
	SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
}

/// <summary>
/// ��ũ�� �ʱ�ȭ
/// </summary>
void ScreenInit() {

	// ���콺 �Է� ����
	HANDLE consoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(consoleInputHandle, ENABLE_EXTENDED_FLAGS);

	// �ܼ� â ũ�� ���� ���� (���콺�� ���� �Ұ���)
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_SIZEBOX);

	// ��üȭ�� ��� ����
	LONG_PTR style = GetWindowLongPtr(consoleWindow, GWL_STYLE);
	style &= ~WS_MAXIMIZEBOX;
	SetWindowLongPtr(consoleWindow, GWL_STYLE, style);

	// ��ũ�� ������ ����
	SetWindow(SCREEN_SIZE_X, SCREEN_SIZE_Y);

	CONSOLE_CURSOR_INFO cci;

	//ȭ�� ���� 2���� �����.
	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	//Ŀ�� �����
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}

/// <summary>
/// ��ũ�� �ø�
/// </summary>
void ScreenFlipping() {
	// ���� ���۸� Ȱ�� ȭ������ ����
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	// ��Ȱ�� ���۸� �����ͼ� �۾� ����
	g_nScreenIndex = !g_nScreenIndex;
}

/// <summary>
/// ��ũ�� �ʱ�ȭ
/// </summary>
void ScreenClear() {
	COORD Coor = { 0,0 };
	DWORD dw;
	// ��ü ȭ���� ���������� ĥ��
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', SCREEN_SIZE_X * SCREEN_SIZE_Y, Coor, &dw);
}

/// <summary>
/// ��ũ�� �ʱ�ȭ
/// </summary>
void ScreenRelease() {
	// ��� ���۸� ����
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
}

/// <summary>
/// ��ũ�� �׸���
/// </summary>
/// <param name="x">x ��ǥ</param>
/// <param name="y">y ��ǥ</param>
/// <param name="string">����� ���ڿ�</param>
void ScreenPrintString(short x, short y, char* string) {
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, (DWORD)strlen(string), &dw, NULL);
}

/// <summary>
/// �� ����
/// </summary>
/// <param name="color">�� ����</param>
void SetColor(unsigned short color) {
	SetConsoleTextAttribute(g_hScreen[g_nScreenIndex], color);
	SetConsoleTextAttribute(g_hScreen[!g_nScreenIndex], color);
}

