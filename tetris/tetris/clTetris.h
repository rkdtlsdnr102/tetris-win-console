#pragma once

#include <utility>
#include "clMap.h"
#include "clTetromino.h"
#include "clStage.h"

class clTetris{

	enum GAME_READY_SCREEN_CHOICE : int{

		GAME_PLAY,
		VIEW_RECORD,
		EXIT
	} ;
	struct GameRecord{

		std::string name ;
		int play_time ;
		int stage ;
		int score ;
		static void loadRecord(std::string filepath, GameRecord *playerRecord) ;
		static void saveRecord(std::string filepath, GameRecord &record) ;
	};

private:

	int _cur_stage_idx ;
	GameRecord _playerGameRecord ;

	std::string _bgm_file_path ;

	clStage *_stages;
	
	GAME_READY_SCREEN_CHOICE _gameReadyScreen() ;
	void _gameRecordScreen() ;
	void _drawGameResult(COORD cursor_pos, const char* msg) ;

	
public:
    clTetris(std::string bgm_file_path) ;
    void run() ;
	~clTetris();


};