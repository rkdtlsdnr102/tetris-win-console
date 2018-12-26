#pragma once
#include <utility>
#include <vector>
#include "clTetromino.h"

#define MAP_PATTERN "."
#define MAP_FRAME_PATTERN "□"
#define PLAY_FIELD_START_PATTERN "-"
#define REMOVING_CELL_PATTERN "▦"
#define SHADOW_TETROMINO_PATTERN "□"

class clMap{

private:

    std::pair<int,int> _map_size ;
    clTetromino *_cur_tetromino ;
	int _play_field_start_row;
    int **_map ;
    COORD _map_dp_abs_tl ;


    void _reorganizeFrom(int idx) ;
    std::pair<int, int> _getShadowTopLeft();


public:
	clMap(std::pair<int, int> map_size = { 10,15 }, COORD map_abs_dp_tl = {2,1});
    void addBottomLine() ;
    bool isLineConnected(int idx) ;
	//draw tetromino on the map information array
	void projectTetromino();
    void removeLine(int idx) ;
    void removeLines(std::vector<int> &idx) ;
    clTetromino &getTetromino() ;
    void setTetromino(clTetromino &tr) ;
    void moveTetrominoHor(int dirX) ;
    bool moveTetrominoDown() ;
    void rotateTromino90() ;
	void hardDrop();
	bool abovePlayfieldStart();
	//calculate and get top left coordinate of shadow tetromino on the map
	void draw();
	void drawStartLine();
	void drawRemovingRow(std::vector<int> &rm_rows);
    void drawShadowTetromino() ;
    void eraseShadowTetromino() ;
    ~clMap() ;
};