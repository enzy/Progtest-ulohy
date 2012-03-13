/* 
 * File:   Kvaterniony
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 1. duben 2010, 19:34
 */

#include <stdlib.h>
#include <iostream>
//#include <istream>
//#include <string>
//#include <iosfwd>
#include <sstream>

#include "CQuaternion.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    const CQuaternion A(1.0, 2.0, 3.0, 4.0);
    const CQuaternion B(8.0, 7.0, 6.0, 5.0);
    
    CQuaternion C = 2;
    CQuaternion D;

    /*CQuaternion E = C;
    cout << C << endl;*/

    cout << A << B << endl; // [ 1, 2, 3, 4 ][ 8, 7, 6, 5 ]
    D = A + B; // [ 9, 9, 9, 9 ]
    cout << D << endl;
    D = A - B; // [ -7, -5, -3, -1 ]
    cout << D << endl;
    D = A * B; // [ -44, 14, 48, 28 ]
    cout << D << endl;
    D = B * A; // [ -44, 32, 12, 46 ]
    cout << D << endl;
    D = A / B; // [ 0.344828, 0.103448, 0, 0.206897 ]
    cout << "A/B " << D << endl;
    D = B / A; // [ 2, -0.6, -5.55112e-17, -1.2 ]
    cout << "B/A " << D << endl;

    bool res;
    res = A == B; // false
    cout << res << endl;
    res = A != B; // true
    cout << res << endl;

    istringstream is("[-5,-7,2,4]");
    is >> D;
    cout << D << endl; // [ -5, -7, 2, 4 ], is . fail () = false
    cout << is.fail() << endl;
    is.str("[-2,-3 4,5]");
    is >> D;
    cout << "[ -5, -7, 2, 4 ]: " << D << endl; // D se nemeni: [ -5, -7, 2, 4 ], is . fail () = true
    cout << is.fail() << endl;
    D(4, 5, 6, 7); // operator () - setter
    cout << "Getter (6): " << D[2] << endl; // getter, zde vraci 6
    cout << "Vyvola InvalidIndexException: " << D[12] << endl; // vyvola InvalidIndexException


    return (EXIT_SUCCESS);
}


