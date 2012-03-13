/* 
 * File:   ChessBoard.cpp
 * Author: Matěj Šimek - www.matejsimek.cz
 * 
 * Created on 12. květen 2010, 22:47
 */

#include <stddef.h>

#include "ChessBoard.h"

#include "Pawn.h"
#include "Knight.h"
#include "Queen.h"

#include <iostream>
using namespace std;

#include <ctime>

ChessBoard::ChessBoard() {
    width = height = 8; // Standart size of Chessboard
    pawnCount = queenCount = knightCount = 0;
    freeSquares = width * height;

    allocateMemory();
}

ChessBoard::ChessBoard(int boardWidth, int boardHeight) {
    width = boardWidth;
    height = boardHeight;
    pawnCount = queenCount = knightCount = 0;
    freeSquares = width * height;

    allocateMemory();
}

bool ChessBoard::allocateMemory() {
    // TODO : Inicializace jednotlivych poli
    board = new Square * [width];
    for (int i = 0; i < width; i++) {
        board[i] = new Square[height];
    }

    if (board != NULL) return true;
    else return false;
}

ChessBoard::~ChessBoard() {
    // Memory de-allocation to prevent memoty leak
    for (int i = 0; i < width; i++) {
        delete [] board[i];
    }
    delete [] board;

    pawnPositions.~vector();
    knightPositions.~vector();
    queenPositions.~vector();
}

void ChessBoard::printToStd() {
    // Number of rows
    cout << "   ";
    for (int j = 0; j < width; j++) {
        cout << "  " << j;
        if (j < 10) cout << " ";
    }
    cout << endl;

    for (int i = 0; i < height; i++) {
        // First line
        cout << "   ";
        for (int j = 0; j < width; j++) {
            cout << "+---";
        }
        cout << "+" << endl;
        // Second line
        if (i < 10) cout << " ";
        cout << i << " ";        
        for (int j = 0; j < width; j++) {
            board[j][i].printToStd();
        }
        cout << "|" << endl;
    }

    // Last line
    cout << "   ";
    for (int i = 0; i < width; i++) {
        cout << "+---";
    }
    cout << "+" << endl;

}

bool ChessBoard::capturePiece(int x, int y) {
    if (board[x][y].isEmpty()) return false;

    if (board[x][y].isPawn()) {
        // Find and move pawn outside of board
        for (int i = 0; i < pawnCount; i++) {
            if (pawnPositions[i].x == x && pawnPositions[i].y == y) {
                pawnPositions.erase(pawnPositions.begin() + i);
                break;
            }
        }
        pawnCount--;
    } else if (board[x][y].isKnight()) {
        // Find and move knight outside of board
        for (int i = 0; i < knightCount; i++) {
            if (knightPositions[i].x == x && knightPositions[i].y == y) {
                knightPositions.erase(knightPositions.begin() + i);
                break;
            }
        }
        knightCount--;
    } else if (board[x][y].isQueen()) {
        // Find and move knight outside of board
        for (int i = 0; i < queenCount; i++) {
            if (queenPositions[i].x == x && queenPositions[i].y == y) {
                queenPositions.erase(queenPositions.begin() + i);
                break;
            }
        }
        queenCount--;
    } else {
        return false;
    }

    board[x][y].erase();
    return true;
}

bool ChessBoard::capturePiece(Position pos) {
    return capturePiece(pos.x, pos.y);
}

bool ChessBoard::isOutOfBoard(int x, int y) {
    return x < 0 || y < 0 || x >= width || y >= height;
}

bool ChessBoard::isPieceEmpty(int x, int y) {
    //if (isOutOfBoard(x, y)) return false;
    return board[x][y].isEmpty();
}

bool ChessBoard::movePiece(int fromX, int fromY, int toX, int toY) {
    if (isOutOfBoard(fromX, fromY) || isOutOfBoard(toX, toY)) return false;
    if (!board[toX][toY].isEmpty() || board[fromX][fromY].isEmpty()) return false;

    if (board[fromX][fromY].isPawn()) {
        board[toX][toY].setPawn();
        board[fromX][fromY].erase();
        return true;
    }

    if (board[fromX][fromY].isKnight()) {
        board[toX][toY].setKnight();
        board[fromX][fromY].erase();
        return true;
    }

    if (board[fromX][fromY].isQueen()) {
        board[toX][toY].setQueen();
        board[fromX][fromY].erase();
        return true;
    }

    return false;
}

bool ChessBoard::movePiece(Position from, Position to) {
    return movePiece(from.x, from.y, to.x, to.y);
}

bool ChessBoard::insertPawn(int x, int y) {
    if (isOutOfBoard(x, y)) return false;
    if (!board[x][y].isEmpty()) return false;

    board[x][y].setPawn();

    Position newPawnPosition;
    newPawnPosition.x = x;
    newPawnPosition.y = y;
    pawnPositions.push_back(newPawnPosition);

    pawnCount++;
    freeSquares--;

    return true;
}

void ChessBoard::insertRandomPawns(int pawnCountToInsert) {
    if (pawnCountToInsert > freeSquares) return;
    int pawnCountInserted = 0;

    srand(time(NULL)); // Initialize random seed

    // Place pawns on random spots
    while (pawnCountInserted < pawnCountToInsert) {

        int randomX = rand() % width;
        int randomY = rand() % height;

        if (insertPawn(randomX, randomY)) {
            pawnCountInserted++;
        } else {
            continue;
        }

    }
}

bool ChessBoard::insertKnight(int x, int y) {
    if (isOutOfBoard(x, y)) return false;
    if (!board[x][y].isEmpty()) return false;

    board[x][y].setKnight();

    Position newKnightPosition;
    newKnightPosition.x = x;
    newKnightPosition.y = y;
    knightPositions.push_back(newKnightPosition);

    knightCount++;
    freeSquares--;

    return true;
}

void ChessBoard::insertRandomKnights(int knightCountToInsert) {
    if (knightCountToInsert > freeSquares) return;
    int knightCountInserted = 0;

    srand(time(NULL)); // Initialize random seed

    // Place knights on random spots
    while (knightCountInserted < knightCountToInsert) {
        int randomX = rand() % width;
        int randomY = rand() % height;

        if (insertKnight(randomX, randomY)) {
            knightCountInserted++;
        } else {
            continue;
        }
    }
}

bool ChessBoard::insertQueen(int x, int y) {
    if (isOutOfBoard(x, y)) return false;
    if (!board[x][y].isEmpty()) return false;

    board[x][y].setQueen();

    Position newQueenPosition;
    newQueenPosition.x = x;
    newQueenPosition.y = y;
    queenPositions.push_back(newQueenPosition);

    queenCount++;
    freeSquares--;

    return true;
}

void ChessBoard::insertRandomQueens(int queenCountToInsert) {
    if (queenCountToInsert > freeSquares) return;
    int queenCountInserted = 0;

    srand(time(NULL)); // Initialize random seed

    // Place knights on random spots
    while (queenCountInserted < queenCountToInsert) {
        int randomX = rand() % width;
        int randomY = rand() % height;

        if (insertQueen(randomX, randomY)) {
            queenCountInserted++;
        } else {
            continue;
        }
    }
}

