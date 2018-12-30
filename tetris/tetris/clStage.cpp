#include "clStage.h"
#include "tetrisUtility.h"
#include <iostream>
#include <iomanip>

#define ROTATE_KEY 0x52
#define HARD_DROP_KEY 0x20

const char THEMES[3][40] = { "../stage1.wav" ,
						"../stage2.wav" ,
						"../stage3.wav" };

clStage::clStage(){

}

clStage::clStage(int down_period_milli, int total_rm_lines_cnt, int add_row_period_milli, 
	int stage_num)
{
	initialize(down_period_milli, total_rm_lines_cnt, add_row_period_milli, stage_num);
}

void clStage::initialize(int down_period_milli, int total_rm_lines_cnt, int add_row_period_milli, 
	int stage_num)
{
	_map = new clMap({ 10,20 }, { 3,2 });

	_stage_num = stage_num;
	_stage_num_abs_dp_tl = { 29,3 };

	_rm_lines_cnt = 0;
	_rm_lines_cnt_dp_tl = { 29,6 };

	_next_tetromino = new clTetromino({ 0,0 }, { 31,13 });

	_total_rm_lines_cnt = total_rm_lines_cnt;

	_add_row_period_milli = add_row_period_milli;
	_down_period_milli = down_period_milli;

	_bgm_file_path = THEMES[_stage_num-1];

	clTimer::getInstance()->setDpTopLeft({ 29,20 });

	_score_dp_tl = {29,23};

	_score = 0;

	_g_state = GAME_STATE::GAME_RUNNING;
	_latest_result = GAME_RESULT::PLAYER_WIN;

}

void clStage::setPlayerRecord(stGameRecord *player_record) {

	_cur_player_record = player_record;
}


clStage::GAME_RESULT clStage::run()
{
	int down_clk_cnt=0, add_row_clk_cnt=0 ;

	HWND console_hwnd = GetConsoleWindow();

	MoveWindow(console_hwnd, 100, 100, 600, 500, TRUE);

	_map->draw();
	_map->getTetromino().draw();
	_next_tetromino->draw();
	_next_tetromino->drawFrame();
	_reDrawRemovedLinesCount();
	
	_drawStageNum();
	
	_score = _cur_player_record->score;
	_reDrawScore();

	PlaySoundA(_bgm_file_path.c_str(), NULL,
		SND_FILENAME | SND_ASYNC | SND_LOOP);

	clTimer::getInstance()->setStartTime(_cur_player_record->play_time_milli);
	clTimer::getInstance()->start();

	while (1)
	{

		down_clk_cnt++;
		add_row_clk_cnt++;

		_userInput();

        if(_g_state==GAME_OVER)
            break ;

		if (add_row_clk_cnt == _add_row_period_milli)
		{
			_map->eraseShadowTetromino();
			_map->getTetromino().erase(MAP_PATTERN);

			_map->addBottomLine();

			_map->draw();

			_map->drawShadowTetromino();
			_map->getTetromino().draw();

			add_row_clk_cnt = 0;

			if (_map->abovePlayfieldStart())
			{
				_g_state = GAME_OVER;
				_latest_result = PLAYER_LOSE;
			}
		}

		if (down_clk_cnt == _down_period_milli)
		{
			_map->eraseShadowTetromino();
			_map->getTetromino().erase(MAP_PATTERN);

			if (_map->moveTetrominoDown())
			{
				_handleCollision();

				_map->draw();
			}

			_map->drawStartLine();

			_map->drawShadowTetromino();
			_map->getTetromino().draw();

			down_clk_cnt = 0;
		}

		if (_g_state == GAME_OVER)
			break;
	}

	PlaySoundA(NULL, 0, 0);

	clTimer::getInstance()->stop();

	//update current user record for this stage
	_cur_player_record->play_time_milli=clTimer::getInstance()->getCurrentTimeMilli();
	_cur_player_record->final_stage = _stage_num;
	_cur_player_record->score = _score;

	//clear stage
	_next_tetromino->erase(EMPTY_PATTERN);
	_next_tetromino->eraseFrame(EMPTY_FRAME_PATTERN);

    return _latest_result ;
 
}

void clStage::_userInput()
{

    SHORT left = GetAsyncKeyState(VK_LEFT);
	SHORT right = GetAsyncKeyState(VK_RIGHT);
	SHORT down = GetAsyncKeyState(VK_DOWN);
	SHORT rotate = GetAsyncKeyState(ROTATE_KEY);
	SHORT hard_drop = GetAsyncKeyState(HARD_DROP_KEY);

	SHORT res;

	if (res=((left & 0x0001) | (right & 0x0001) | (down & 0x0001) | (rotate & 0x0001) | (hard_drop & 0x0001)))
	{
		_map->eraseShadowTetromino() ;
		_map->getTetromino().erase(MAP_PATTERN);

		if ((left & 0x0001))
		{			
			_map->moveTetrominoHor(-1);		
		}
		else if ((right & 0x0001))
		{			
			_map->moveTetrominoHor(1);			
		}
		else if ((down & 0x0001))
		{
			if (_map->moveTetrominoDown())
			{	
				_handleCollision();

				_map->draw();
			}
		}
		else if ((hard_drop & 0x0001))
		{			

			_map->hardDrop();

			_handleCollision();

			_map->draw();
			
		}

		if ((rotate & 0x0001))
		{			
			_map->rotateTromino90();			
		}

		_map->drawStartLine();

		_map->drawShadowTetromino() ;
		_map->getTetromino().draw();
	}
}




void clStage::_handleCollision() {

	if (_map->abovePlayfieldStart())
	{
		_g_state = GAME_OVER;
		_latest_result = PLAYER_LOSE;
	}
	else
	{

		clTetromino cur_tr = _map->getTetromino();

		auto cur_tr_tl = cur_tr.getTetrominoTopLeft();

		_map->projectTetromino();

		_map->draw();


		std::vector<int> remove_rows;

		for (int r = cur_tr.getBottom(); r >= cur_tr_tl.second; r--)
		{
			if (_map->isLineConnected(r))
				remove_rows.push_back(r);
		}

		if (remove_rows.size() > 0)
		{
			_map->drawRemovingRow(remove_rows);

			_map->removeLines(remove_rows);

			_map->draw();

			_rm_lines_cnt += remove_rows.size();

			_score += remove_rows.size();

			_reDrawRemovedLinesCount();
			_reDrawScore();
		}

		_map->setTetromino(*_next_tetromino);

		_next_tetromino->erase(EMPTY_PATTERN);
		_next_tetromino->eraseFrame(EMPTY_FRAME_PATTERN);

		*_next_tetromino = clTetromino::getRandomTetrominoFormat(
			_next_tetromino->getTetrominoTopLeft(),
			_next_tetromino->getDpTopLeft());

		_next_tetromino->draw();
		_next_tetromino->drawFrame();

		if (_rm_lines_cnt >= _total_rm_lines_cnt)
		{
			_g_state = GAME_OVER;
			_latest_result = PLAYER_WIN;
		}

	}
}

void clStage::_drawStageNum() {

	drawFrameUtil({ _stage_num_abs_dp_tl.X - 3,_stage_num_abs_dp_tl.Y - 1 }, 13, 3, 
		"─", "│", "┌", "┐", "└", "┘");

	char str[20] = { '\0' };

	std::sprintf(str, "Stage: %d", _stage_num);

	drawXY(_stage_num_abs_dp_tl.X,_stage_num_abs_dp_tl.Y,str);

}


void clStage::_reDrawRemovedLinesCount() {

	drawFrameUtil({ _rm_lines_cnt_dp_tl.X - 3,_rm_lines_cnt_dp_tl.Y - 1 }, 13, 3,
		"─", "│", "┌", "┐", "└", "┘");

	char str[20] = { '\0' };

	std::strcpy(str, "                   ");

	drawXY(_rm_lines_cnt_dp_tl.X, _rm_lines_cnt_dp_tl.Y, str);

	std::sprintf(str, "Removed lines: %d", _rm_lines_cnt);

	drawXY(_rm_lines_cnt_dp_tl.X,_rm_lines_cnt_dp_tl.Y, str);
}

void clStage::_reDrawScore() {

	drawFrameUtil({ _score_dp_tl.X - 3,_score_dp_tl.Y - 1 }, 13, 3,
		"─", "│", "┌", "┐", "└", "┘");

	char str[20] = { '\0' };

	std::strcpy(str, "                   ");

	drawXY(_score_dp_tl.X, _score_dp_tl.Y, str);

	std::sprintf(str, "Score: %d", _score);

	drawXY(_score_dp_tl.X, _score_dp_tl.Y, str);
}

clStage::GAME_RESULT clStage::getLatestResult() {

	return _latest_result;
}

clStage::~clStage() {

	delete _map;
	delete _next_tetromino;
}

