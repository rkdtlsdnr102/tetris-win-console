#include "clTetris.h"
#include "tetrisUtility.h"
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <mmsystem.h>
#include <algorithm>

#define EMPTY_PATTERN  " "
#define EMPTY_PATTERN_DOUBLE "  "
#define SCREEN_FRAME_PATTERN "■"

const std::string clTetris::_record_file_path = "../player_records.dat";


clTetris::clTetris(std::string bgm_file_path)
{

	_bgm_file_path=bgm_file_path ;

	_cur_stage_idx=0 ;

}

void clTetris::run() 
{
	while(1)
	{
		GAME_READY_SCREEN_CHOICE chosen_menu = _gameReadyScreen();

		if (chosen_menu == GAME_READY_SCREEN_CHOICE::GAME_PLAY)
		{
			_stages = new clStage[3];

			stGameRecord player_record = { "",0,0,0 };

			_stages[0].initialize(100000, 5, 800000, 1,
				"C:\\Users\\USER\\Desktop\\tetris\\tetris-win-console\\tetris\\Debug\\BGM_Tetris_Bradinsky.wav");
			_stages[1].initialize(90000, 10, 700000, 2,
				"C:\\Users\\USER\\Desktop\\tetris\\tetris-win-console\\tetris\\Debug\\BGM+Tetris+Kalinka.wav");
			_stages[2].initialize(80000, 15, 600000, 3,
				"C:\\Users\\USER\\Desktop\\tetris\\tetris-win-console\\tetris\\Debug\\BGM_Tetris_Bradinsky.wav");

			clStage::GAME_RESULT res = clStage::GAME_RESULT::PLAYER_WIN;

			system("cls");

			while (_cur_stage_idx <= 2 && res == clStage::GAME_RESULT::PLAYER_WIN)
			{
				_stages[_cur_stage_idx].setPlayerRecord(&player_record);
				res=_stages[_cur_stage_idx].run();
				_cur_stage_idx++;
			}

			if (_cur_stage_idx > 2 && res == clStage::GAME_RESULT::PLAYER_WIN)
			{
				drawXY(25, 25, "PLAYER WIN!!!");
			}
			else
			{
				drawXY(25, 25, "PLAYER LOSE~~~");
			}

			player_record.name = "player_temp";

			while (1)
			{
				if ((GetAsyncKeyState(VK_RETURN) & 0x0001))
					break;
			}

			//save user game record

			stGameRecord::saveRecord(player_record, _record_file_path);

			//clear stages
			delete[] _stages;//retrieve memory

			_cur_stage_idx = 0;
			clTimer::getInstance()->reset();
		}
		else if (chosen_menu == GAME_READY_SCREEN_CHOICE::VIEW_RECORD)
		{
			_gameRecordScreen();
		}
		else if (chosen_menu==GAME_READY_SCREEN_CHOICE::EXIT)
		{
			break;
		}
	}
}

clTetris::GAME_READY_SCREEN_CHOICE clTetris::_gameReadyScreen(){

	int i,j;

	HWND console_hwnd = GetConsoleWindow();

	MoveWindow(console_hwnd, 100, 100, 800, 500, TRUE);

	system("cls");

	int text_battle_x=3;
	int text_battle_y=2;
	
	int text_tetris_x=16;
	int text_tetris_y=9;
	
	int text_pressKey_x=20;
	int text_pressKey_y=15;

	PlaySoundA(_bgm_file_path.c_str(), NULL,
		SND_FILENAME | SND_ASYNC | SND_LOOP);

	int text_battle_order[9]={0,1,2,3,4,2,1,4,2};
	int text_battle [5][6][25]={
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,

		1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,0,
		1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,
		1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,0,0,
		1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,
		1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,
		0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,
		0,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,0,
		0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,
		0,1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,

		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,0,
		1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,
		1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,0,0,
		1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,
		1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,0,

		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,
		0,0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,
		0,0,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,
		0,0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,
		0,0,1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1
	};

	int text_tetris [5][21]={
		1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,0,1,0,1,1,1,
		0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,0,
		0,1,0,0,1,1,1,0,0,1,0,0,1,1,0,0,1,0,1,1,1,
		0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1,
		0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,0,1,0,1,1,1
	};

	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE) ;
	
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0003);
	for(int k=1;k<9;k++){
		for(i=0;i<6;i++){
			for(j=0;j<25;j++){
				if(text_battle[text_battle_order[k]][i][j]!=text_battle[text_battle_order[k-1]][i][j]){				
					if(text_battle[text_battle_order[k]][i][j]==1) drawXY((2 * (text_battle_x + j)), (SHORT)(text_battle_y + i), "■");
					else if(text_battle[text_battle_order[k]][i][j]==0) drawXY((2 * (text_battle_x + j)), (SHORT)(text_battle_y + i), " ");
				}
			}
		}
		Sleep(100);
	}
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0007);
	Sleep(200);


	COORD explain_frame_dp_tl = { 8,15 };
	//draw explain
	drawFrameUtil(explain_frame_dp_tl, 15, 11,
		"─", "│", "┌", "┐", "└", "┘");

	drawXY(explain_frame_dp_tl.X + 4,explain_frame_dp_tl.Y + 2,
		"LEFT RIGHT MOVE: ←,→");

	drawXY( explain_frame_dp_tl.X + 4,explain_frame_dp_tl.Y + 4,
		"MOVE 1 DOWN: ↓");
	drawXY(explain_frame_dp_tl.X + 4,explain_frame_dp_tl.Y + 6 ,
		"HARD DROP: SPACE");
	drawXY(explain_frame_dp_tl.X + 4,explain_frame_dp_tl.Y + 8 ,
		"ROTATION(clockwise): R");

	drawXY(50,18, "START");
	drawXY(50,21, "RECORD");
	drawXY(50,24, "EXIT");

	int arrow_idx=0;
	COORD arrow_pos = { 47,18 };//pointing START menu

	drawXY(arrow_pos.X, arrow_pos.Y, "▶");

	CONSOLE_SCREEN_BUFFER_INFO org_binfo;
	GetConsoleScreenBufferInfo(hdl, &org_binfo);

	SHORT key = 0;

	for (int cnt = 0;; cnt++) {
		Sleep(10);

		if (cnt % 75 == 0) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cnt % 7 + 2);
			for (i = 0; i < 5; i++) {
				for (j = 0; j < 21; j++) {					
					if (text_tetris[i][j] == 1) drawXY(2 * (text_tetris_x + j), text_tetris_y + i,"▣");
				}
			}
		}

		SetConsoleTextAttribute(hdl, org_binfo.wAttributes);

		
		
		if (GetAsyncKeyState(VK_DOWN) & 0x0001)
		{
			if (arrow_idx < 2)
			{
				drawXY(arrow_pos.X, arrow_pos.Y, " ");

				arrow_pos.Y+=3;

				drawXY(arrow_pos.X, arrow_pos.Y, "▶");

				arrow_idx++;
			}
		}
		else if(GetAsyncKeyState(VK_UP) & 0x0001)
		{
			if (arrow_idx > 0)
			{
				drawXY(arrow_pos.X, arrow_pos.Y, " ");

				arrow_pos.Y-=3;

				drawXY(arrow_pos.X, arrow_pos.Y, "▶");

				arrow_idx--;
			}
		}
		
		//엔터키
		if ((GetAsyncKeyState(VK_RETURN) & 0x8001)==0x8000) break;
	}

	GAME_READY_SCREEN_CHOICE choice;

	switch (arrow_idx)
	{
	case 0:
		choice = GAME_PLAY;
		break;

	case 1:
		choice = VIEW_RECORD;
		break;

	case 2:
		choice = EXIT;
		break;
	}

	return	choice;
}


std::string make_tuple(std::vector<std::string> &vec,int col_max_width=20, int col_pad=3) {
	
	int col_offset = 0 ;

	std::string whole_str = "", col_str;

	col_str.resize(col_max_width, ' ');

	for (auto &str : vec)
	{
		auto beg = col_str.begin() + (col_max_width - str.length()) / 2;
		auto end = beg + str.length();
		col_str.replace(beg, end, str.c_str());
		col_offset = col_offset + col_pad + col_max_width;
		whole_str += col_str;
		std::fill(col_str.begin(), col_str.end(), ' ');
	}

	return whole_str;
}

void clTetris::_gameRecordScreen() {

	HWND console_hwnd = GetConsoleWindow();

	MoveWindow(console_hwnd, 100, 100, 1000, 500, TRUE);

	system("cls");

	drawFrameUtil({ 5,3 }, 52,20,
		"─", "│", "┌", "┐", "└", "┘");

	std::vector<std::string> name_cols = {"RANK","NAME","TIME","STAGE","SCORE"};

	std::string name_tuple = make_tuple(name_cols, 20, 3);
	
	drawXY(7, 5, name_tuple.c_str());

	drawXY(7, 26, "PREV_PAGE : <-");
	drawXY(7, 27, "NEXT_PAGE : ->");
	drawXY(7, 28, "RETURN : ESC");

	std::vector<stGameRecord> game_records;

	stGameRecord::loadRecord(&game_records, _record_file_path);

	int rec_per_view=10,cur_view_rec_start_idx=0;

	//draw initial view
	auto rec_it = game_records.begin() + cur_view_rec_start_idx;
	auto rec_end = game_records.end();
	
	if (cur_view_rec_start_idx + rec_per_view <= (int)game_records.size())
		rec_end = game_records.begin() + cur_view_rec_start_idx + rec_per_view ;

	for (int l = 0; rec_it != rec_end; rec_it++, l++)
	{
		std::vector<std::string> cols = { std::to_string(l + 1),rec_it->name,
				std::to_string(rec_it->play_time_milli),
				std::to_string(rec_it->final_stage),
				std::to_string(rec_it->score) };

		std::string tuple = make_tuple(cols, 20, 3);

		drawXY(7, 7 + l, tuple.c_str());
	}
	
	while (1)
	{
		bool view_update = false;

		SHORT LEFT_ARROW = GetAsyncKeyState(VK_LEFT);
		SHORT RIGHT_ARROW = GetAsyncKeyState(VK_RIGHT);
		SHORT ESC = GetAsyncKeyState(VK_ESCAPE);

		if(LEFT_ARROW & 0x0001)
		{
			if (cur_view_rec_start_idx - rec_per_view >= 0)
			{
				cur_view_rec_start_idx -= rec_per_view;

				view_update = true;
			}

		}

		if(RIGHT_ARROW & 0x0001)
		{
			if (cur_view_rec_start_idx + rec_per_view < (int)game_records.size())
			{
				cur_view_rec_start_idx += rec_per_view;

				view_update = true;
			}
		}

		if (ESC & 0x0001)
			break;

		if (view_update)
		{
			//erase prev view
			for (int l = 0; l<rec_per_view; l++)			
				drawXY(7, 7 + l, std::string("                                                                    ").c_str());				
			

			auto rec_it = game_records.begin() + cur_view_rec_start_idx;
			auto rec_end = game_records.end();

			if (cur_view_rec_start_idx + rec_per_view <= (int)game_records.size())
				rec_end = game_records.begin() + cur_view_rec_start_idx + rec_per_view ;

			for (int l=0; rec_it != rec_end; rec_it++,l++)
			{
				std::vector<std::string> cols = { std::to_string(cur_view_rec_start_idx +l + 1),
					rec_it->name,
				std::to_string(rec_it->play_time_milli),
				std::to_string(rec_it->final_stage),
				std::to_string(rec_it->score) };

				std::string tuple = make_tuple(cols, 20, 3);

				drawXY(7, 7 + l, tuple.c_str());
			}
		}

	}

}

clTetris::~clTetris() {

	
}
