/* 
 * File:   Square.cpp
 * Author: Matěj Šimek - www.matejsimek.cz
 * 
 * Created on 12. květen 2010, 22:53
 */

#include "Square.h"
#include "Piece.h"

#include <iostream>
using namespace std;

Square::Square() {
    hasPiece = hasKnight = hasPawn = hasQueen = false;
}

Square::Square(int pieceType) {
    switch (pieceType) {
        case 0:
            erase();
            break;
        case 1:
            setPawn();
            break;
        case 2:
            setKnight();
            break;
        case 3:
            setQueen();
            break;
        default:
            erase();
    }
}

void Square::erase(){
    hasPiece = hasKnight = hasPawn = hasQueen = false;
}

void Square::setKnight() {
    hasPiece = hasKnight = true;
    hasPawn = hasQueen = false;    
}

void Square::setPawn() {
    hasPiece = hasPawn = true;
    hasKnight = hasQueen = false;
}

void Square::setQueen() {
    hasPiece = hasQueen = true;
    hasPawn = hasKnight = false;
}

bool Square::isKnight() {
    return hasPiece && hasKnight;
}

bool Square::isPawn() {
    return hasPiece && hasPawn;
}

bool Square::isQueen() {
    return hasPiece && hasQueen;
}

void Square::printToStd() {   
    cout << "| ";
    if (isEmpty()) cout << " ";
    else if (isKnight()) cout << "N";
    else if (isPawn()) cout << "P";
    else if (isQueen()) cout << "Q";
    cout << " ";
}

bool Square::isEmpty() {
    return !hasPiece;
}
