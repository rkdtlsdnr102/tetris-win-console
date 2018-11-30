#include "clTetromino.h"
#include <ctime>
#include <iostream>

stTetromino_Shape clTetromino::L_SHAPE= { L_SHAPE_TYPE,FOREGROUND_BLUE | FOREGROUND_INTENSITY,{
							{{0,0},{0,1},{0,2},{1,2}},
							{{0,0},{1,0},{2,0},{0,1}},
							{{0,0},{1,0},{1,1},{1,2}},
							{{0,1},{1,1},{2,1},{2,0}}
							} };

stTetromino_Shape clTetromino::T_SHAPE = { T_SHAPE_TYPE,FOREGROUND_RED | FOREGROUND_INTENSITY,{
										{{0,0},{1,0},{2,0},{1,1}},
										{{0,1},{1,0},{1,1},{1,2}},
										{{1,0},{0,1},{1,1},{2,1}},
										{{0,0},{0,1},{0,2},{1,1}}
} };

stTetromino_Shape clTetromino::I_SHAPE = { O_SHAPE_TYPE,FOREGROUND_GREEN | FOREGROUND_INTENSITY,{
										{{0,0},{0,1},{0,2},{0,3}},
										{{0,0},{1,0},{2,0},{3,0}}} };

stTetromino_Shape clTetromino::J_SHAPE = { O_SHAPE_TYPE,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,{
										{{1,0},{1,1},{1,2},{0,2}},
										{{0,0},{0,1},{1,1},{2,1}},
										{{0,0},{0,1},{0,2},{1,0}},
										{{0,0},{1,0},{2,0},{2,1}}
} };

stTetromino_Shape clTetromino::S_SHAPE = { O_SHAPE_TYPE,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,{
										{{0,1},{1,1},{1,0},{2,0}},
										{{0,0},{0,1},{1,1},{1,2}}
} };

stTetromino_Shape clTetromino::Z_SHAPE = { O_SHAPE_TYPE,FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,{
										{{0,0},{1,0},{1,1},{2,1}},
										{{1,0},{1,1},{0,1},{0,2}}
} };

stTetromino_Shape clTetromino::O_SHAPE = { O_SHAPE_TYPE,FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,{
										{{0,0},{1,0},{0,1},{1,1}},
} };

std::vector<stTetromino_Shape> clTetromino::SHAPES = { I_SHAPE,J_SHAPE, L_SHAPE,O_SHAPE,S_SHAPE, T_SHAPE,Z_SHAPE };
clTetromino::clTetromino(int shape_type){

    _shape_type=shape_type ;
    _cur_shape = &SHAPES[shape_type] ;
	_rot_pos = 0;
	_prev_shape = &(_cur_shape->points_per_rot[_rot_pos]);


	srand((unsigned)time(nullptr));
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

	return max;
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

	return max ;
}

std::vector<std::pair<int,int>>* clTetromino::getCurrentShape(){

	return &_cur_shape->points_per_rot[_rot_pos];
}

int clTetromino::getType() {

	return _cur_shape->type;
}

void clTetromino::rotate90(){

	_prev_shape = &(_cur_shape->points_per_rot[_rot_pos]);
    _rot_pos = (_rot_pos+1)%SHAPES[_shape_type].points_per_rot.size() ;
}

std::vector<std::pair<int, int>> *clTetromino::getNextShape() {

	int next_rot_pos = (_rot_pos + 1) % SHAPES[_shape_type].points_per_rot.size();

	return &SHAPES[_shape_type].points_per_rot[next_rot_pos];
}

void clTetromino::draw(COORD cursor_pos) {

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO buf_info;
	GetConsoleScreenBufferInfo(hdl, &buf_info);

	SetConsoleTextAttribute(hdl, _cur_shape->color);

	for (auto pt : _cur_shape->points_per_rot[_rot_pos])
	{
		COORD coord = cursor_pos;

		coord.X += pt.first*2;
		coord.Y += pt.second;

		SetConsoleCursorPosition(hdl, coord);

		puts(TETROMINO_PATTERN);
	}

	SetConsoleTextAttribute(hdl, buf_info.wAttributes);
}

void clTetromino::erase(COORD cursor_pos, const char* pattern) {

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

	for (auto pt : _cur_shape->points_per_rot[_rot_pos])
	{
		COORD coord = cursor_pos;

		coord.X += pt.first*2;
		coord.Y += pt.second;

		SetConsoleCursorPosition(hdl, coord);

		puts(pattern);
	}

}



clTetromino clTetromino::createRandomTetromino(){

    int type = rand()%SHAPES.size() ;

	return clTetromino(type);
}

