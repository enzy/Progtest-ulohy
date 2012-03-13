/* 
 * File:   Piece.h
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 12. červen 2010, 13:13
 */

#ifndef PIECE_H
#define	PIECE_H

#include "Position.h"
#include "ChessBoard.h"

class Piece {
public:
    /**
     * Create empty piece with no position (-1,-1)
     */
    Piece();
    /**
     * Create empty piece on entered position [x,y]
     * @param x
     * @param y
     */
    Piece(int x, int y);

    /**
     * Create empty piece on entered position [x, y] on entered chessboard
     * @param x
     * @param y
     * @param parentBoard chessboard where to place piece
     */
    Piece(int x, int y, ChessBoard * _parentBoard);

    /**
     * Current X position
     */
    int posX;

    /**
     * Current Y position
     */
    int posY;

    /**
     * Pointer to chessboard where piece is placed
     */
    ChessBoard * parentBoard;

    /**
     * Finds next move to the closest pawn
     * @return Position to move
     */
    Position findNextMove();

    /**
     * Get current position of piece on chessboard, counted from posX, posY variables
     * @return actual position
     */
    Position getCurrentPosition();

    /**
     * Position in last turn
     */
    Position lastPosition;

    /**
     * Set new position of piece on chessboard
     * @param pos
     */
    void setPosition(Position newPos);

    /**
     * Get distance from-to positions
     * @param fromX
     * @param fromY
     * @param toX
     * @param toY
     * @return distance
     */
    double getDistance(int fromX, int fromY, int toX, int toY);

    /**
     * Get distance from-to positions
     * @param fromPosition
     * @param toPosition
     * @return distance
     */
    double getDistance(Position fromPosition, Position toPosition);
    /**
     * Get distance from actual position to destination
     * @param toX
     * @param toY
     * @return distance
     */
    double getDistanceTo(int toX, int toY);

    /**
     * Get distance from actual position to destination
     * @param toPosition
     * @return distance
     */
    double getDistanceTo(Position toPosition);

private:

};

#endif	/* PIECE_H */

