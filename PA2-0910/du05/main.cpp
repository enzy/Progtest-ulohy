/* 
 * File:   main.cpp - Adresářový strom
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 17. duben 2010, 16:02
 */

#include <stdlib.h>
#include <cstring>
#include <iostream>

using namespace std;

#include "CFileSystem.h"

/*
 * 
 */
int main(int argc, char** argv) {

    char buffer[100];
    bool status;
    CFileSystem fs1;
    status = fs1 . MkDir("/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test", false); // vraci true
    cout << status << "(1)" << endl;
    status = fs1 . MkDir("/prvni", false); // vraci true
    cout << status << "(1)" << endl;
    status = fs1 . MkDir("/dalsi", false); // vraci true
    cout << status << "(1)" << endl;
    cout << fs1;
    /*--------------------------
    /
    +-/dalsi
    +-/prvni
    +-/test
    ----------------------------*/
    CFileSystem fs2;
    status = fs2 . MkDir("/test", false); // vraci true
    cout << status << "(1)" << endl;
    status = fs2 . MkDir("/pokus", false); // vraci true
    cout << status << "(1)" << endl;
    status = fs2 . MkDir("/test/pokus", false); // vraci true
    cout << status << "(1)" << endl;
    status = fs2 . MkDir("/test/pokus/hokus", false); // vraci true
    cout << status << "(1)" << endl;
    status = fs2 . MkDir("/pokus/pokus/pokus/pokus/hokus", false); // vraci false
    cout << status << "(0)" << endl;
    cout << fs2;
    /*--------------------------
    /
    +-/pokus
    +-/test
      +-/pokus
        +-/hokus
    ----------------------------*/
    status = fs2 . MkDir("/pokus/pokus/pokus/pokus/hokus", true); // vraci true
    cout << status << "(1)" << endl;
    cout << fs2;
    /*--------------------------
    /
    +-/pokus
    | +-/pokus
    |   +-/pokus
    |     +-/pokus
    |       +-/hokus
    +-/test
      +-/pokus
        +-/hokus
    ----------------------------*/
    CFileSystem fs3;
    status = fs3 . MkDir("/test", false); // vraci true
    cout << status << "(1)" << endl;
    status = fs3 . MkDir("/test", false); // vraci false
    cout << status << "(0)" << endl;
    status = fs3 . MkDir("pokus", false); // vraci false
    cout << status << "(0)" << endl;
    status = fs3 . MkDir("//hokus", false); // vraci false
    cout << status << "(0)" << endl;
    status = fs3 . MkDir("/test//pokus", false); // vraci false
    cout << status << "(0)" << endl;
    cout << fs3;
    /*--------------------------
    /
    +-/test
    ----------------------------*/
    CFileSystem fs4;
    strncpy(buffer, "/test1", sizeof ( buffer));
    status = fs4 . MkDir(buffer, true); // vraci true
    cout << status << "(1)" << endl;
    strncpy(buffer, "/test2", sizeof ( buffer));
    status = fs4 . MkDir(buffer, true); // vraci true
    cout << status << "(1)" << endl;
    strncpy(buffer, "/test3", sizeof ( buffer));
    status = fs4 . MkDir(buffer, true); // vraci true
    cout << status << "(1)" << endl;
    cout << fs4;
    /*--------------------------
    /
    +-/test1
    +-/test2
    +-/test3
    ----------------------------*/
    CFileSystem fs5;
    status = fs5 . MkDir("/a/b/c/d/e/f/g/h/i/j/k/l/m", true); // vraci true
    cout << status << "(1)" << endl;
    status = fs5 . MkDir("/a/b/c/d/e/f/g/h/i/A/B/C", true); // vraci true
    cout << status << "(1)" << endl;
    status = fs5 . MkDir("/zzzzzzz", true); // vraci true
    cout << status << "(1)" << endl;
    cout << fs5;
    /*--------------------------
    /
    +-/a
    | +-/b
    |   +-/c
    |     +-/d
    |       +-/e
    |         +-/f
    |           +-/g
    |             +-/h
    |               +-/i
    |                 +-/A
    |                 | +-/B
    |                 |   +-/C
    |                 +-/j
    |                   +-/k
    |                     +-/l
    |                       +-/m
    +-/zzzzzzz
    ----------------------------*/
    status = fs5 . RmDir("/a/b/c/d/e/f/g/h/i/j/k/l/m", false); // vraci true
    cout << status << "(1)" << endl;
    status = fs5 . RmDir("/a/b/c/d/e/f/g/h/i/j/k/A", false); // vraci false
    cout << status << "(0)" << endl;
    cout << fs5;
    /*--------------------------
    /
    +-/a
    | +-/b
    |   +-/c
    |     +-/d
    |       +-/e
    |         +-/f
    |           +-/g
    |             +-/h
    |               +-/i
    |                 +-/A
    |                 | +-/B
    |                 |   +-/C
    |                 +-/j
    |                   +-/k
    |                     +-/l
    +-/zzzzzzz
    ----------------------------*/
    status = fs5 . RmDir("/a/b/c", true); // vraci true
    cout << status << "(1)" << endl;
    cout << fs5;
    /*--------------------------
    /
    +-/a
    | +-/b
    +-/zzzzzzz
    ----------------------------*/
    status = fs5 . RmDir("/", true); // vraci true
    cout << status << "(1)" << endl;
    cout << fs5;
    /*--------------------------
    /
    ----------------------------*/



    return (EXIT_SUCCESS);
}

