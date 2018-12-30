#pragma once

#include <utility>
#include "clMap.h"
#include "clTetromino.h"
#include "clStage.h"
#include "stGameRecord.h"

class clTetris{

	enum GAME_READY_SCREEN_CHOICE : int{

		GAME_PLAY,
		VIEW_RECORD,
		EXIT
	} ;

private:

	int _cur_stage_idx ;
	stGameRecord _playerGameRecord ;

	std::string _bgm_file_path ;

	clStage *_stages;
	
	GAME_READY_SCREEN_CHOICE _gameReadyScreen() ;
	void _gameRecordScreen() ;
	void _drawGameResult(COORD cursor_pos, const char* msg) ;

	static const std::string _record_file_path;
	
public:
    clTetris() ;
    void run() ;
	~clTetris();


};