#include "clMap.h"
#include "tetrisUtility.h"
#include <iostream>
#include <thread>
#include <Windows.h>

#define ADD_BLOCK GRAY

//#define DEBUG

clMap::clMap(std::pair<int,int> map_size,COORD map_abs_dp_tl){

    _map_size=map_size ;
    _map = new int*[_map_size.second] ;

	_map_dp_abs_tl = map_abs_dp_tl;

	_play_field_start_row = _map_size.second*0.1;

    for(int i=0;i<_map_size.second;i++)
    {
		_map[i] = new int[_map_size.first];

        std::memset(_map[i],EMPTY_CELL,sizeof(int)*_map_size.first) ;
    }

	_cur_tetromino = new clTetromino({_map_size.first/2,0},map_abs_dp_tl) ;

}

void clMap::removeLine(int idx){

    for(int i=0;i<_map_size.first;i++){

        _map[idx][i]=EMPTY_CELL ;
    }

    _reorganizeFrom(idx) ;
}

void clMap::removeLines(std::vector<int> &idx){

    for(int j=idx.size()-1;j>=0;j--){

        for(int i=0;i<_map_size.first;i++){

			_map[idx[j]][i] = EMPTY_CELL;

        }

		_reorganizeFrom(idx[j]);
    }

}

void clMap::_reorganizeFrom(int idx){

	int from_idx=idx ;

    for(int c=0;c<_map_size.first;c++){      

        for(int r=from_idx;r>0;r--)
        {
            _map[r][c]=_map[r-1][c] ;
            _map[r-1][c]=EMPTY_CELL ;
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

	std::pair<int,int> cut_tetr_tl = _cur_tetromino->getTetrominoTopLeft() ;
	std::vector<std::pair<int, int>> *shape = _cur_tetromino->getCurrentShape();

    bool collide=false ;

	for (auto pt : *shape) {

		if (cut_tetr_tl.second+pt.second + 1 >= _map_size.second ||
		 _map[cut_tetr_tl.second+pt.second + 1][cut_tetr_tl.first+pt.first] != EMPTY_CELL)
		{
			collide = true;
			break;
		}
	}

    if(!collide)
	{
		_cur_tetromino->move({0,1}) ;
	}
    
    return collide ;

}

void clMap::moveTetrominoHor(int dirX){

	std::pair<int,int> cur_tetr_tl = _cur_tetromino->getTetrominoTopLeft() ;
    int br = _cur_tetromino->getRight();

	if (cur_tetr_tl.first + dirX >= 0 && br + dirX < _map_size.first)
	{
		std::vector<std::pair<int, int>> *shape = _cur_tetromino->getCurrentShape();
		
		bool colide = false;

		for (auto pt : *shape)
		{
			if (_map[cur_tetr_tl.second+pt.second][cur_tetr_tl.first+pt.first+dirX] != EMPTY_CELL)
			{
				colide = true;
				break;
			}
		}

		if (!colide)
			_cur_tetromino->move({dirX,0}) ;
	}
		
}

void clMap::hardDrop() {

	std::pair<int,int> cur_tetr_tl = _cur_tetromino->getTetrominoTopLeft() ;
	std::vector<std::pair<int, int>> *shape = _cur_tetromino->getCurrentShape();

	bool collide = false;

	int down=0 ;

	while (!collide)
	{
		for (auto pt : *shape) {

			if (cur_tetr_tl.second + pt.second + down+1 >= _map_size.second || 
			_map[cur_tetr_tl.second + pt.second + down+1][cur_tetr_tl.first + pt.first] != EMPTY_CELL)
			{
				collide = true;
				break;
			}
		}

		if (!collide)
		{
			down++ ;
		}
	}

	_cur_tetromino->move({0,down}) ;
}

std::pair<int, int> clMap::_getShadowTopLeft() {

	std::vector<std::pair<int, int>> *shape = _cur_tetromino->getCurrentShape();

	bool collide = false;

	std::pair<int, int> tr_coll_tl = _cur_tetromino->getTetrominoTopLeft();

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

	bool above = false;

	for (int c = 0; c < _map_size.first; c++)
	{
		if (_map[_play_field_start_row][c] != EMPTY_CELL)
		{
			above = true;
			break;
		}
	}

	return above;
}

void clMap::rotateTromino90() {

	std::pair<int,int> cur_tetr_tl = _cur_tetromino->getTetrominoTopLeft() ;
	std::vector<std::pair<int,int>>* next_shape = _cur_tetromino->getNextShape();

	bool valid = true;

	for (auto pt : *next_shape)
	{
		auto new_pt = std::pair<int,int>{ cur_tetr_tl.first + pt.first, cur_tetr_tl.second + pt.second };

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
		_cur_tetromino->rotate90();
	}
}

void clMap::addBottomLine() {

	std::pair<int, int> cut_tetr_tl = _cur_tetromino->getTetrominoTopLeft();
	std::vector<std::pair<int, int>> *shape = _cur_tetromino->getCurrentShape();

	bool collide = false;

	for (auto pt : *shape) {

		if (cut_tetr_tl.second + pt.second + 1 >= _map_size.second ||
			_map[cut_tetr_tl.second + pt.second + 1][cut_tetr_tl.first + pt.first] != EMPTY_CELL)
		{
			collide = true;
			break;
		}
	}

	if (!collide)
	{
		for (int c = 0; c < _map_size.first; c++)
		{
			for (int r = 0; r < _map_size.second - 1; r++)
			{
				_map[r][c] = _map[r + 1][c];
			}
		}

		for (int c = 0; c < _map_size.first; c++)
		{
			_map[_map_size.second - 1][c] = ADD_BLOCK;
		}

		_map[_map_size.second - 1][std::rand() % _map_size.first] = EMPTY_CELL;

	}
}


void clMap::draw() {

	drawFrameUtil({ _map_dp_abs_tl.X-2,_map_dp_abs_tl.Y-1 }, _map_size.first+2, _map_size.second+2,
		MAP_FRAME_PATTERN, MAP_FRAME_PATTERN,
		MAP_FRAME_PATTERN, MAP_FRAME_PATTERN, MAP_FRAME_PATTERN, MAP_FRAME_PATTERN);

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO org_setting, new_setting;

	GetConsoleScreenBufferInfo(hdl, &org_setting);

	new_setting = org_setting;

	for (int r = 0; r < _map_size.second; r++) {

		for (int c = 0; c < _map_size.first; c++) {

			COORD coord = _map_dp_abs_tl;

			coord.X += c * 2;
			coord.Y += r;

			

			if (_map[r][c] == EMPTY_CELL)
			{			
				if (r != _play_field_start_row)
					drawXY(coord.X, coord.Y,MAP_PATTERN);
				else
					drawXY(coord.X, coord.Y, PLAY_FIELD_START_PATTERN);
			}
			else
			{
				new_setting.wAttributes = _map[r][c];
				SetConsoleTextAttribute(hdl, new_setting.wAttributes);
				drawXY(coord.X, coord.Y, TETROMINO_PATTERN);
			}

			SetConsoleTextAttribute(hdl, org_setting.wAttributes);
		}
	}

	drawStartLine() ;
}

void clMap::drawStartLine()
{
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO org_setting, new_setting;

	GetConsoleScreenBufferInfo(hdl, &org_setting);

	new_setting = org_setting;

	int tr_bt = _cur_tetromino->getBottom();

	if (_cur_tetromino->getTetrominoTopLeft().second-1 <= _play_field_start_row &&
		tr_bt-1 >= _play_field_start_row)
	{
 		for (int c = 0; c < _map_size.first; c++) {

			COORD coord = _map_dp_abs_tl;

			coord.X += c * 2;
			coord.Y += _play_field_start_row;

			if (_map[_play_field_start_row][c] == EMPTY_CELL)
				drawXY(coord.X, coord.Y, PLAY_FIELD_START_PATTERN);
			else
			{
				new_setting.wAttributes = _map[_play_field_start_row][c];
				SetConsoleTextAttribute(hdl, new_setting.wAttributes);
				drawXY(coord.X, coord.Y, TETROMINO_PATTERN);
			}

			SetConsoleTextAttribute(hdl, org_setting.wAttributes);
		}
	}
}

void clMap::drawRemovingRow(std::vector<int> &rm_rows) {

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
				COORD coord = _map_dp_abs_tl;

				coord.X += c * 2;
				coord.Y += r;

				drawXY(coord.X, coord.Y, pattern);
			}
		}

		long long clk = 0;
		
		while (clk++ < 35000000);
	}
}

void clMap::drawShadowTetromino()
{
	auto shadow_tr_tl = _getShadowTopLeft() ;
	auto *shape = _cur_tetromino->getCurrentShape() ;

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE) ;

	CONSOLE_SCREEN_BUFFER_INFO org_setting, new_setting;

	GetConsoleScreenBufferInfo(hdl, &org_setting);

	new_setting=org_setting ;
	
	new_setting.wAttributes=_cur_tetromino->getColor() ;

	SetConsoleTextAttribute(hdl, new_setting.wAttributes);

	for(auto pt : *shape)
	{
		COORD cursor_pos = { _map_dp_abs_tl.X + shadow_tr_tl.first*2 ,_map_dp_abs_tl.Y + shadow_tr_tl.second };
		
		cursor_pos.X += pt.first*2;
		cursor_pos.Y += pt.second;

		drawXY(cursor_pos.X, cursor_pos.Y, SHADOW_TETROMINO_PATTERN) ;
	}

	SetConsoleTextAttribute(hdl,org_setting.wAttributes) ;

}

void clMap::eraseShadowTetromino()
{
	auto shadow_tr_tl = _getShadowTopLeft() ;
	auto *shape = _cur_tetromino->getCurrentShape() ;

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE) ;

	for(auto pt : *shape)
	{
		COORD cursor_pos = { _map_dp_abs_tl.X + shadow_tr_tl.first*2 ,_map_dp_abs_tl.Y + shadow_tr_tl.second };

		cursor_pos.X += pt.first*2;
		cursor_pos.Y += pt.second;

		drawXY(cursor_pos.X, cursor_pos.Y,MAP_PATTERN) ;
	}
}

void clMap::setTetromino(clTetromino &tr){

    *_cur_tetromino=tr ;

	_cur_tetromino->setTopLeft({ _map_size.first / 2,0 });
	_cur_tetromino->setDpTopLeft(_map_dp_abs_tl) ;
}

void clMap::projectTetromino()
{
	auto shape = _cur_tetromino->getCurrentShape();
	std::pair<int,int> cur_tetr_tl = _cur_tetromino->getTetrominoTopLeft() ;
	

	for (auto pt : *shape)
	{
		_map[pt.second + cur_tetr_tl.second][pt.first + cur_tetr_tl.first] = _cur_tetromino->getColor();
	}
}

clTetromino &clMap::getTetromino(){

    return *_cur_tetromino ;
}

clMap::~clMap(){

    for(int r=0;r<_map_size.second;r++){

        delete[] _map[r] ;
    }

	delete _cur_tetromino ;
    delete[] _map ;
}