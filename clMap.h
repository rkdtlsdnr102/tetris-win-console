#pragma once
#include <utility>
#include <vector>
#include "clTetromino.h"

#define MAP_PATTERN "."
#define MAP_FRAME_PATTERN "��"

class clMap{

private:

    //std::pair<int,int> _tl ;
    std::pair<int,int> _map_size ;
    clTetromino _cur_tetromino ;
    std::pair<short,short> _cur_tetromino_tl ;
    int **_map ;

    void _reorganizeFrom(int idx, int rm_cnt) ;


public:
    clMap(std::pair<int,int> map_size={10,15}) ;
    bool isLineConnected(int idx) ;
	//map ���� tetromino�� �׷��ִ´�.
	void projectTetromino();
    void removeLine(int idx) ;
    void removeLines(std::vector<int> &idx) ;
    clTetromino &getTetromino() ;
    void setTetromino(clTetromino &tr) ;
    void moveTetrominoHor(int offX) ;
    bool moveTetrominoDown() ;
    void rotateTromino90() ;
	void hardDrop();
	//�浹�� �߻��ϴ� ���������� tetromino�� �ʻ��� topleft ��ġ
	std::pair<short, short> getShadowTopLeft();
    std::pair<short,short> getTetrominoTopLeft() ;
	void draw(COORD cursor_pos);
    ~clMap() ;
};