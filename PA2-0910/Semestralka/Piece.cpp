/* 
 * File:   Piece.cpp
 * Author: Matěj Šimek - www.matejsimek.cz
 * 
 * Created on 12. červen 2010, 13:13
 */

#include "Piece.h"
#include <math.h>

Piece::Piece() {
    posX = posY = -1;
}

Piece::Piece(int x, int y) {
    posX = x;
    posY = y;
}

Piece::Piece(int x, int y, ChessBoard * _parentBoard) {
    posX = x;
    posY = y;
    parentBoard = _parentBoard;
}

Position Piece::getCurrentPosition() {
    Position current;
    current.x = posX;
    current.y = posY;
    return current;
}

void Piece::setPosition(Position newPos){
    posX = newPos.x;
    posY = newPos.y;
}

double Piece::getDistance(int fromX, int fromY, int toX, int toY) {
    int diffX = toX - fromX;
    int diffY = toY - fromY;
    return sqrtf(diffX * diffX + diffY * diffY);
}

double Piece::getDistance(Position fromPosition, Position toPosition) {
    int diffX = toPosition.x - fromPosition.x;
    int diffY = toPosition.y - fromPosition.y;
    return sqrtf(diffX * diffX + diffY * diffY);
}

double Piece::getDistanceTo(int toX, int toY) {
    int diffX = toX - posX;
    int diffY = toY - posY;
    return sqrtf(diffX * diffX + diffY * diffY);
}

double Piece::getDistanceTo(Position toPosition) {
    int diffX = toPosition.x - posX;
    int diffY = toPosition.y - posY;
    return sqrtf(diffX * diffX + diffY * diffY);
}
