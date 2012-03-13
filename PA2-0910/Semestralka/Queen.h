/* 
 * File:   Queen.h
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 9. červen 2010, 18:18
 */

#ifndef QUEEN_H
#define	QUEEN_H

#include "Piece.h"
#include "ChessBoard.h"

class Queen : public Piece {
public:
    Queen();
    Queen(int x, int y);
    Queen(int x, int y, ChessBoard * _parentBoard);

    Position findNextMove();
private:

};

#endif	/* QUEEN_H */

