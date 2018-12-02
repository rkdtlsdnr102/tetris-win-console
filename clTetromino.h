#pragma once

#ifndef CL_TETROMINO_H
#define CL_TETROMINO_H
#include <utility>
#include <vector>
#include <Windows.h>

#define EMPTY_CELL -1
#define TETROMINO_PATTERN "бс"
#define SHADOW_TETROMINO_PATTERN "бр"


#define I_SHAPE_TYPE 10
#define J_SHAPE_TYPE 20
#define L_SHAPE_TYPE 30
#define O_SHAPE_TYPE 40
#define S_SHAPE_TYPE 50
#define T_SHAPE_TYPE 60
#define Z_SHAPE_TYPE 70


enum CONSOLE_TEXT_COLOR : int{
	BLACK,      /*  0 : BLACK */
	DARK_BLUE,    /*  1 : DARK_BLUE */
	DARK_GREEN,    /*  2 : DARK_GREEN */
	DARK_SKY_BLUE,  /*  3 : DARK_SKY_BLUE */
	DARK_RED,    /*  4 : DARK_RED */
	DARK_VOILET,  /*  5 : DARK_VOILET */
	DARK_YELLOW,  /*  6 : DARK_YELLOW */
	GRAY,      /*  7 : GRAY */
	DARK_GRAY,    /*  8 : DARK_GRAY */
	BLUE,      /*  9 : BLUE */
	GREEN,      /* 10 : GREEN */
	SKY_BLUE,    /* 11 : SKY_BLUE */
	RED,      /* 12 : RED */
	VIOLET,      /* 13 : VIOLET */
	YELLOW,      /* 14 : YELLOW */
	WHITE,      /* 15 : WHITE */
};

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
	int _shape_type;
	int _rot_pos;

public:
	clTetromino(int shape_type = 0);
    void rotate90() ;
	std::vector<std::pair<int, int>>* getCurrentShape() ;
	std::vector<std::pair<int,int>>* getNextShape() ;
	int getRight();
	int getBottom();
	int getType();
	int getColor();
	void draw(COORD cursor_pos, const char* pattern=TETROMINO_PATTERN);
	void erase(COORD cursor_pos, const char* pattern);
	static clTetromino createRandomTetromino();

};

#endif