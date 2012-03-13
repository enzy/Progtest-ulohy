/* 
 * File:   ChessBoard.h
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 12. květen 2010, 22:47
 */

#ifndef _CHESSBOARD_H
#define	_CHESSBOARD_H

#include "Square.h"
#include "Position.h"
#include <vector>

/**
 * ChessBoard Class
 */
class ChessBoard {
public:
    /**     
     * Create typical gameplay chessboard 8x8, 8 pawns, 1 queen, 1 knight
     */
    ChessBoard();

    /**
     * Create custom chessboard, allowing non square cases
     * @param boardWidth in Squares
     * @param boardHeight in Squares
     */
    ChessBoard(int boardWidth, int boardHeight);

    /**
     * Desctructor, de-allocatte memory
     */
    virtual ~ChessBoard();

    /**
     * Formated output to std::out
     */
    void printToStd();

    /**
     * Insert to chessboard random placed pawns
     * @param pawnCountToInsert Number of pawns to insert
     */
    void insertRandomPawns(int pawnCountToInsert);

    /**
     * Insert single pawn on the specific position
     * @param x from left side
     * @param y from top side
     * @return Success of operation (false when piece is full)
     */
    bool insertPawn(int x, int y);

    /**
     * Insert to chessboard random placed knights.
     * @param knightCountToInsert Number of knights to insert
     */
    void insertRandomKnights(int knightCountToInsert);

    /**
     * Insert single knight on the specific position
     * @param x from left side
     * @param y from top side
     * @return Success of operation (false when piece is full)
     */
    bool insertKnight(int x, int y);

    /**
     * Insert to chessboard random placed queens.
     * @param queenCountToInsert Number of queens to insert
     */
    void insertRandomQueens(int queenCountToInsert);

    /**
     * Insert single queen on the specific position
     * @param x from left side
     * @param y from top side
     * @return Success of operation (false when piece is full)
     */
    bool insertQueen(int x, int y);

    /**
     * Moves with piece from source to destination coordinations
     * @param fromX Source piece
     * @param fromY
     * @param toX Destination piece
     * @param toY
     * @return Success of operation (false when source piece is empty or destination piece is full)
     */
    bool movePiece(int fromX, int fromY, int toX, int toY);

    /**
     * Moves with piece from source to destination positions
     * @param from
     * @param to
     * @return Success of operation (false when source piece is empty or destination piece is full)
     */
    bool movePiece(Position from, Position to);

    /**
     *
     * @param x
     * @param y
     * @return true when field is empty
     */
    bool isPieceEmpty(int x, int y);

    /**
     * Check position if its in gameplay board
     * @param x from left corner
     * @param y from top corner
     * @return true if position is outside, otherwise false
     */
    bool isOutOfBoard(int x, int y);

    /**
     * Capture piece on position if its possible
     * @param x
     * @param y
     * @return true when capture, false when impossible
     */
    bool capturePiece(int x, int y);

    /**
     * Capture piece on position if its possible
     * @param pos position
     * @return true when capture, false when impossible
     */
    bool capturePiece(Position pos);

    /**
     * Array of pawn positions on chessboard
     */
    //Position * pawnPositions;
    vector<Position> pawnPositions;

    /**
     * Array of knight positions on chessboard
     */
    vector<Position> knightPositions;

    /**
     * Array of queen positions on chessboard
     */
    vector<Position> queenPositions;

    int getPawnCount() {
        return pawnCount;
    };

    int getKnightCount() {
        return knightCount;
    };

    int getQueenCount() {
        return queenCount;
    };

    int getWidth() {
        return width;
    };

    int getHeight() {
        return height;
    };

private:
    /**
     * Allocates (2D array) memory using widht and height class variables
     * @return Success of memory alocation
     */
    bool allocateMemory();

    /**
     * Chessboard (from squares). Creates and indexing from top left corner - board[x][y]
     */
    Square ** board;

    /**
     * Chessboard width (in squares)
     */
    int width;
    /**
     * Chessboard height (in squares)
     */
    int height;

    /**
     * How many free squares remaining on chessboard
     */
    int freeSquares;

    /**
     * Number of pawns on chessboard
     */
    int pawnCount;

    /**
     * Number of queens on chessboard
     */
    int queenCount;

    /**
     * Nubmer of knights on chessboard
     */
    int knightCount;

};

#endif	/* _CHESSBOARD_H */

