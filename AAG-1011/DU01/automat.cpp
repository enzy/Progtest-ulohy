/* 
 * File:   DFA automat
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 7. listopad 2010, 14:38
 */

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int stateStep(int state, char inputChar);

void unknowInputChar() {
    cout << "Nespravny vstup." << endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    /*  1 - 16
        input state = 1
        output state = 12,13,14,15,16 
     */
    int state = 1;

    string inputLine;
    /* a, b, c, d */
    char inputChar = ' ';

    while (true) {

        getline(cin, inputLine);

        if (inputLine.empty()) exit(EXIT_SUCCESS);        

        for (unsigned int i = 0; i < inputLine.length(); i++) {
            inputChar = inputLine[i];
            state = stateStep(state, inputChar);
        }

        if (state >= 12 && state <= 16) {
            cout << "1" << endl;
        } else {
            cout << "0" << endl;
        }

        state = 1;
        inputLine.clear();
    }

    return 0;
}

/*
 * Mohlo by to byti realizovano pomoci poli, ale tato varianta je mene pracnejsi
 */
int stateStep(int state, char inputChar) {
    switch (state) {
        case 1:
            if (inputChar == 'a') {
                state = 11;
            } else if (inputChar == 'b') {
                state = 5;
            } else if (inputChar == 'c') {
                state = 14;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 2:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                state = 9;
            } else if (inputChar == 'c') {
                state = 12;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 3:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                ;
            } else if (inputChar == 'c') {
                state = 13;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 4:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                ;
            } else if (inputChar == 'c') {
                state = 12;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 5:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                ;
            } else if (inputChar == 'c') {
                ;
            } else if (inputChar == 'd') {
                state = 6;
            } else {
                unknowInputChar();
            }
            break;
        case 6:
            if (inputChar == 'a') {
                state = 7;
            } else if (inputChar == 'b') {
                state = 8;
            } else if (inputChar == 'c') {
                ;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 7:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                state = 10;
            } else if (inputChar == 'c') {
                ;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 8:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                ;
            } else if (inputChar == 'c') {
                state = 3;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 9:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                ;
            } else if (inputChar == 'c') {
                state = 4;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 10:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                state = 13;
            } else if (inputChar == 'c') {
                ;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 11:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                state = 16;
            } else if (inputChar == 'c') {
                ;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 12:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                ;
            } else if (inputChar == 'c') {
                state = 12;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 13:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                ;
            } else if (inputChar == 'c') {
                ;
            } else if (inputChar == 'd') {
                ;
            } else {
                unknowInputChar();
            }
            break;
        case 14:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                ;
            } else if (inputChar == 'c') {
                ;
            } else if (inputChar == 'd') {
                state = 2;
            } else {
                unknowInputChar();
            }
            break;
        case 15:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                ;
            } else if (inputChar == 'c') {
                ;
            } else if (inputChar == 'd') {
                state = 3;
            } else {
                unknowInputChar();
            }
            break;
        case 16:
            if (inputChar == 'a') {
                ;
            } else if (inputChar == 'b') {
                ;
            } else if (inputChar == 'c') {
                ;
            } else if (inputChar == 'd') {
                state = 8;
            } else {
                unknowInputChar();
            }
            break;
    }

    return state;
}
