
#define TETRIS_TEST
#ifdef TETROMINO_TEST

#include <iostream>
#include "clTetromino.h"

int main(){

	clTetromino &&tr = clTetromino::createRandomTetromino();

	auto cur_shape = tr.getCurrentShape();

	int map[10][10] = { 0, };

	
	for (int i = 0; i < cur_shape->size(); i++)
	{
		
		map[3+(*cur_shape)[i].second][5+(*cur_shape)[i].first] = tr.getVal();
		
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

	clMap map({ 10,15 });

	//map.moveTetrominoHor(1);
	//map.draw();

	//std::cout << '\n';

	//clTetromino tr = clTetromino::createRandomTetromino();

	//map.replaceTetromino(tr);
	map.rotateTromino90();
	map.rotateTromino90();
	map.rotateTromino90();

	map.moveTetrominoHor(1);
	map.moveTetrominoHor(1);
	map.moveTetrominoHor(1);
	map.moveTetrominoHor(1);
	map.moveTetrominoHor(1);
	map.moveTetrominoHor(1);
	map.rotateTromino90();
	map.rotateTromino90();
	map.rotateTromino90();
	map.moveTetrominoHor(1);
	map.moveTetrominoHor(1);
	
	map.draw({1,1});
	map.getTetromino().draw({ 1,1 });

	map.moveTetrominoDown();

	map.draw({ 1,1 });
	map.getTetromino().draw({ 1,1 });

	map.moveTetrominoDown();
	
	map.draw({ 1,1 });
	map.getTetromino().draw({ 1,1 });
	
	map.moveTetrominoDown();
	
	map.draw({ 1,1 });
	map.getTetromino().draw({ 1,1 });
	
	map.moveTetrominoDown();
	
	map.draw({ 1,1 });
	map.getTetromino().draw({ 1,1 });
	
	map.moveTetrominoDown();
	map.draw({ 1,1 });
	map.getTetromino().draw({ 1,1 });
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

#elif defined TETRIS_TEST

#include <iostream>
#include "clTetris.h"

int main() {


	clTetris tetris(10);

	tetris.run();
	
}


#endif 

