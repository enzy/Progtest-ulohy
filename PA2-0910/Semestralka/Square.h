/* 
 * File:   Square.h
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 12. květen 2010, 22:53
 */

#ifndef _SQUARE_H
#define	_SQUARE_H

class Square {
public:
    /**
     * Default, empty square
     */
    Square();
    /**
     * Custom square type
     * @param pieceType [0 - empty, 1 - pawn, 2 - knight, 3 - queen]
     */
    Square(int pieceType);    

    bool isPawn();
    bool isKnight();
    bool isQueen();
    bool isEmpty();

    void erase();

    void setPawn();
    void setKnight();
    void setQueen();

    /**
     * Prints square content to std::out
     */
    void printToStd();
private:
    bool hasPiece;
    bool hasPawn;
    bool hasQueen;
    bool hasKnight;

};

#endif	/* _SQUARE_H */

