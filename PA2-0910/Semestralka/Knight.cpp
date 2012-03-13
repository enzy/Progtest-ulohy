/* 
 * File:   Knight.cpp
 * Author: Matěj Šimek - www.matejsimek.cz
 * 
 * Created on 9. červen 2010, 18:19
 */

#include "Knight.h"

Knight::Knight() {
    posX = posY = -1;
    lastPosition = getCurrentPosition();
}

Knight::Knight(int x, int y) {
    posX = x;
    posY = y;
    lastPosition = getCurrentPosition();
}

Knight::Knight(int x, int y, ChessBoard * _parentBoard) {
    posX = x;
    posY = y;
    parentBoard = _parentBoard;
    lastPosition = getCurrentPosition();
}

Position Knight::findNextMove() {

    int boardHeight = parentBoard->getHeight();
    int boardWidth = parentBoard->getWidth();

    int max = boardWidth * boardHeight;

    Position * moves = new Position[max];
    double * distances = new double[max];
    int nubmerOfMoves = 0;

    // TODO : check the pawns on the road

    // Find the closest pawn
    double min = max;
    int minPosition = 0;
    Position closestPawnPos;

    bool hasPawnInRange = false;

    // Range of Knight moves
    Position * knightRange = new Position[8];
    knightRange[0] = Position(posX + 1, posY + 2);
    knightRange[1] = Position(posX - 1, posY + 2);
    knightRange[2] = Position(posX + 2, posY + 1);
    knightRange[3] = Position(posX + 2, posY - 1);
    knightRange[4] = Position(posX - 2, posY + 1);
    knightRange[5] = Position(posX - 2, posY - 1);
    knightRange[6] = Position(posX + 1, posY - 2);
    knightRange[7] = Position(posX - 1, posY - 2);

    // Check, if somy Pawn is in range
    for (int i = 0; i < parentBoard->getPawnCount(); i++) {
        for (int j = 0; j < 8; j++) {
            if (parentBoard->pawnPositions[i] == knightRange[j]) {
                hasPawnInRange = true;
                closestPawnPos = knightRange[j];
                minPosition = i;
                break;
            }
        }
        if (hasPawnInRange) break;
    }
    if (hasPawnInRange) cout << "Closest Pawn to the Knight: P" << minPosition << " (" << closestPawnPos << ")" << endl;

    // Find closest pawn and jump on
    if (!hasPawnInRange) {
        for (int i = 0; i < parentBoard->getPawnCount(); i++) {
            // Skip pieces outside board
            if (parentBoard->isOutOfBoard(parentBoard->pawnPositions[i].x, parentBoard->pawnPositions[i].y)) continue;
            double dist = getDistanceTo(parentBoard->pawnPositions[i]);
            if (dist == 0) continue;
            if (dist <= min) {
                min = dist;
                minPosition = i;
            }
        }
        closestPawnPos = parentBoard->pawnPositions[minPosition];
        cout << "Closest Pawn to the Knight: P" << minPosition << " (" << closestPawnPos << "), Distance: " << min << endl;
    }

    // Find the best move to the closest Pawn position
    int newX, newY;

    // L - long down, short right
    newX = posX + 1;
    newY = posY + 2;
    if (!parentBoard->isOutOfBoard(newX, newY)) {
        Position newPosition(newX, newY);
        if (closestPawnPos == newPosition || parentBoard->isPieceEmpty(newX, newY)) {
            moves[nubmerOfMoves] = newPosition;
            distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        }
    }
    // long down, short left
    newX = posX - 1;
    newY = posY + 2;
    if (!parentBoard->isOutOfBoard(newX, newY)) {
        Position newPosition(newX, newY);
        if (closestPawnPos == newPosition || parentBoard->isPieceEmpty(newX, newY)) {
            moves[nubmerOfMoves] = newPosition;
            distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        }
    }
    // short down, long right
    newX = posX + 2;
    newY = posY + 1;
    if (!parentBoard->isOutOfBoard(newX, newY)) {
        Position newPosition(newX, newY);
        if (closestPawnPos == newPosition || parentBoard->isPieceEmpty(newX, newY)) {
            moves[nubmerOfMoves] = newPosition;
            distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        }
    }
    // short up, long right
    newX = posX + 2;
    newY = posY - 1;
    if (!parentBoard->isOutOfBoard(newX, newY)) {
        Position newPosition(newX, newY);
        if (closestPawnPos == newPosition || parentBoard->isPieceEmpty(newX, newY)) {
            moves[nubmerOfMoves] = newPosition;
            distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        }
    }
    // short down, long left
    newX = posX - 2;
    newY = posY + 1;
    if (!parentBoard->isOutOfBoard(newX, newY)) {
        Position newPosition(newX, newY);
        if (closestPawnPos == newPosition || parentBoard->isPieceEmpty(newX, newY)) {
            moves[nubmerOfMoves] = newPosition;
            distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        }
    }
    // short up, long left
    newX = posX - 2;
    newY = posY - 1;
    if (!parentBoard->isOutOfBoard(newX, newY)) {
        Position newPosition(newX, newY);
        if (closestPawnPos == newPosition || parentBoard->isPieceEmpty(newX, newY)) {
            moves[nubmerOfMoves] = newPosition;
            distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        }
    }
    // long up, short right
    newX = posX + 1;
    newY = posY - 2;
    if (!parentBoard->isOutOfBoard(newX, newY)) {
        Position newPosition(newX, newY);
        if (closestPawnPos == newPosition || parentBoard->isPieceEmpty(newX, newY)) {
            moves[nubmerOfMoves] = newPosition;
            distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        }
    }
    // long up, short left
    newX = posX - 1;
    newY = posY - 2;
    if (!parentBoard->isOutOfBoard(newX, newY)) {
        Position newPosition(newX, newY);
        if (closestPawnPos == newPosition || parentBoard->isPieceEmpty(newX, newY)) {
            moves[nubmerOfMoves] = newPosition;
            distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        }
    }

    // Filter closest move
    min = max;
    minPosition = 0;
    for (int i = 0; i < nubmerOfMoves; i++) {
        double dist = distances[i];
        if (dist <= min && lastPosition != moves[i]) {
            min = dist;
            minPosition = i;
        }
    }
    Position bestMove = moves[minPosition];
    if (parentBoard->isOutOfBoard(bestMove.x, bestMove.y)) {
        cout << "No move for knight in this turn." << endl;
        bestMove = Position(-1, -1);
    } else {
        cout << "Best Knight move to the Pawn: (" << bestMove << "), Distance: " << min << endl;
    }

    delete [] moves;
    delete [] distances;

    return bestMove;

}


