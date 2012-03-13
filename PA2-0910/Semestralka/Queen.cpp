/* 
 * File:   Queen.cpp
 * Author: Matěj Šimek - www.matejsimek.cz
 * 
 * Created on 9. červen 2010, 18:18
 */

#include <stdio.h>

#include "Queen.h"
#include "Pawn.h"
#include <iostream>
using namespace std;

Queen::Queen() {
    posX = posY = -1;
    lastPosition = getCurrentPosition();
}

Queen::Queen(int x, int y) {
    posX = x;
    posY = y;
    lastPosition = getCurrentPosition();
}

Queen::Queen(int x, int y, ChessBoard * _parentBoard) {
    posX = x;
    posY = y;
    parentBoard = _parentBoard;
    lastPosition = getCurrentPosition();
}

Position Queen::findNextMove() {

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
    vector<Position> pawnsInRange;

    for (int i = 0; i < parentBoard->getPawnCount(); i++) {
        hasPawnInRange = false;
        // Move 1
        for (int n = 1; posX + n < boardWidth; n++) {
            Position newPosition(posX + n, posY);
            if (parentBoard->pawnPositions[i] == newPosition) {
                pawnsInRange.push_back(newPosition);
                hasPawnInRange = true;
                break;
            }
        }
        if (hasPawnInRange) continue;
        // Move 2
        for (int n = 1; posX - n >= 0; n++) {
            Position newPosition(posX - n, posY);
            if (parentBoard->pawnPositions[i] == newPosition) {
                pawnsInRange.push_back(newPosition);
                hasPawnInRange = true;
                break;
            }
        }
        if (hasPawnInRange) continue;
        // Move 3
        for (int n = 1; posY + n < boardHeight; n++) {
            Position newPosition(posX, posY + n);
            if (parentBoard->pawnPositions[i] == newPosition) {
                pawnsInRange.push_back(newPosition);
                hasPawnInRange = true;
                break;
            }
        }
        if (hasPawnInRange) continue;
        // Move 4
        for (int n = 1; posY - n >= 0; n++) {
            Position newPosition(posX, posY - n);
            if (parentBoard->pawnPositions[i] == newPosition) {
                pawnsInRange.push_back(newPosition);
                hasPawnInRange = true;
                break;
            }
        }
        if (hasPawnInRange) continue;
        // Move 5
        for (int n = 1; n < max; n++) {
            if (parentBoard->isOutOfBoard(posX + n, posY - n)) break;
            Position newPosition(posX + n, posY - n);
            if (parentBoard->pawnPositions[i] == newPosition) {
                pawnsInRange.push_back(newPosition);
                hasPawnInRange = true;
                break;
            }
        }
        if (hasPawnInRange) continue;
        // Move 6
        for (int n = 1; n < max; n++) {
            if (parentBoard->isOutOfBoard(posX + n, posY + n)) break;
            Position newPosition(posX + n, posY + n);
            if (parentBoard->pawnPositions[i] == newPosition) {
                pawnsInRange.push_back(newPosition);
                hasPawnInRange = true;
                break;
            }
        }
        if (hasPawnInRange) continue;
        // Move 7
        for (int n = 1; n < max; n++) {
            if (parentBoard->isOutOfBoard(posX - n, posY - n)) break;
            Position newPosition(posX - n, posY - n);
            if (parentBoard->pawnPositions[i] == newPosition) {
                pawnsInRange.push_back(newPosition);
                hasPawnInRange = true;
                break;
            }
        }
        if (hasPawnInRange) continue;
        // Move 8
        for (int n = 1; n < max; n++) {
            if (parentBoard->isOutOfBoard(posX - n, posY + n)) break;
            Position newPosition(posX - n, posY + n);
            if (parentBoard->pawnPositions[i] == newPosition) {
                pawnsInRange.push_back(newPosition);
                hasPawnInRange = true;
                break;
            }
        }
        if (hasPawnInRange) continue;
    }
    if (pawnsInRange.size() > 0) {
        hasPawnInRange = true;
    } else {
        hasPawnInRange = false;
    }

    // TODO filter closest pawn in range
    // Find closest Pawn in Range
    if (hasPawnInRange) {
        for (int i = 0; i < (int)pawnsInRange.size(); i++) {
            // Skip pieces outside board
            if (parentBoard->isOutOfBoard(pawnsInRange[i].x, pawnsInRange[i].y)) continue;
            double dist = getDistanceTo(pawnsInRange[i]);
            if (dist == 0) continue;
            if (dist <= min) {
                min = dist;
                minPosition = i;
            }
        }
        closestPawnPos = pawnsInRange[minPosition];
        cout << "Closest Pawn to the Queen: P" << minPosition << " (" << closestPawnPos << "), Distance: " << min << endl;
    }        // Or find closest pawn outside range
    else {
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
        cout << "Closest Pawn to the Queen: P" << minPosition << " (" << closestPawnPos << "), Distance: " << min << endl;
    }



    // Calculate moves to right (x+n,y)
    for (int n = 1; posX + n < boardWidth; n++) {
        Position newPosition;
        newPosition.x = posX + n;
        newPosition.y = posY;
        if (closestPawnPos != newPosition && !parentBoard->isPieceEmpty(posX + n, posY)) break;
        moves[nubmerOfMoves] = newPosition;
        distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        if (closestPawnPos == newPosition) break;
    }
    // Calculate moves to left (x-n, y)
    for (int n = 1; posX - n >= 0; n++) {
        Position newPosition;
        newPosition.x = posX - n;
        newPosition.y = posY;
        if (closestPawnPos != newPosition && !parentBoard->isPieceEmpty(posX - n, posY)) break;
        moves[nubmerOfMoves] = newPosition;
        distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        if (closestPawnPos == newPosition) break;
    }
    // Calculate moves down (x, y+n)
    for (int n = 1; posY + n < boardHeight; n++) {
        Position newPosition;
        newPosition.x = posX;
        newPosition.y = posY + n;
        if (closestPawnPos != newPosition && !parentBoard->isPieceEmpty(posX, posY + n)) break;
        moves[nubmerOfMoves] = newPosition;
        distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        if (closestPawnPos == newPosition) break;
    }
    // Calculate moves up (x, y-n)
    for (int n = 1; posY - n >= 0; n++) {
        Position newPosition;
        newPosition.x = posX;
        newPosition.y = posY - n;
        if (closestPawnPos != newPosition && !parentBoard->isPieceEmpty(posX, posY - n)) break;
        moves[nubmerOfMoves] = newPosition;
        distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        if (closestPawnPos == newPosition) break;
    }
    // Calculate moves right up (x+n, y-n)
    for (int n = 1; n < max; n++) {
        if (parentBoard->isOutOfBoard(posX + n, posY - n)) break;
        Position newPosition;
        newPosition.x = posX + n;
        newPosition.y = posY - n;
        if (closestPawnPos != newPosition && !parentBoard->isPieceEmpty(posX + n, posY - n)) break;
        moves[nubmerOfMoves] = newPosition;
        distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        if (closestPawnPos == newPosition) break;
    }
    // Calculate moves right down (x+n, y+n)
    for (int n = 1; n < max; n++) {
        if (parentBoard->isOutOfBoard(posX + n, posY + n)) break;
        Position newPosition;
        newPosition.x = posX + n;
        newPosition.y = posY + n;
        if (closestPawnPos != newPosition && !parentBoard->isPieceEmpty(posX + n, posY + n)) break;
        moves[nubmerOfMoves] = newPosition;
        distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        if (closestPawnPos == newPosition) break;
    }
    // Calculate moves left up (x-n, y-n)
    for (int n = 1; n < max; n++) {
        if (parentBoard->isOutOfBoard(posX - n, posY - n)) break;
        Position newPosition;
        newPosition.x = posX - n;
        newPosition.y = posY - n;
        if (closestPawnPos != newPosition && !parentBoard->isPieceEmpty(posX - n, posY - n)) break;
        moves[nubmerOfMoves] = newPosition;
        distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        if (closestPawnPos == newPosition) break;
    }
    // Calculate moves left down (x-n, y+n)
    for (int n = 1; n < max; n++) {
        if (parentBoard->isOutOfBoard(posX - n, posY + n)) break;
        Position newPosition;
        newPosition.x = posX - n;
        newPosition.y = posY + n;
        if (closestPawnPos != newPosition && !parentBoard->isPieceEmpty(posX - n, posY + n)) break;
        moves[nubmerOfMoves] = newPosition;
        distances[nubmerOfMoves++] = getDistance(newPosition, closestPawnPos);
        if (closestPawnPos == newPosition) break;
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
    cout << "Best Queen move to the Pawn: (" << bestMove << "), Distance: " << min << endl;

    delete [] moves;
    delete [] distances;

    return bestMove;
}
