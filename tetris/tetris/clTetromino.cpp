#include "clTetromino.h"
#include <iostream>

stTetromino_Shape clTetromino::L_SHAPE= { L_SHAPE_TYPE,BLUE,{
							{{0,0},{0,1},{0,2},{1,2}},
							{{0,0},{1,0},{2,0},{0,1}},
							{{0,0},{1,0},{1,1},{1,2}},
							{{0,1},{1,1},{2,1},{2,0}}
							} };

stTetromino_Shape clTetromino::T_SHAPE = { T_SHAPE_TYPE,RED,{
										{{0,0},{1,0},{2,0},{1,1}},
										{{0,1},{1,0},{1,1},{1,2}},
										{{1,0},{0,1},{1,1},{2,1}},
										{{0,0},{0,1},{0,2},{1,1}}
} };

stTetromino_Shape clTetromino::I_SHAPE = { I_SHAPE_TYPE,GREEN,{
										{{0,0},{0,1},{0,2},{0,3}},
										{{0,0},{1,0},{2,0},{3,0}}} };

stTetromino_Shape clTetromino::J_SHAPE = { J_SHAPE_TYPE,SKY_BLUE,{
										{{1,0},{1,1},{1,2},{0,2}},
										{{0,0},{0,1},{1,1},{2,1}},
										{{0,0},{0,1},{0,2},{1,0}},
										{{0,0},{1,0},{2,0},{2,1}}
} };

stTetromino_Shape clTetromino::S_SHAPE = { S_SHAPE_TYPE,VIOLET,{
										{{0,1},{1,1},{1,0},{2,0}},
										{{0,0},{0,1},{1,1},{1,2}}
} };

stTetromino_Shape clTetromino::Z_SHAPE = { Z_SHAPE_TYPE,GRAY,{
										{{0,0},{1,0},{1,1},{2,1}},
										{{1,0},{1,1},{0,1},{0,2}}
} };

stTetromino_Shape clTetromino::O_SHAPE = { O_SHAPE_TYPE,YELLOW,{
										{{0,0},{1,0},{0,1},{1,1}},
} };

std::vector<stTetromino_Shape> clTetromino::SHAPES = { I_SHAPE,J_SHAPE, L_SHAPE,O_SHAPE,S_SHAPE, T_SHAPE,Z_SHAPE };

clTetromino::clTetromino(std::pair<int,int> init_tl, COORD cur_dp_abs_tl,int shape_type){

    _cur_shape = &SHAPES[shape_type] ;
	_shape_type = shape_type;
	_rot_pos = 0;

	_cur_tl = init_tl ;
	_cur_dp_abs_tl=cur_dp_abs_tl ;
}


int clTetromino::getRight() {

	int max = -1;

	for (auto pt : _cur_shape->points_per_rot[_rot_pos])
	{
		if (max < pt.first)
		{
			max = pt.first;
		}
	}

	return _cur_tl.first+max;
}

int clTetromino::getBottom() {

	int max = -1;

	for (auto pt : _cur_shape->points_per_rot[_rot_pos])
	{
		if (max < pt.second)
		{
			max = pt.second;
		}
	}

	return _cur_tl.second+max ;
}

std::pair<int,int> clTetromino::getTetrominoTopLeft(){

	return _cur_tl ;
}

std::vector<std::pair<int,int>>* clTetromino::getCurrentShape(){

	return &_cur_shape->points_per_rot[_rot_pos];
}

int clTetromino::getType() {

	return _cur_shape->type;
}

int clTetromino::getColor() {

	return _cur_shape->color;
}

void clTetromino::rotate90(){

    _rot_pos = (_rot_pos+1)%SHAPES[_shape_type].points_per_rot.size() ;
}

void clTetromino::move(std::pair<int,int> offset)
{
	_cur_tl.first+=offset.first ;
	_cur_tl.second+=offset.second ;
}

std::vector<std::pair<int, int>> *clTetromino::getNextShape() {

	int next_rot_pos = (_rot_pos + 1) % SHAPES[_shape_type].points_per_rot.size();

	return &SHAPES[_shape_type].points_per_rot[next_rot_pos];
}

void clTetromino::draw(const char* pattern) {

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO buf_info;
	GetConsoleScreenBufferInfo(hdl, &buf_info);

	SetConsoleTextAttribute(hdl, _cur_shape->color);

	for (auto pt : _cur_shape->points_per_rot[_rot_pos])
	{
		COORD coord = { _cur_dp_abs_tl.X+_cur_tl.first*2, _cur_dp_abs_tl.Y+_cur_tl.second };

		coord.X += pt.first*2;
		coord.Y += pt.second;

		drawXY(coord.X, coord.Y,pattern);
	}

	SetConsoleTextAttribute(hdl, buf_info.wAttributes);
}

void clTetromino::drawFrame(const char *pattern) {

	drawFrameUtil({_cur_dp_abs_tl.X-4, _cur_dp_abs_tl.Y - 2 },
		getRight() - _cur_tl.first+5,
		getBottom() - _cur_tl.second+5,
		pattern, pattern,
		pattern, pattern, pattern, pattern);
}

void clTetromino::eraseFrame(const char *pattern)
{
	drawFrameUtil({ _cur_dp_abs_tl.X - 4, _cur_dp_abs_tl.Y - 2 },
		getRight() - _cur_tl.first+5,
		getBottom() - _cur_tl.second+5,
		pattern, pattern,
		pattern, pattern, pattern, pattern);
}

void clTetromino::erase(const char* pattern) {

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	int pattern_len = std::strlen(pattern);

	for (auto pt : _cur_shape->points_per_rot[_rot_pos])
	{
		COORD coord = { _cur_dp_abs_tl.X + _cur_tl.first*2, _cur_dp_abs_tl.Y + _cur_tl.second };

		coord.X += pt.first*2;
		coord.Y += pt.second;

		drawXY(coord.X,coord.Y,pattern);
	}

}

void clTetromino::setTopLeft(std::pair<int,int> top_left)
{
	_cur_tl = top_left ;
}
	
void clTetromino::setDpTopLeft(COORD dp_top_left)
{
	_cur_dp_abs_tl = dp_top_left ;
}


clTetromino clTetromino::getRandomTetrominoFormat(std::pair<int,int> init_tl, COORD cur_abs_dp_tl){

    int type = rand()%SHAPES.size() ;

	return clTetromino(init_tl, cur_abs_dp_tl, type);
}

COORD clTetromino::getDpTopLeft(){

	return _cur_dp_abs_tl ;
}

