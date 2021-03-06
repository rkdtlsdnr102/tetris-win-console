#include "clMap.h"
#include <iostream>
#include <thread>
#include <Windows.h>

//#define DEBUG

clMap::clMap(std::pair<int,int> map_size){

    _map_size=map_size ;
    _map = new int*[_map_size.second] ;

	_play_field_start_row = _map_size.second*0.1;

    for(int i=0;i<_map_size.second;i++)
    {
		_map[i] = new int[_map_size.first];

        std::memset(_map[i],EMPTY_CELL,sizeof(int)*_map_size.first) ;
    }

	_cur_tetromino = clTetromino::createRandomTetromino();
	_cur_tetromino_tl = {_map_size.first/2,0} ;

}

void clMap::removeLine(int idx){

    for(int i=0;i<_map_size.first;i++){

        _map[idx][i]=EMPTY_CELL ;
    }

    _reorganizeFrom(idx-1) ;
}

void clMap::removeLines(std::vector<int> &idx){

    for(int j=idx.size()-1;j>=0;j--){

        for(int i=0;i<_map_size.first;i++){

			_map[idx[j]][i] = EMPTY_CELL;

        }

		_reorganizeFrom(idx[j] - 1);
    }

}

void clMap::_reorganizeFrom(int idx){

	int from_idx=idx ;

    for(int c=0;c<_map_size.first;c++){      

        for(int r=from_idx;r>=0;r--)
        {
            _map[r+1][c]=_map[r][c] ;
            _map[r][c]=EMPTY_CELL ;
        }
    }
}

bool clMap::isLineConnected(int idx){

    bool connected=true ;

    for(int c=0;c<_map_size.first;c++)
        if(_map[idx][c]==EMPTY_CELL)
        {
            connected=false ;
            break ;
        }

    return connected ;
}

bool clMap::moveTetrominoDown(){

	std::vector<std::pair<int, int>> *shape = _cur_tetromino.getCurrentShape();

    bool collide=false ;

	for (auto pt : *shape) {

		if (_cur_tetromino_tl.second+pt.second + 1 >= _map_size.second || _map[_cur_tetromino_tl.second+pt.second + 1][_cur_tetromino_tl.first+pt.first] != EMPTY_CELL)
		{
			collide = true;
			break;
		}
	}

    if(!collide)
	{
		_cur_tetromino_tl.second+=1 ;
	}
    
    return collide ;

}

void clMap::moveTetrominoHor(int offX){

    int br = _cur_tetromino.getRight();

	if (_cur_tetromino_tl.first + offX >= 0 && _cur_tetromino_tl.first + br + offX < _map_size.first)
	{
		std::vector<std::pair<int, int>> *shape = _cur_tetromino.getCurrentShape();
		
		bool colide = false;

		for (auto pt : *shape)
		{
			if (_map[_cur_tetromino_tl.second+pt.second][_cur_tetromino_tl.first+pt.first+offX] != EMPTY_CELL)
			{
				colide = true;
				break;
			}
		}

		if (!colide)
			_cur_tetromino_tl.first += offX;
	}
		
}

void clMap::hardDrop() {

	std::vector<std::pair<int, int>> *shape = _cur_tetromino.getCurrentShape();

	bool collide = false;

	while (!collide)
	{
		for (auto pt : *shape) {

			if (_cur_tetromino_tl.second + pt.second + 1 >= _map_size.second || _map[_cur_tetromino_tl.second + pt.second + 1][_cur_tetromino_tl.first + pt.first] != EMPTY_CELL)
			{
				collide = true;
				break;
			}
		}

		if (!collide)
		{
			_cur_tetromino_tl.second++;
		}
	}
}

std::pair<short, short> clMap::getShadowTopLeft() {

	std::vector<std::pair<int, int>> *shape = _cur_tetromino.getCurrentShape();

	bool collide = false;

	std::pair<int, int> tr_coll_tl = _cur_tetromino_tl;

	while (!collide)
	{
		for (auto pt : *shape) {

			if (tr_coll_tl.second + pt.second + 1 >= _map_size.second || _map[tr_coll_tl.second + pt.second + 1][tr_coll_tl.first + pt.first] != EMPTY_CELL)
			{
				collide = true;
				break;
			}
		}

		if (!collide)
		{
			tr_coll_tl.second++;
		}
	}

	return tr_coll_tl;
}

bool clMap::abovePlayfieldStart() {

	if (_cur_tetromino_tl.second < _play_field_start_row)
		return true;

	return false;
}

void clMap::rotateTromino90() {

	std::vector<std::pair<int,int>>* next_shape = _cur_tetromino.getNextShape();

	bool valid = true;

	for (auto pt : *next_shape)
	{
		auto new_pt = std::pair<int,int>{ _cur_tetromino_tl.first + pt.first, _cur_tetromino_tl.second + pt.second };

		if (!(0 <= new_pt.first && new_pt.first < _map_size.first &&
			0 <= new_pt.second && new_pt.second < _map_size.second &&
			_map[new_pt.second][new_pt.first]==EMPTY_CELL))
		{
			valid = false;
			break;
		}
	}

	if (valid)
	{
		_cur_tetromino.rotate90();
	}
}



void clMap::draw(COORD cursor_pos) {

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int r = 0; r < _map_size.second; r++)
	{
		COORD coord = cursor_pos;

		coord.X -= 2;
		coord.Y += r;
		
		SetConsoleCursorPosition(hdl, coord);
		puts(MAP_FRAME_PATTERN);

		coord.X += (_map_size.first * 2 + 2);

		SetConsoleCursorPosition(hdl, coord);
		puts(MAP_FRAME_PATTERN);
	}

	for (int c = -1; c <= _map_size.first; c++)
	{
		COORD coord = cursor_pos;

		coord.Y -= 1;
		coord.X += 2 * c;

		SetConsoleCursorPosition(hdl, coord);
		puts(MAP_FRAME_PATTERN);

		coord.Y += (_map_size.second + 1);

		SetConsoleCursorPosition(hdl, coord);
		puts(MAP_FRAME_PATTERN);
	}

	CONSOLE_SCREEN_BUFFER_INFO org_setting, new_setting;

	GetConsoleScreenBufferInfo(hdl, &org_setting);

	new_setting = org_setting;

	for (int r = 0; r < _map_size.second; r++) {

		for (int c = 0; c < _map_size.first; c++) {

			COORD coord = cursor_pos;

			coord.X += c * 2;
			coord.Y += r;

			SetConsoleCursorPosition(hdl, coord);

			if (_map[r][c] == EMPTY_CELL)
			{			
				if (r != _play_field_start_row)
					puts(MAP_PATTERN);
				else
					puts(PLAY_FIELD_START_PATTERN);
			}
			else
			{
				new_setting.wAttributes = _map[r][c];
				SetConsoleTextAttribute(hdl, new_setting.wAttributes);
				puts(TETROMINO_PATTERN);
			}

			SetConsoleTextAttribute(hdl, org_setting.wAttributes);
		}
	}
}

void clMap::drawStartLine(COORD cursor_pos)
{
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO org_setting, new_setting;

	GetConsoleScreenBufferInfo(hdl, &org_setting);

	new_setting = org_setting;

	int tr_bt = _cur_tetromino.getBottom();

	if (_cur_tetromino_tl.second-1 <= _play_field_start_row &&
		_cur_tetromino_tl.second + tr_bt-1 >= _play_field_start_row)
	{
 		for (int c = 0; c < _map_size.first; c++) {

			COORD coord = cursor_pos;

			coord.X += c * 2;
			coord.Y += _play_field_start_row;

			SetConsoleCursorPosition(hdl, coord);

			if (_map[_play_field_start_row][c] == EMPTY_CELL)
				puts(PLAY_FIELD_START_PATTERN);
			else
			{
				new_setting.wAttributes = _map[_play_field_start_row][c];
				SetConsoleTextAttribute(hdl, new_setting.wAttributes);
				puts(TETROMINO_PATTERN);
			}

			SetConsoleTextAttribute(hdl, org_setting.wAttributes);
		}
	}
}

void clMap::drawRemovingRow(std::vector<int> &rm_rows, COORD cursor_pos) {

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < 4; i++)
	{
		const char * pattern = nullptr;

		if (i % 2 == 0)
			pattern = REMOVING_CELL_PATTERN;
		else
			pattern = MAP_PATTERN;

		for (int r : rm_rows)
		{
			for (int c = 0; c < _map_size.first; c++)
			{
				COORD coord = cursor_pos;

				coord.X += c * 2;
				coord.Y += r;

				SetConsoleCursorPosition(hdl, coord);

				puts(pattern);
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void clMap::setTetromino(clTetromino &tr){

    _cur_tetromino=tr ;

	_cur_tetromino_tl = { _map_size.first / 2,0 };
}

void clMap::projectTetromino()
{
	auto shape = _cur_tetromino.getCurrentShape();

	for (auto pt : *shape)
	{
		_map[pt.second + _cur_tetromino_tl.second][pt.first + _cur_tetromino_tl.first] = _cur_tetromino.getColor();
	}
}

clTetromino &clMap::getTetromino(){

    return _cur_tetromino ;
}

std::pair<short,short> clMap::getTetrominoTopLeft(){

	return _cur_tetromino_tl ;
}

clMap::~clMap(){

    for(int r=0;r<_map_size.second;r++){

        delete[] _map[r] ;
    }

    delete[] _map ;
}