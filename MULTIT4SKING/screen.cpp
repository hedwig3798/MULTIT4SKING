#include <windows.h>
#include "definsize.h"

// 스크린 버퍼 인덱스
static bool g_nScreenIndex;
// 스크린 버퍼 (이중)
static HANDLE g_hScreen[2];

/// <summary>
/// 스크린 크기 조절
/// </summary>
/// <param name="Width">높이</param>
/// <param name="Height">넓이</param>
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
/// 스크린 초기화
/// </summary>
void ScreenInit() {

	// 마우스 입력 무시
	HANDLE consoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(consoleInputHandle, ENABLE_EXTENDED_FLAGS);

	// 콘솔 창 크기 변경 방지 (마우스로 조절 불가능)
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_SIZEBOX);

	// 전체화면 기능 방지
	LONG_PTR style = GetWindowLongPtr(consoleWindow, GWL_STYLE);
	style &= ~WS_MAXIMIZEBOX;
	SetWindowLongPtr(consoleWindow, GWL_STYLE, style);

	// 스크린 사이즈 조절
	SetWindow(SCREEN_SIZE_X, SCREEN_SIZE_Y);

	CONSOLE_CURSOR_INFO cci;

	//화면 버퍼 2개를 만든다.
	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	//커서 숨기기
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}

/// <summary>
/// 스크린 플립
/// </summary>
void ScreenFlipping() {
	// 현재 버퍼를 활성 화면으로 돌림
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	// 비활성 버퍼를 가져와서 작업 수행
	g_nScreenIndex = !g_nScreenIndex;
}

/// <summary>
/// 스크린 초기화
/// </summary>
void ScreenClear() {
	COORD Coor = { 0,0 };
	DWORD dw;
	// 전체 화면을 검은색으로 칠함
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', SCREEN_SIZE_X * SCREEN_SIZE_Y, Coor, &dw);
}

/// <summary>
/// 스크린 초기화
/// </summary>
void ScreenRelease() {
	// 모든 버퍼를 닫음
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
}

/// <summary>
/// 스크린 그리기
/// </summary>
/// <param name="x">x 좌표</param>
/// <param name="y">y 좌표</param>
/// <param name="string">출력할 문자열</param>
void ScreenPrintString(short x, short y, char* string) {
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, (DWORD)strlen(string), &dw, NULL);
}

/// <summary>
/// 색 지정
/// </summary>
/// <param name="color">색 변수</param>
void SetColor(unsigned short color) {
	SetConsoleTextAttribute(g_hScreen[g_nScreenIndex], color);
	SetConsoleTextAttribute(g_hScreen[!g_nScreenIndex], color);
}

