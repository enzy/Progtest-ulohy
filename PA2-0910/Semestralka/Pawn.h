/* 
 * File:   Pawn.h
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 9. červen 2010, 18:23
 */

#ifndef PAWN_H
#define	PAWN_H

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn();
    Pawn(int x, int y);
    Pawn(int x, int y, ChessBoard * _parentBoard);

private:

};

#endif	/* PAWN_H */

