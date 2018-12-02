#pragma once

#include <utility>
#include "clMap.h"
#include "clTetromino.h"

class clTetris{

	enum GAME_STATE : int {
		GAME_RUNNING,
		GAME_OVER
	};

	enum GAME_RESULT : int {

		PLAYER_WIN,
		PLAYER_LOSE
	};

private:
    clMap *_map ;
	COORD _map_dp_tl;

    clTetromino _next_tetromino ;
	COORD _next_tetromino_dp_tl;
	
    int _rm_lines_cnt ;
	COORD _rm_lines_dp_tl;
	int _fps;

	COORD _explain_frame_dp_tl;

	GAME_STATE _g_state;
	GAME_RESULT _last_result;

    void userInput() ;
    void _handleCollision() ;
	void _drawRemainingCount(COORD cursor_pos);
	void _drawNextTetromino(COORD cursor_pos);
	void _drawExplain(COORD cursor_pos);

public:
    clTetris(int fps=15) ;
    void run() ;
	~clTetris();


};