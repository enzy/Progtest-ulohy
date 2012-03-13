/* 
 * File:   Pawn.cpp
 * Author: Matěj Šimek - www.matejsimek.cz
 * 
 * Created on 9. červen 2010, 18:23
 */

#include "Pawn.h"

Pawn::Pawn() {
    posX = posY = -1;
}

Pawn::Pawn(int x, int y) {
    posX = x;
    posY = y;
}

Pawn::Pawn(int x, int y, ChessBoard* _parentBoard) {
    posX = x;
    posY = y;
    parentBoard = _parentBoard;
}
