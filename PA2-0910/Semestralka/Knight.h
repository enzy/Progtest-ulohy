/* 
 * File:   Knight.h
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 9. červen 2010, 18:19
 */

#ifndef KNIGHT_H
#define	KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    /**
     * Create free knight no position (-1,-1)
     */
    Knight();
    Knight(int x, int y);
    Knight(int x, int y, ChessBoard * _parentBoard);

    Position findNextMove();

private:

};

#endif	/* KNIGHT_H */

