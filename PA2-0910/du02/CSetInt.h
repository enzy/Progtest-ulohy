/*
 * File:   Vyhledávání binárním půlením
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 27. březen 2010, 23:55
 */

#include <iostream>
using namespace std;

class CSetInt {
    int size; // realny pocet prvku
    int pole_size; // velikost pole pro prvky
    int * pole; // pole pro prvky    

    bool MakeMoreSpace() {
        /* Vyhraz misto na dalsi prvky */
        pole_size *= 2;
        pole = (int*) realloc(pole, (pole_size) * sizeof (int));
        if (pole == NULL) return false;
        return true;
    }

public:

    // Konstruktor

    CSetInt(void) {
        size = 0; // prazdno
        pole_size = 1000; // pocatecni velikost pole
        pole = (int*) calloc(pole_size, sizeof (int));
        if (pole == NULL) print_error(__LINE__);
    };

    // Desktruktor

    ~CSetInt(void) {
        free(pole);
    };

    // Testovani spravnosti algorytmu

    bool testujSerazeni() {
        for (int i = 0; i < size - 1; i++) {
            if (pole[i] > pole[i + 1]) continue;
            else return false;
        }
        return true;
    }

    // Binarni vyhledavani, viceucelove

    int vyhledejBinarnimPulenim(int hodnota, int levyIndex, int pravyIndex, int &poziceUkonceni) {
        // Dokud se pohybujes odleva doprava, hledej
        while (levyIndex <= pravyIndex) {
            // Spočítej prostředek
            int prostredniIndex = (levyIndex + pravyIndex) / 2;
            // Pokud je hodnota vetsi, pokracuj v horni polovine
            if (hodnota < pole[prostredniIndex])
                levyIndex = prostredniIndex + 1;
                // Pokud mensi, pokracuj v dolni polovine
            else if (hodnota > pole[prostredniIndex])
                pravyIndex = prostredniIndex - 1;
            else
                // Jinak jsme nasli prvek a vratime jeho pozici
                return prostredniIndex;
        }
        // Prvek nebyl nalezen a vratime -1 a pozici, na kterou ma byt vlozen
        poziceUkonceni = levyIndex;
        return -1;
    };

    // Vlozeni noveho prvku

    bool Insert(int X) {
        // Aby se vesel dalsi prvek
        if (size + 1 >= pole_size) {
            if (!MakeMoreSpace()) return false;
        }

        int poziceProVlozeni = -1;

        // Prvni prvek
        if (size == 0) {
            pole[0] = X;
            size++;
            return true;

            // Dva prvky
        } else if (size == 1) {
            if (pole[0] > X) pole[1] = X;
            else if (pole[0] < X) {
                pole[1] = pole[0];
                pole[0] = X;
            } else {
                return false;
            }
            size++;
            return true;

            // Tri prvky
        } else if (size == 2) {
            if (pole[0] == X || pole[1] == X) return false;
            if (X < pole[1]) {
                pole[2] = X;
            } else if (X < pole[0]) {
                pole[2] = pole[1];
                pole[1] = X;
            } else if (X > pole[0]) {
                pole[2] = pole[1];
                pole[1] = pole[0];
                pole[0] = X;
            }
            size++;
            return true;

            // 4 a vice
        } else {
            // Zkus najit prvek v poli a pokud tam je, vrat false, jinak ziskej pozici ukonceni
            if (vyhledejBinarnimPulenim(X, 0, size - 1, poziceProVlozeni) != -1) return false;
            size++;
        }

        // Udelej pro nej misto posunutim prvku doprava
        for (int i = size - 1; i > poziceProVlozeni; i--) {
            pole[i] = pole[i - 1];
        }
        pole[poziceProVlozeni] = X;

        return true;
    };

    // Vyhledej a smaz prvek v poli

    bool Delete(int X) {

        if (size == 0) return false;
        if (size == 1) {
            if (pole[0] != X) return false;
            pole[0] = 0;
            size = 0;
            return true;
        }

        // Vyhledej, zdali je prvek v poli
        int poziceProVlozeni = -1;
        int vyhledanaPozice = vyhledejBinarnimPulenim(X, 0, size - 1, poziceProVlozeni);

        if (vyhledanaPozice == -1) return false;

        // Pokud ano, prepis hodnoty smerem zleva doprava
        for (int i = vyhledanaPozice; i < size - 1; i++) {
            pole[i] = pole[i + 1];
        }
        size--;

        // Vrat uspech
        return true;
    };

    // Vyhledej prvek v poli

    bool IsSet(int X) {
        int poziceProVlozeni = -1;
        if (vyhledejBinarnimPulenim(X, 0, size - 1, poziceProVlozeni) != -1) return true;
        return false;
    };

    // Vrat velikost

    int Size(void) {
        return size;
    };

    // Vytiskni obsah pole

    void print() {
        cout << "CSetInt (" << size << "/" << pole_size << "): ";
        for (int i = 0; i < size; i++) {
            cout << pole[i];
            if (i + 1 < size) cout << ", ";
        }
        cout << endl;
    }

    // Vytiskni chybu

    void print_error(int ln) {
        cout << "Nespravny vstup. [" << ln << "])" << endl;
        exit(EXIT_FAILURE);
    }

};
