#pragma once
void ScreenInit();
void ScreenFlipping();
void ScreenClear();
void ScreenRelease();
void ScreenPrintString(short x, short y, char* string);
void SetColor(unsigned short color);
void SetWindow(int Width, int Height);