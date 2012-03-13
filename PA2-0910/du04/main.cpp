/* 
 * File:   main.cpp
 * Author: Enzy
 *
 * Created on 9. duben 2010, 23:11
 */

#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include "CList.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    bool status;
    int tempInt;
    string tempStr;

    
    cout << "#1" << endl;

    CList<int> LI1;

    status = LI1.InsEnd(3000);
    // status = true, LI1.Size () => 1
    // (3000)
    cout << status << ", " << LI1.Size() << endl;
    LI1.Print();
    
    status = LI1.InsEnd(5000);
    // status = true, LI1.Size () => 2
    // (3000, 5000)
    cout << status << ", " << LI1.Size() << endl;
    LI1.Print();

    status = LI1.InsEnd(1000);
    // status = true, LI1.Size () => 3
    // (3000, 5000, 1000)
    cout << status << ", " << LI1.Size() << endl;
    LI1.Print();

    status = LI1.Read(0, tempInt);
    // status = true, tempInt=3000
    cout << status << ", " << tempInt << endl;

    status = LI1.Read(1, tempInt);
    // status = true, tempInt=5000
    cout << status << ", " << tempInt << endl;

    status = LI1.Read(2, tempInt);
    // status = true, tempInt=1000
    cout << status << ", " << tempInt << endl;

    
    cout << endl << "#2" << endl;

    CList<int> LI2;
    status = LI2.InsStart(10);
    // status = true, LI2.Size () => 1
    // (10)
    cout << status << ", " << LI2.Size() << endl;
    LI2.Print();


    status = LI2.InsAfter(0, 20);
    // status = true, LI2.Size () => 2
    // (10, 20)
    cout << status << ", " << LI2.Size() << endl;
    LI2.Print();

    status = LI2.InsEnd(30);
    // status = true, LI2.Size () => 3
    // (10, 20, 30)
    cout << status << ", " << LI2.Size() << endl;
    LI2.Print();

    status = LI2.InsBefore(2, 40);
    // status = true, LI2.Size () => 4
    // (10, 20, 40, 30)
    cout << status << ", " << LI2.Size() << endl;
    LI2.Print();

    status = LI2.Read(0, tempInt);
    // status = true, tempInt=10
    cout << status << ", " << tempInt << endl;

    status = LI2.Read(1, tempInt);
    // status = true, tempInt=20
    cout << status << ", " << tempInt << endl;

    status = LI2.Read(2, tempInt);
    // status = true, tempInt=40
    cout << status << ", " << tempInt << endl;

    status = LI2.Read(3, tempInt);
    // status = true, tempInt=30
    cout << status << ", " << tempInt << endl;


    
    cout << endl << "#3" << endl;

    CList<int> LI3;
    status = LI3.InsEnd(10);
    // status = true, LI3.Size () => 1
    // (10)
    cout << status << ", " << LI3.Size() << endl;
    LI3.Print();

    status = LI3.InsEnd(20);
    // status = true, LI3.Size () => 2
    // (10, 20)
    cout << status << ", " << LI3.Size() << endl;
    LI3.Print();

    status = LI3.InsEnd(30);
    // status = true, LI3.Size () => 3
    // (10, 20, 30)
    cout << status << ", " << LI3.Size() << endl;
    LI3.Print();

    status = LI3.InsEnd(40);
    // status = true, LI3.Size () => 4
    // (10, 20, 30, 40)
    cout << status << ", " << LI3.Size() << endl;
    LI3.Print();

    status = LI3.Delete(2);
    // status = true, LI3.Size () => 3
    // (10, 20, 40)
    cout << status << ", " << LI3.Size() << endl;
    LI3.Print();

    status = LI3.InsBefore(2, 50);
    // status = true, LI3.Size () => 4
    // (10, 20, 50, 40)
    cout << status << ", " << LI3.Size() << endl;
    LI3.Print();

    status = LI3.Read(3, tempInt);
    // status = true, tempInt=40
    cout << status << ", " << tempInt << endl;

    status = LI3.InsBefore(4, 60);
    // status = false, LI3.Size () => 4
    // (10, 20, 50, 40)
    cout << status << ", " << LI3.Size() << endl;
    LI3.Print();

    status = LI3.InsAfter(3, 70);
    // status = true, LI3.Size () => 5
    // (10, 20, 50, 40, 70)
    cout << status << ", " << LI3.Size() << endl;
    LI3.Print();


    
    cout << endl << "#4" << endl;

    CList<string> LS;
    status = LS.InsEnd("first");
    // status = true, LS.Size () => 1
    // (first)
    cout << status << ", " << LS.Size() << endl;
    LS.Print();

    status = LS.InsEnd("third");
    // status = true, LS.Size () => 2
    // (first, third)
    cout << status << ", " << LS.Size() << endl;
    LS.Print();

    status = LS.InsAfter(0, "second");
    // status = true, LS.Size () => 3
    // (first, second, third)
    cout << status << ", " << LS.Size() << endl;
    LS.Print();

    status = LS.Read(2, tempStr);
    // status = true, tempStr="third"
    cout << status << ", " << tempStr << endl;


    cout << endl << "# Custom testing" << endl;

    CList<int> test(LI1);

    LI1.Print();

    test.InsEnd(888888);
    test.Print();

    for (int i = 0; i < 1000; i++) {
        test.InsEnd(i);
    }
    //test.Print();


    test = LI1;
    test.Print();

    LI1.InsEnd(69);
    LI1.InsEnd(96);
    LI1.InsStart(696);

    test.InsEnd(333);
    test.InsStart(666);
    
    LI1.Print();
    test.Print();    



    cout << endl << "END" << endl;


    return (EXIT_SUCCESS);
}

