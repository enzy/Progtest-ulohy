/* 
 * File:   main.cpp - Královna a jezdec
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 12. květen 2010, 22:31
 *
 * Zadání:
 * 19. Královna a jezdec (ML)
 * V jazyce C++ implementujte problém „Královna a jezdec“. Je zadána šachovnice
 * (může být různě veliká), na které je nějaké množství pěšců, královna a jezdec.
 * Královna a jezdec se střídají v tazích a snaží se v co nejmenším počtu tahů
 * sebrat všechny pěšce.
 *
 * Pokyny pro odevzdání:
 * https://edux.fit.cvut.cz/courses/BI-PA2/teacher/vranyj1/sp_pokyny
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>

#include "ChessBoard.h"
#include "Queen.h"
#include "Knight.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    // inicialization

    // automaticky nebo rucni mod?

    // rucni mod - sirka, vyska; pocet pescu; umisteni;

    cout << "Welcome to the program called:\n\n"
            "+----------------------------------------------------+\n"
            "|  The Queen and Knight in the battle against pawns  |\n"
            "+----------------------------------------------------+\n"
            "| Created by: Matej Simek - www.matejsimek.cz | 2010 |\n"
            "+----------------------------------------------------+\n\n";

    string input = "";
    int choose = 0;

    while (true) {
        cout << "Please choose game mode:\n"
                "1 - fully-automatic mode (board 5x5, 5 pawns)\n"
                "2 - semi-automatic mode (custom board size, custom pawn count)\n" << endl;
        //"3 - manual mode" << endl;

        getline(cin, input);
        stringstream myStream(input);
        if (myStream >> choose) {
            if (choose > 0 && choose < 3) break;
        }
        cout << "Invalid number, please try again.\n" << endl;
    }

    // Where to store pieces
    ChessBoard * hra1;

    switch (choose) {
        case 1:
            hra1 = new ChessBoard(5, 5);
            hra1->insertRandomPawns(5);
            break;
        case 2:
            int width, height, pawnCount;
            while (true) {
                cout << "Type width of chessboard to create:" << endl;
                getline(cin, input);
                stringstream myStream(input);
                if (myStream >> width) {
                    if (width > 99) cout << "Its to many, 99 is max." << endl;
                    if (width < 3) cout << "Minimum width is 3." << endl;
                    else break;
                }
                cout << "Invalid number, please try again.\n" << endl;
            }
            while (true) {
                cout << "Type height of chessboard to create:" << endl;
                getline(cin, input);
                stringstream myStream(input);
                if (myStream >> height) {
                    if (height > 99) cout << "Its to many, 99 is max." << endl;
                    if (height < 3) cout << "Minimum height is 3." << endl;
                    else break;
                }
                cout << "Invalid number, please try again.\n" << endl;
            }
            hra1 = new ChessBoard(width, height);
            while (true) {
                cout << "Type pawn count to place on chessboard:" << endl;
                getline(cin, input);
                stringstream myStream(input);
                if (myStream >> pawnCount) {
                    if (pawnCount > width * height - 2) {
                        cout << "Its to many." << endl;
                    } else {
                        break;
                    }
                }
                cout << "Invalid number, please try again.\n" << endl;
            }
            hra1->insertRandomPawns(pawnCount);
            break;
    }

    hra1->insertRandomKnights(1);
    hra1->insertRandomQueens(1);

    cout << "Initial setup" << endl;
    hra1->printToStd();
    cout << endl;


    Queen q1(hra1->queenPositions[0].x, hra1->queenPositions[0].y, hra1);
    Knight n1(hra1->knightPositions[0].x, hra1->knightPositions[0].y, hra1);

    for (int i = 0; i < hra1->getPawnCount(); i++) {
        //cout << "P" << i << ": " << hra1->pawnPositions[i].x << ", " << hra1->pawnPositions[i].y << endl;
    }

    int moveCount = 1;

    while (hra1->getPawnCount() > 0) {
        // Switch Queen and Knight in play, Queen first.
        if (moveCount % 2 != 0) {
            // Compute Queen next move
            Position queenNextMove = q1.findNextMove();
            // Make Queen move
            if (hra1->isOutOfBoard(queenNextMove.x, queenNextMove.y)) continue;
            hra1->capturePiece(queenNextMove);
            hra1->movePiece(q1.getCurrentPosition(), queenNextMove);
            q1.setPosition(queenNextMove);
        } else {
            // Compute Knight next move
            Position knightNextMove = n1.findNextMove();
            // Make Knight move
            if (hra1->isOutOfBoard(knightNextMove.x, knightNextMove.y)) continue;
            hra1->capturePiece(knightNextMove);
            hra1->movePiece(n1.getCurrentPosition(), knightNextMove);
            n1.setPosition(knightNextMove);
        }
        cout << "\nMove #" << moveCount++ << endl;

        // Print movement
        hra1->printToStd();
        cout << endl;
    }

    cout << "\nThis is the end. Thank you for watching." << endl;



    return 0;
}

