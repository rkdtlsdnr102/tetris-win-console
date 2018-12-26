#include "tetrisUtility.h"
#include <iostream>
#include <mutex>

void drawFrameUtil(COORD cursor_pos, int w, int h, 
	const char *wPattern, const char *hPattern,
	const char *tlPattern, const char *trPattern,
	const char *blPattern, const char *brPattern)
{
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	int w_pattern_width = std::strlen(wPattern);
	//draw 4 corner pattern
	
	drawXY(cursor_pos.X,cursor_pos.Y ,tlPattern);
	drawXY(cursor_pos.X+(w-1)*w_pattern_width,cursor_pos.Y , trPattern);
	drawXY(cursor_pos.X,cursor_pos.Y+h-1,blPattern);
	drawXY(cursor_pos.X+ (w - 1)*w_pattern_width ,cursor_pos.Y+h-1,brPattern);
	
	for (int c = 1; c < w-1; c++)
	{

		COORD frame_cursor_pos = cursor_pos;
		frame_cursor_pos.X += c* w_pattern_width;

		drawXY(frame_cursor_pos.X, frame_cursor_pos.Y,wPattern);
	
		frame_cursor_pos.Y += (h-1);

		drawXY(frame_cursor_pos.X, frame_cursor_pos.Y, wPattern);
	}

	for (int r = 1; r < h-1; r++)
	{

		COORD frame_cursor_pos = cursor_pos;		
		frame_cursor_pos.Y += r;

		drawXY(frame_cursor_pos.X, frame_cursor_pos.Y, hPattern);

		frame_cursor_pos.X += (w-1)*w_pattern_width;

		drawXY(frame_cursor_pos.X, frame_cursor_pos.Y, hPattern);
		
    }
}

std::mutex mtx;

void gotoXY(int x, int y)
{
	mtx.lock();
	hideCursor();
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { (SHORT)x,(SHORT)y });
	mtx.unlock();
}

void drawXY(int x, int y, const char *text)
{
	mtx.lock();
	hideCursor();
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { (SHORT)x,(SHORT)y });
	puts(text);
	mtx.unlock();
}

void hideCursor() {

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cinfo;

	GetConsoleCursorInfo(hdl, &cinfo);

	cinfo.bVisible = FALSE;

	SetConsoleCursorInfo(hdl, &cinfo);
}