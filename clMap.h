#pragma once
#include <utility>
#include <vector>
#include "clTetromino.h"

#define MAP_PATTERN "."
#define MAP_FRAME_PATTERN "бр"
#define PLAY_FIELD_START_PATTERN "-"
#define REMOVING_CELL_PATTERN "в╠"

class clMap{

private:

    std::pair<int,int> _map_size ;
    clTetromino _cur_tetromino ;
    std::pair<short,short> _cur_tetromino_tl ;
	int _play_field_start_row;
    int **_map ;


    void _reorganizeFrom(int idx) ;


public:
    clMap(std::pair<int,int> map_size={10,15}) ;
    bool isLineConnected(int idx) ;
	//draw tetromino on the map information array
	void projectTetromino();
    void removeLine(int idx) ;
    void removeLines(std::vector<int> &idx) ;
    clTetromino &getTetromino() ;
    void setTetromino(clTetromino &tr) ;
    void moveTetrominoHor(int offX) ;
    bool moveTetrominoDown() ;
    void rotateTromino90() ;
	void hardDrop();
	bool abovePlayfieldStart();
	//calculate and get top left coordinate of shadow tetromino on the map
	std::pair<short, short> getShadowTopLeft();
    std::pair<short,short> getTetrominoTopLeft() ;
	void draw(COORD cursor_pos);
	void drawStartLine(COORD cursor_pos);
	void drawRemovingRow(std::vector<int> &rm_rows,COORD cursor_pos);
    ~clMap() ;
};