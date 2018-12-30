
#define TETRIS_TEST

#ifdef TETROMINO_TEST

#include <iostream>
#include "clTetromino.h"

int main(){

	clTetromino &&tr = clTetromino::getRandomTetrominoFormat({0,0},{1,10});

	auto cur_shape = tr.getCurrentShape();

	int map[10][10] = { 0, };

	
	for (int i = 0; i < cur_shape->size(); i++)
	{
		
		map[1+(*cur_shape)[i].second][1+(*cur_shape)[i].first] = tr.getType();
		
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			std::cout << map[i][j] << ' ';
		}

		std::cout << '\n';
	}
    
}

#elif defined MAP_TEST

#include <iostream>
#include "clMap.h"
#include <thread>

int main() {

	clMap map({ 10,15 }, { 2,1 });

	//map.moveTetrominoHor(1);
	//map.draw();

	//std::cout << '\n';

	//clTetromino tr = clTetromino::createRandomTetromino();

	//map.replaceTetromino(tr);
	map.rotateTromino90();
	map.getTetromino().draw();

	map.moveTetrominoHor(1);
	map.getTetromino().draw();
	
	map.getTetromino().draw();

	map.moveTetrominoDown();

	map.draw();
	map.getTetromino().draw();

	map.moveTetrominoDown();
	
	map.draw();
	map.getTetromino().draw();
	
	map.moveTetrominoDown();
	
	map.draw();
	map.getTetromino().draw();
	
	map.moveTetrominoDown();
	
	map.draw();
	map.getTetromino().draw();
	
	map.moveTetrominoDown();
	map.draw();
	map.getTetromino().draw();
	//map.moveTetromino(0, 1);
	//map.moveTetromino(0, 0);
	//map.moveTetromino(0, 0);
	//map.draw();
	//map.moveTetromino(0, 1);
	//map.moveTetromino(0, 1);
	//map.draw();
	//map.moveTetromino(0, 1);
	//map.draw();
	//map.moveTetromino(0, 1);
	//map.draw();
	//map.moveTetromino(0, 1);
	//map.draw();
	//map.moveTetromino(0, 1);
	//map.draw();
	//map.moveTetromino(0, 1);
	////map.getTetromino().rotate90();
	//map.moveTetromino(0, 1);
	//map.draw();
	//map.moveTetromino(0, 1);
	//map.draw();
	//map.moveTetromino(0, 1);
	//map.draw();
	//map.moveTetromino(0, 1);
	//map.draw();
	//map.moveTetromino(0, 1);
	//map.moveTetromino(0, 1);
	//map.draw();
}

#elif defined STAGE_TEST

#include <iostream>
#include "clStage.h"
#include <ctime>

int main() {

	srand((unsigned)time(nullptr));//for random tetris generator

	clStage* stage1 = new clStage(80000,5, 500000, 1, "C:\\Users\\USER\\Desktop\\tetris\\tetris-win-console\\tetris\\Debug\\BGM_Tetris_Bradinsky.wav");

	
	clStage* stage2 = new clStage(70000, 10, 400000, 2, "C:\\Users\\USER\\Desktop\\tetris\\tetris-win-console\\tetris\\Debug\\BGM+Tetris+Kalinka.wav");

	
	clStage* stage3 = new clStage(60000, 15, 300000, 3, "C:\\Users\\USER\\Desktop\\tetris\\tetris-win-console\\tetris\\Debug\\BGM_Tetris_Bradinsky.wav");

	stage1->run();
	delete stage1 ;
	system("cls");

	stage2->run();
	delete stage2 ;
	system("cls");

	stage3->run();
	delete stage3 ;
	
}

#elif defined(TIMER_TEST)

#include "clTimer.h"
#include "tetrisUtility.h"

int main() {

	hideCursor();
	clTimer *t = clTimer::getInstance();

	t->setDpTopLeft({ 20,20 });

	t->start();

	while (1);


}

#elif defined(GAMERECORD_TEST)

#include "stGameRecord.h"
#include <vector>
#include <iostream>

int main() {

	std::vector<stGameRecord> records;

	
	stGameRecord::loadRecord(&records, "C:\\Users\\Kim\\Desktop\\tetris\\tetris-win-console\\tetris\\tetris\\player_records.dat");

	for (auto& rec : records)
	{
		std::cout << rec .to_string()<< '\n';
	}
	
	//stGameRecord::saveRecord(rec, "C:\\Users\\Kim\\Desktop\\tetris\\tetris-win-console\\tetris\\tetris\\player_records.dat");
}

#elif defined(TETRIS_TEST)

#include "clTetris.h"
#include <string>

int main(){

	clTetris tetris ;

	tetris.run() ;
}

#endif 

