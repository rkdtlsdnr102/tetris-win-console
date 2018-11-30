#include "clTetris.h"
#include <iostream>
#include <iomanip>
#include <Windows.h>

#define ROTATE_KEY 0x52
#define HARD_DROP_KEY 0x20
#define KEY_PRESSED 0x0001
#define EMPTY_PATTERN  " "

const char rm_count_frame[][100] = { "¡à¡à¡à¡à¡à¡à¡à¡à¡à¡à¡à¡à",
									"¡à                    ¡à",
									"¡à                    ¡à",
									"¡à                    ¡à",
									"¡à¡à¡à¡à¡à¡à¡à¡à¡à¡à¡à¡à" };
const char next_tetromino_frame[][100] = { "¡à¡à¡à¡à¡à¡à¡à",
										"¡à          ¡à",
										"¡à          ¡à",
										"¡à          ¡à",
										"¡à          ¡à",
										"¡à          ¡à",
										"¡à¡à¡à¡à¡à¡à¡à" };


clTetris::clTetris(int fps)
{
	_map = new clMap({15,20});
	_map_dp_tl = { 10,3 };

	_next_tetromino = clTetromino::createRandomTetromino();
	_next_tetromino_dp_tl = { 50,12 };

	auto tr_tl = _map->getTetrominoTopLeft();

    _rm_lines_cnt = 20;
	_rm_lines_dp_tl = { 46,5 };
	_fps = fps;
}

void clTetris::run()
{
	int clk=0,period = 1000000 / _fps;

	_map->draw(_map_dp_tl);
	auto tr_tl = _map->getTetrominoTopLeft();
	_map->getTetromino().draw({ _map_dp_tl.X + tr_tl.first*2,_map_dp_tl.Y + tr_tl.second });
	_drawNextTetromino(_next_tetromino_dp_tl);
	_drawRemainingCount(_rm_lines_dp_tl);

	while (1)
	{
		clk++;

		userInput();

		if (clk == period)
		{
			auto tr_tl = _map->getTetrominoTopLeft();
			auto shadow_tl = _map->getShadowTopLeft();

			_map->getTetromino().erase({ _map_dp_tl.X + tr_tl.first*2,_map_dp_tl.Y + tr_tl.second },MAP_PATTERN);
			_map->getTetromino().erase({ _map_dp_tl.X + shadow_tl.first * 2, _map_dp_tl.Y + shadow_tl.second }, MAP_PATTERN);
			
			if (_map->moveTetrominoDown())
			{
				_next_tetromino.erase(_next_tetromino_dp_tl, EMPTY_PATTERN);

				_handleCollision();

				_map->draw(_map_dp_tl);
				_drawNextTetromino(_next_tetromino_dp_tl);
				_drawRemainingCount(_rm_lines_dp_tl);
			}
				
			tr_tl = _map->getTetrominoTopLeft();
			shadow_tl = _map->getShadowTopLeft();
			_map->getTetromino().draw({ _map_dp_tl.X + tr_tl.first*2,_map_dp_tl.Y + tr_tl.second });
			_map->getTetromino().draw({ _map_dp_tl.X + shadow_tl.first * 2,_map_dp_tl.Y + shadow_tl.second });

			clk = 0;
		}

		if (_rm_lines_cnt <= 0)
		{
			std::cout << "Game Over!!\n";
			break;
		}
	}
 
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
				_next_tetromino.erase(_next_tetromino_dp_tl, EMPTY_PATTERN);

				_handleCollision();

				_map->draw(_map_dp_tl);
				_drawNextTetromino(_next_tetromino_dp_tl);
				_drawRemainingCount(_rm_lines_dp_tl);
			}
		}
		else if ((hard_drop & KEY_PRESSED) == KEY_PRESSED)
		{			

			_map->hardDrop();

			_next_tetromino.erase(_next_tetromino_dp_tl, EMPTY_PATTERN);

			_handleCollision();

			_map->draw(_map_dp_tl);
			_drawNextTetromino(_next_tetromino_dp_tl);
			_drawRemainingCount(_rm_lines_dp_tl);
			
		}

		if ((rotate & KEY_PRESSED) == KEY_PRESSED)
		{			
			_map->rotateTromino90();			
		}

		tr_tl = _map->getTetrominoTopLeft();
		shadow_tl = _map->getShadowTopLeft();
		_map->getTetromino().draw({ _map_dp_tl.X + tr_tl.first * 2,_map_dp_tl.Y + tr_tl.second });
		_map->getTetromino().draw({ _map_dp_tl.X + shadow_tl.first * 2,_map_dp_tl.Y + shadow_tl.second });
	}
}

void clTetris::_drawRemainingCount(COORD cursor_pos) {

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < 5; i++)
	{
		
		COORD frame_cursor_pos = cursor_pos;
		frame_cursor_pos.X -= 2;
		frame_cursor_pos.Y += (-2 + i);

		SetConsoleCursorPosition(hdl, frame_cursor_pos);

		std::cout << rm_count_frame[i];
	}

	SetConsoleCursorPosition(hdl, cursor_pos);
	
	std::cout << std::setw(20) << std::cout.fill(' ');
	SetConsoleCursorPosition(hdl, cursor_pos);
	std::cout << "Remaining lines: " << _rm_lines_cnt;
}

void clTetris::_drawNextTetromino(COORD cursor_pos)
{
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < 7; i++)
	{
		COORD frame_cursor_pos = cursor_pos;
		frame_cursor_pos.X -= 4;
		frame_cursor_pos.Y += (-2 + i);

		SetConsoleCursorPosition(hdl, frame_cursor_pos);

		std::cout << next_tetromino_frame[i];
	}

	_next_tetromino.draw(cursor_pos);
}

void clTetris::_handleCollision(){

    clTetromino cur_tr = _map->getTetromino() ;

	std::pair<int,int> tl_tr=_map->getTetrominoTopLeft() ;

	_map->projectTetromino();

    std::vector<int> remove_rows ;

    for(int r=tl_tr.second+cur_tr.getBottom();r>=tl_tr.second;r--)
    {
        if(_map->isLineConnected(r))
            remove_rows.push_back(r) ;
    }

	if (remove_rows.size() > 0)
	{
		_map->removeLines(remove_rows);

		_rm_lines_cnt -= remove_rows.size();
	}

	_map->setTetromino(_next_tetromino);

	_next_tetromino = clTetromino::createRandomTetromino();
}

clTetris::~clTetris() {

	delete _map;
}
