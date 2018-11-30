#pragma once

#ifndef CL_TETROMINO_H
#define CL_TETROMINO_H
#include <utility>
#include <vector>
#include <Windows.h>

#define EMPTY_CELL 0
#define TETROMINO_PATTERN "бс"

#define L_SHAPE_TYPE 1
#define T_SHAPE_TYPE 2
#define I_SHAPE_TYPE 3
#define J_SHAPE_TYPE 4
#define S_SHAPE_TYPE 5
#define Z_SHAPE_TYPE 6
#define O_SHAPE_TYPE 7

struct stTetromino_Shape {

	int type;
	int color;
	std::vector<std::vector<std::pair<int, int>>> points_per_rot;
};


class clTetromino{

private:
	static stTetromino_Shape I_SHAPE;
	static stTetromino_Shape J_SHAPE;
	static stTetromino_Shape L_SHAPE;
	static stTetromino_Shape O_SHAPE;
	static stTetromino_Shape S_SHAPE;
	static stTetromino_Shape T_SHAPE;
	static stTetromino_Shape Z_SHAPE;

	static std::vector<stTetromino_Shape> SHAPES ;

private:
    stTetromino_Shape* _cur_shape ;
	std::vector<std::pair<int, int>> *_prev_shape;
    int _shape_type ;
	int _rot_pos;

public:
	clTetromino(int shape_type = 0);
    void rotate90() ;
	std::vector<std::pair<int, int>>* getCurrentShape() ;
	std::vector<std::pair<int,int>>* getNextShape() ;
	int getRight();
	int getBottom();
	int getType();
	void draw(COORD cursor_pos);
	void erase(COORD cursor_pos, const char* pattern);
	static clTetromino createRandomTetromino();

};

#endif