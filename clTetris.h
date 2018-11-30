#pragma once

#include <utility>
#include "clMap.h"
#include "clTetromino.h"

class clTetris{

private:
    clMap *_map ;
	COORD _map_dp_tl;

    clTetromino _next_tetromino ;
	COORD _next_tetromino_dp_tl;
	
    int _rm_lines_cnt ;
	COORD _rm_lines_dp_tl;
	int _fps;

    void userInput() ;
    void _handleCollision() ;
	void _drawRemainingCount(COORD cursor_pos);
	void _drawNextTetromino(COORD cursor_pos);
	void _drawShadowTetromino(COORD cursor_pos);

public:
    clTetris(int fps=15) ;
    void run() ;
	~clTetris();


};