/* 
 * File:   Vyhledávání binárním půlením
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 27. březen 2010, 23:55
 */

#include <stdlib.h>
#include <iostream>
#include "CSetInt.h"

#include <time.h>
#include <exception>

using namespace std;

void print_error(int ln) {
    cout << "Nespravny vstup. [" << ln << "])" << endl;
    exit(EXIT_FAILURE);
}

void print_ok(int ln) {
    cout << "OK. [" << ln << "])" << endl;
}

int main(int argc, char** argv) {

    bool mydebug = true;

    bool testMazani = true;
    bool testTestovani = false;

    if (mydebug) {
        bool status;
        CSetInt S1;


        time_t start, konec;
        // initialize random seed:
        srand(time(NULL));

        start = time(NULL);

        int pocet_prvku = 100000; // progtest 1000000

        for (int i = 1; i <= pocet_prvku; i++) {
            if (!S1.Insert(rand() % pocet_prvku + 1)) {
                //cout << "Vkladani prvku " << i << " neprovedeno" << endl;
                ;
            }
            // Indikace prubehu
            if (i == 1) cout << "# Vkladani zacalo" << endl;
            if (i == pocet_prvku * 0.25) cout << "# Vkladani je ve 1/4" << endl;
            if (i == pocet_prvku * 0.5) cout << "# Vkladani je ve 2/4" << endl;
            if (i == pocet_prvku * 0.75) cout << "# Vkladani je ve 3/4" << endl;
            if (i == pocet_prvku) cout << "# Vkladani je ve 4/4" << endl;
        }

        konec = time(NULL);
        cout << "# Potrebny cas: " << difftime(konec, start) << "s" << endl;

        if (S1.testujSerazeni()) cout << "# Vysledek je spravne serazen !" << endl;
        else {
            cout << "!!! CHYBA !!! Vysledek neni spravne serazen" << endl;
            cout << "Chybna rada: ";
            S1.print();
        }

        if (testMazani) {
            cout << "# Testovani delete..." << endl;

            for (int i = 0; i < pocet_prvku; i++) {
                S1.Delete(i);
            }

            if (S1.testujSerazeni()) cout << "# Vysledek je spravne serazen !" << endl;
            else {
                cout << "!!! CHYBA !!! Vysledek neni spravne serazen" << endl;
                // cout << "Chybna rada: ";
                // S1.print();
            }
        }

        if (testTestovani) {
            cout << "# Testovani isSet..." << endl;

            for (int i = 0; i < pocet_prvku; i++) {
                S1.IsSet(i);
            }

            if (S1.testujSerazeni()) cout << "# Vysledek je spravne serazen !" << endl;
            else {
                cout << "!!! CHYBA !!! Vysledek neni spravne serazen" << endl;
                // cout << "Chybna rada: ";
                // S1.print();
            }
        }


    } else {

        CSetInt S1;
        bool status;

        status = S1 . Insert(10);
        // status = true, S1 . Size () => 1
        if (!status || S1.Size() != 1) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S1 . Insert(20);
        // status = true, S1 . Size () => 2
        if (!status || S1.Size() != 2) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S1 . Insert(30);
        // status = true, S1 . Size () => 3
        if (!status || S1.Size() != 3) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S1 . IsSet(20);
        // status = true
        if (!status) print_error(__LINE__);
        else print_ok(__LINE__);

        CSetInt S2;
        status = S2 . Insert(10);
        // status = true, S2 . Size () => 1
        if (!status || S2.Size() != 1) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S2 . Insert(20);
        // status = true, S2 . Size () => 2
        if (!status || S2.Size() != 2) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S2 . Insert(30);
        // status = true, S2 . Size () => 3
        if (!status || S2.Size() != 3) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S2 . IsSet(30);
        // status = true
        if (!status) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S2 . Delete(30);
        // status = true, S2 . Size () => 2
        if (!status || S2.Size() != 2) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S2 . IsSet(30);
        // status = false
        if (status) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S2 . Delete(30);
        // status = false, S2 . Size () => 2
        if (status || S2.Size() != 2) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S2 . Insert(30);
        // status = true, S2 . Size () => 3
        if (!status || S2.Size() != 3) print_error(__LINE__);
        else print_ok(__LINE__);

        status = S2 . IsSet(30);
        // status = true
        if (!status) print_error(__LINE__);
        else print_ok(__LINE__);
    }

    return (EXIT_SUCCESS);
}

