#include <Windows.h>

#define FRAME_PATTERN "■"
#define EMPTY_FRAME_PATTERN "  "

void drawFrameUtil(COORD cursor_pos, int w, int h,
	const char *wPattern, const char *hPattern,
	const char *tlPattern, const char *trPattern,
	const char *blPattern, const char *brPattern) ;

void gotoXY(int x, int y);

void drawXY(int x, int y, const char *text);

void hideCursor();