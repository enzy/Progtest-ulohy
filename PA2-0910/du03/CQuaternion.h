/* 
 * File:   CQuaternion.h - Kvaterniony
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 1. duben 2010, 22:11
 *
 * http://en.wikipedia.org/wiki/Quaternion
 */

/* Při realizaci rozhraní (operátorů) dejte dobrý pozor na kvalifikátory 
 * const. Testovací prostředí vyžaduje, aby realizované metody a přetížené 
 * operátory správně deklarovaly, že nemodifikují parametry tam, kde to 
 * není potřeba (např. operandy pro sčítání).          
 */
#include <iostream>
#include <cmath>

using namespace std;

#ifndef __PROGTEST__

class InvalidIndexException {
public:

    InvalidIndexException(void) {
        cout << "InvalidIndexException" << endl;
        exit(EXIT_FAILURE);
    }
};
#endif /* __PROGTEST__ */

class CQuaternion {
public:

    // konstruktor implicitni
    // Implicitní konstruktor - vytvoří kvaternion se složkami [0,0,0,0].

    CQuaternion(void) {
        m_A = 0;
        m_B = 0;
        m_C = 0;
        m_D = 0;
    };

    // konstruktor uziv. konverze double -> kvaternion
    // konstruktor uživatelské konverze z datového typu double na kvaternion. Při konverzi se desetinné číslo uloží do reálné složky (m_A), ostatní složky budou nastavené na hodnoty 0.

    CQuaternion(double s) {
        m_A = s;
        m_B = 0;
        m_C = 0;
        m_D = 0;
    };

    // konstruktor se 4 parametry typu double (slozkami)

    CQuaternion(double s, double i, double j, double k) {
        m_A = s;
        m_B = i;
        m_C = j;
        m_D = k;
    };

    // konstruktor kopirujici (je-li treba)
    // Kopírující konstruktor bude realizován pokud to implementace třídy bude vyžadovat.

    //CQuaternion(const CQuaternion &CQ) {    };

    // destruktror (je-li treba)
    // Destruktor bude implementován, pokud to implementace třídy bude vyžadovat.

    //~CQuaternion(void) {    };

    // operator = (je-li treba)
    // Operátor = bude přetížen, pokud to implementace třídy bude vyžadovat.

    // CQuaternion operator=(const CQuaternion &kopie) {};

    // operator ==
    // Budou realizované přetížené operátory == a !=, které porovnají kvaterniony a vrátí bool hodnotu výsledku porovnání.

    bool operator==(CQuaternion druhy) const {
        if (m_A == druhy.m_A && m_B == druhy.m_B && m_C == druhy.m_C && m_D == druhy.m_D) return true;
        return false;
    };

    // operator !=

    bool operator!=(CQuaternion druhy) const {
        if (m_A == druhy.m_A && m_B == druhy.m_B && m_C == druhy.m_C && m_D == druhy.m_D) return false;
        return true;
    };

    // metoda Norm
    // Bude realizovaná metoda Norm, která vrací velikost (normu) kvaternionu jako desetinné číslo - návratovou hodnotu (předpis - viz Wikipedia).

    double Norm(void) const {
        return sqrt(m_A * m_A + m_B * m_B + m_C * m_C + m_D * m_D);
    };

    // operator [] (index 0,1,2,3 - getter)

    /* Bude přetížený operátor [], kterým půjde celočíselnými indexy 0, 1, 2 a 3
     * získávat složky kvaternionu (m_A, m_B, m_C a m_D, v tomto pořadí). Bude
     * se tedy jednat o getter. Pokud bude indexováno jiným indexem, vvyvolá
     * volání výjimku InvalidIndexException (volání: throw InvalidIndexException();).
     */
    double operator[](int index) const {
        switch (index) {
            case 0: return m_A;
            case 1: return m_B;
            case 2: return m_C;
            case 3: return m_D;
            default: throw InvalidIndexException();
        }
        return 0;
    };

    // operator () kvaternarni pro 4 desetinna cisla (setter)
    // Bude přetížen operátor () jako kvaternární operátor, kterým půjde nastavit jednotlivé složky kvaternionu (setter pro všechny složky najednou, nic nevrací).

    void operator() (long double s, long double i, long double j, long double k) {
        m_A = s;
        m_B = i;
        m_C = j;
        m_D = k;
    };

    // operator +
    // Bude realizován přetížený operátor +, který provede součet dvou kvaternionů.

    CQuaternion operator+(const CQuaternion druhy) const {
        return CQuaternion(m_A + druhy.m_A, m_B + druhy.m_B, m_C + druhy.m_C, m_D + druhy.m_D);
    };

    // operator -
    // Bude realizovaný přetížený operátor -, který vypočete rozdíl dvou kvaternionů.

    CQuaternion operator-(const CQuaternion druhy) const {
        return CQuaternion(m_A - druhy.m_A, m_B - druhy.m_B, m_C - druhy.m_C, m_D - druhy.m_D);
    };

    // operator *
    // Bude realizovaný přetížený operátor *, který vypočete součin dvou kvaternionů (pozor - kvaterniony nejsou komutativní).

    CQuaternion operator*(const CQuaternion druhy) const {
        long double s1, x1, y1, z1;
        long double s2, x2, y2, z2;
        long double s, i, j, k;

        s1 = m_A;
        x1 = m_B;
        y1 = m_C;
        z1 = m_D;

        s2 = druhy.m_A;
        x2 = druhy.m_B;
        y2 = druhy.m_C;
        z2 = druhy.m_D;

        /*
         q1 * q2 =  (s1 + x1i + y1j + z1k) * (s2 + x2i + y2j + z2k) =
                    (s1s2 - x1x2 - y1y2 - z1z2) + (s1x2 + s2x1 + y1z2 - y2z1)i +
                    (s1y2 + s2y1 + x2z1 - x1z2)j + (s1z2 + s2z1 + x1y2 - x2y1)k
         */
        s = (s1 * s2) - (x1 * x2) - (y1 * y2) - (z1 * z2);
        i = (s1 * x2) + (s2 * x1) + (y1 * z2) - (y2 * z1);
        j = (s1 * y2) + (s2 * y1) + (x2 * z1) - (x1 * z2);
        k = (s1 * z2) + (s2 * z1) + (x1 * y2) - (x2 * y1);

        return CQuaternion(s, i, j, k);
    };

    // operator /
    // Bude realizovaný přetížený operátor /, který vypočete podíl dvou kvaternionů (x / y vypočte pravý podíl, tedy x * y-1).

    CQuaternion operator/(const CQuaternion druhy) const {
        CQuaternion soucinitel(m_A, m_B, m_C, m_D);
        long double norm = druhy.Norm();
        norm *= norm;
        long double s, i, j, k;
        // q^-1 = q.konjug / q.norm^2
        s = druhy.m_A / norm;
        i = druhy.m_B / norm * (-1);
        j = druhy.m_C / norm * (-1);
        k = druhy.m_D / norm * (-1);
        CQuaternion inverzni(s, i, j, k);

        return soucinitel * inverzni;
    };

    /* Budou realizované přetížené operátory << a >>, které umožní formátovaný
     * výstup a vstup kvaternionu do výstupního resp. vstupního proudu.
     * Formátování pro výstupní proud je potřeba dodržet podle ukázky níže
     * (včetně mezer, za pravou hranatou závorkou není žádný další znak, formát
     * zobrazní desetinných čísel ponechte beze změn). Pro vstup ze vstupního
     * proudu umožněte čtení s libovolně vloženým whitespace, ale požadujte
     * přítomnost hranatých závorek a čárek. Kvaternion nemusí být posledním
     * obsahem vstupního streamu, není tedy úkolem kontrolovat eof. Pokud se při
     * čtení kvaternionu vyskytne chyba (např. nesprávný formát čísla, chybějící
     * čárka, ...), pak nastavte ve vstupním streamu fail bit -
     * volání is . setstate ( ios::failbit ) a ponechte původní obsah instance
     * kvaternionu beze změny.
     */

    // operator >>

    //istream& operator>> (istream& is, string& str){    };

    friend std::istream & operator>>(std::istream& is, CQuaternion& q) {
        long double s, i, j, k;
        char enC, sep1, sep2, sep3, esC;

        is >> enC >> s >> sep1 >> i >> sep2 >> j >> sep3 >> k >> esC;

        if (!is.good() || enC != '[' || sep1 != ',' || sep2 != ',' || sep3 != ',' || esC != ']') {
            // Pokud nastane nejaka chyba
            is.setstate(ios::failbit);
            return is;
        }

        // Pokud probehlo vsechno spravne, nastav hodnoty
        q(s, i, j, k);

        //return is;
        return is;
    };

    // operator <<

    friend std::ostream & operator<<(std::ostream& os, const CQuaternion& q) {
        long double s, i, j, k;
        s = q.m_A;
        i = q.m_B;
        j = q.m_C;
        k = q.m_D;
        os << "[ " << s << ", " << i << ", " << j << ", " << k << " ]";
        return os;
    };



protected:
    // Členské proměnné m_A, m_B, m_C a m_D budou uchovávat hodnoty jednotlivých složek kvaternionu (m_A reálnou, m_B složku i, m_C složku j a m_D složku k).
    long double m_A, m_B, m_C, m_D; // slozky realna, i, j, k
};
// pripadne podpurne globalni funkce


