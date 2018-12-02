#include "clTetris.h"
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <mmsystem.h>

#define ROTATE_KEY 0x52
#define HARD_DROP_KEY 0x20
#define KEY_PRESSED 0x0001
#define EMPTY_PATTERN  " "
#define SCREEN_FRAME_PATTERN "¡á"



clTetris::clTetris(int fps)
{
	_map = new clMap({10,20});
	_map_dp_tl = { 2,1 };

	_explain_frame_dp_tl = { 42,1 };

	_next_tetromino = clTetromino::createRandomTetromino();
	_next_tetromino_dp_tl = { 30,7 };

	auto tr_tl = _map->getTetrominoTopLeft();

	HWND console_hwnd = GetConsoleWindow();

	MoveWindow(console_hwnd, 100, 100, 1000, 600, TRUE);

    _rm_lines_cnt = 20;
	_rm_lines_dp_tl = { 28,19 };
	_fps = fps;

	_g_state = GAME_RUNNING;
	_last_result = PLAYER_WIN;

	PlaySound(TEXT("BGM+Tetris+Kalinka.wav"), NULL,
					SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void clTetris::run()
{
	int clk=0,period = 1000000 / _fps;

	_map->draw(_map_dp_tl);
	_map->drawStartLine(_map_dp_tl);
	auto tr_tl = _map->getTetrominoTopLeft();
	_map->getTetromino().draw({ _map_dp_tl.X + tr_tl.first*2,_map_dp_tl.Y + tr_tl.second });
	_drawNextTetromino(_next_tetromino_dp_tl);
	_drawRemainingCount(_rm_lines_dp_tl);
	_drawExplain(_explain_frame_dp_tl);

	while (1)
	{
		clk++;

		userInput();

		if (_g_state == GAME_OVER)
			break;

		if (clk == period)
		{
			auto tr_tl = _map->getTetrominoTopLeft();
			auto shadow_tl = _map->getShadowTopLeft();

			_map->getTetromino().erase({ _map_dp_tl.X + tr_tl.first*2,_map_dp_tl.Y + tr_tl.second },MAP_PATTERN);
			_map->getTetromino().erase({ _map_dp_tl.X + shadow_tl.first * 2, _map_dp_tl.Y + shadow_tl.second }, MAP_PATTERN);
			
			if (_map->moveTetrominoDown())
			{					
				_handleCollision();

				_map->draw(_map_dp_tl);			
				_drawRemainingCount(_rm_lines_dp_tl);
			}
			
			_map->drawStartLine(_map_dp_tl);

			tr_tl = _map->getTetrominoTopLeft();
			shadow_tl = _map->getShadowTopLeft();
			_map->getTetromino().draw({ _map_dp_tl.X + shadow_tl.first * 2,_map_dp_tl.Y + shadow_tl.second }, SHADOW_TETROMINO_PATTERN);
			_map->getTetromino().draw({ _map_dp_tl.X + tr_tl.first*2,_map_dp_tl.Y + tr_tl.second });		

			clk = 0;
		}

		if (_g_state == GAME_OVER)
			break;
	}

	if (_last_result == PLAYER_WIN)
	{
		std::cout << "PLAYER WIN";
	}
	else
		std::cout << "PLAYER_LOSE";
 
}

void clTetris::userInput()
{

    short left = GetAsyncKeyState(VK_LEFT);
    short right = GetAsyncKeyState(VK_RIGHT);
    short down = GetAsyncKeyState(VK_DOWN);
    short rotate = GetAsyncKeyState(ROTATE_KEY);
	short hard_drop = GetAsyncKeyState(HARD_DROP_KEY);

	short res;
	if (res=((left & KEY_PRESSED) | (right & KEY_PRESSED) | (down & KEY_PRESSED) | (rotate & KEY_PRESSED) | (hard_drop & KEY_PRESSED)))
	{
		auto tr_tl = _map->getTetrominoTopLeft();
		auto shadow_tl = _map->getShadowTopLeft();		

		_map->getTetromino().erase({ _map_dp_tl.X + shadow_tl.first*2, _map_dp_tl.Y + shadow_tl.second }, MAP_PATTERN);
		_map->getTetromino().erase({ _map_dp_tl.X + tr_tl.first * 2,_map_dp_tl.Y + tr_tl.second }, MAP_PATTERN);

		if ((left & KEY_PRESSED) == KEY_PRESSED)
		{			
			_map->moveTetrominoHor(-1);		
		}
		else if ((right & KEY_PRESSED) == KEY_PRESSED)
		{			
			_map->moveTetrominoHor(1);			
		}
		else if ((down & KEY_PRESSED) == KEY_PRESSED)
		{
			if (_map->moveTetrominoDown())
			{	
				_handleCollision();

				_map->draw(_map_dp_tl);
				_drawRemainingCount(_rm_lines_dp_tl);
			}
		}
		else if ((hard_drop & KEY_PRESSED) == KEY_PRESSED)
		{			

			_map->hardDrop();

			_handleCollision();

			_map->draw(_map_dp_tl);
			_drawRemainingCount(_rm_lines_dp_tl);
			
		}

		if ((rotate & KEY_PRESSED) == KEY_PRESSED)
		{			
			_map->rotateTromino90();			
		}

		_map->drawStartLine(_map_dp_tl);

		tr_tl = _map->getTetrominoTopLeft();
		shadow_tl = _map->getShadowTopLeft();
		_map->getTetromino().draw({ _map_dp_tl.X + shadow_tl.first * 2,_map_dp_tl.Y + shadow_tl.second }, SHADOW_TETROMINO_PATTERN);
		_map->getTetromino().draw({ _map_dp_tl.X + tr_tl.first * 2,_map_dp_tl.Y + tr_tl.second });
	}
}

void drawFrame(COORD cursor_pos, int w, int h)
{
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int c = 0; c < w; c++)
	{

		COORD frame_cursor_pos = cursor_pos;
		frame_cursor_pos.X += 2*c;

		SetConsoleCursorPosition(hdl, frame_cursor_pos);

		std::cout << SCREEN_FRAME_PATTERN;
		
		frame_cursor_pos.Y += (h-1);

		SetConsoleCursorPosition(hdl, frame_cursor_pos);

		std::cout << SCREEN_FRAME_PATTERN;
	}

	for (int r = 0; r < h; r++)
	{

		COORD frame_cursor_pos = cursor_pos;		
		frame_cursor_pos.Y += r;

		SetConsoleCursorPosition(hdl, frame_cursor_pos);

		std::cout << SCREEN_FRAME_PATTERN;

		frame_cursor_pos.X += (2 * w-2);

		SetConsoleCursorPosition(hdl, frame_cursor_pos);

		std::cout << SCREEN_FRAME_PATTERN;
	}
}

void clTetris::_drawRemainingCount(COORD cursor_pos) {

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	drawFrame({ cursor_pos.X - 3,cursor_pos.Y - 2 }, 13, 5);

	SetConsoleCursorPosition(hdl, cursor_pos);
	
	std::cout << std::setw(20) << std::cout.fill(' ');
	SetConsoleCursorPosition(hdl, cursor_pos);
	std::cout << "Remaining lines: " << _rm_lines_cnt;
}

void clTetris::_drawNextTetromino(COORD cursor_pos)
{
	int tr_bt = _next_tetromino.getBottom(), tr_r = _next_tetromino.getRight();

	drawFrame({ cursor_pos.X - 4,cursor_pos.Y-2 }, 7, 8);

	_next_tetromino.draw(cursor_pos);
}

void clTetris::_drawExplain(COORD cursor_pos)
{

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	drawFrame(cursor_pos, 15, 12);

	SetConsoleCursorPosition(hdl, { cursor_pos.X + 4,cursor_pos.Y + 2 });
	puts("LEFT RIGHT MOVE: ¡æ,¡ç");
	SetConsoleCursorPosition(hdl, { cursor_pos.X + 4,cursor_pos.Y + 4 });
	puts("MOVE 1 DOWN: ¡é");
	SetConsoleCursorPosition(hdl, { cursor_pos.X + 4,cursor_pos.Y + 6 });
	puts("HARD DROP: SPACE");
	SetConsoleCursorPosition(hdl, { cursor_pos.X + 4,cursor_pos.Y + 8 });
	puts("ROTATION(clockwise): R");
}

void clTetris::_handleCollision(){

	if (_map->abovePlayfieldStart())
	{
		_g_state = GAME_OVER;
		_last_result = PLAYER_LOSE;
	}
	else
	{

		clTetromino cur_tr = _map->getTetromino();

		std::pair<int, int> tl_tr = _map->getTetrominoTopLeft();

		_map->projectTetromino();

		std::vector<int> remove_rows;

		for (int r = tl_tr.second + cur_tr.getBottom(); r >= tl_tr.second; r--)
		{
			if (_map->isLineConnected(r))
				remove_rows.push_back(r);
		}

		if (remove_rows.size() > 0)
		{
			_map->draw(_map_dp_tl);
			_map->drawRemovingRow(remove_rows, _map_dp_tl);

			_map->removeLines(remove_rows);

			_rm_lines_cnt -= remove_rows.size();
		}

		_map->setTetromino(_next_tetromino);

		_next_tetromino.erase(_next_tetromino_dp_tl, EMPTY_PATTERN);

		_next_tetromino = clTetromino::createRandomTetromino();

		_drawNextTetromino(_next_tetromino_dp_tl);

		if (_rm_lines_cnt <= 0)
		{
			_g_state = GAME_OVER;
			_last_result = PLAYER_WIN;
		}

	}
}

clTetris::~clTetris() {

	delete _map;

	PlaySound(NULL, 0,0);
}
