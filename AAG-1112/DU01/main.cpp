/* 
 * File:   main.cpp
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 19. listopad 2011, 19:02
 */

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

/**
 * students' interface
 * @param inputString null terminated string with automaton input
 * @return true if string has been accepted
 */
bool checkString(const char * inputString);

#ifndef __PROGTEST__
/*
 * Localhost main for testing purposes
 */
int main(int argc, char** argv) {
    using namespace std;
    string line;
    while (getline(cin, line)){
        cout << checkString(line.c_str()) << endl;
    }
}
#endif

bool checkString(const char * inputString){    
    /* Transition table [state][input] */
    static char transitions[][4] = {
        /* a, b, c, d */
        {9, 1, 4, 5},
        {-1, -1, -1, 2},
        {3, -1, -1, -1},
        {-1, -1, 6, -1},
        {8, -1, -1, -1},
        {11, -1, -1, -1},
        {21, -1, -1, -1},
        {22, -1, -1, -1},
        {-1, -1, -1, 10},
        {-1, -1, 7, 13},
        {-1, -1, -1, 15},
        {-1, -1, -1, 16},
        {-1, -1, -1, 17},
        {14, 12, 4, 19},
        {-1, -1, 7, 20},
        {-1, 21, -1, -1},
        {-1, 23, -1, -1},
        {3, 21, -1, -1},
        {3, 23, -1, -1},
        {11, 21, -1, -1},
        {14, 24, 4, 19},
        {-1, -1, -1, -1},
        {22, -1, -1, -1},
        {-1, -1, -1, 16},
        {-1, -1, -1, 18}
    };
    /* Start state: 0, Final states: 24,23,22,21 */

    int state = 0; // state caching    
    bool final = false; // final state testing

    // Run on all individual letters on input
    for (int i = 0; inputString[i]; i++) {
        // convert char to array index
        int input;
        switch (inputString[i]){
            case 'a':
                input = 0;
                break;
            case 'b':
                input = 1;
                break;
            case 'c':
                input = 2;
                break;
            case 'd':
                input = 3;
                break;
            default:
                return false; // something not pretty came to our input
        }        

        char nextState = transitions[state][input]; // read transition from actual state to another by input index
        
        if(nextState != -1){ // only non empty transition shall pass
            state = nextState;
        } else if(state != 0){
            return false; // kill the beast on empty transition
        }
    }

    // test if actual state is final one and set flag eventually
    if(state >= 21 && state <= 24){
        final = true;
    }

    // return my opinion about input
    return final;
}
