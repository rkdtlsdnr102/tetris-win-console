#include "clTetromino.h"
#include "clMap.h"
#include "clTimer.h"
#include "stGameRecord.h"

#define EMPTY_PATTERN  " "
#define EMPTY_PATTERN_DOUBLE "  "

class clStage{

public:
    enum GAME_RESULT : int {

		PLAYER_WIN,
		PLAYER_LOSE
	};

    enum GAME_STATE : int {
		GAME_RUNNING,
		GAME_OVER
	};

private:

    clTetromino *_next_tetromino ;

    clMap *_map ;
    int _stage_num ;
	COORD _stage_num_abs_dp_tl;

    int _total_rm_lines_cnt ;
    
    int _add_row_period_milli ;
    int _down_period_milli ;

    int _rm_lines_cnt ;
    COORD _rm_lines_cnt_dp_tl ;

    std::string _bgm_file_path ;

	COORD _score_dp_tl;
	int _score;

	stGameRecord *_cur_player_record;
	

	GAME_STATE _g_state;
	GAME_RESULT _latest_result;

	void _userInput();
	void _handleCollision();
	void _reDrawRemovedLinesCount();
	void _reDrawScore();
	void _drawStageNum();

    static std::string _collision_sound_file_path ;

public:
	//basic constructor, do nothing, for dynamic array allocation
    clStage() ;
    clStage(int down_period_milli, int total_rm_lines_cnt, int add_row_period_milli, int stage_num, 
		std::string bgm_file_path) ;
    GAME_RESULT run() ;
	void initialize(int down_period_milli, int total_rm_lines_cnt, int add_row_period_milli, int stage_num, 
		std::string bgm_file_path);
	void setPlayerRecord(stGameRecord *record);
	GAME_RESULT getLatestResult();
	~clStage();
   
};