/* 
 * File:   CList.h - Generický seznam prvků
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 9. duben 2010, 23:38
 */

/*

Úkolem je realizovat třídu CList, která bude generickou datovou strukturou -
seznamem prvků zadaného typu (generický parametr). Třída bude umět provádět
základní operace nad seznamem jako je vkládání, odebírání a dotazování.
Vnitřně bude třída realizována pomocí dvojsměrně zřetězeného spojového seznamu.
                             
Implementovaná třída nesmí používat datové struktury z STL (vector, list, ...).
Jejich použití povede k chybě při kompilaci. Třída je testovaná v omezeném
prostředí, kde je limitovaná dostupná paměť (postačuje pro uožení zadávaných
dat a režii pro spojové seznamy) a je omezena dobou běhu (10s na testovacím
počítači pro cca 10000 vkládaných/odebíraných prvků, referenční řešení pro
výpočet potřebuje cca 1sec).

 */

#include <iostream>
using namespace std;

/* O generickém parametru T víte, že se jedná o datový typ, který správně
 * implementuje operace kopírování a výmaz. Může se tedy jednat o primitivní
 * datový typ (int, double, ...), nebo o třídu, která má správně implementovaný
 * operátor =, destruktor a kopírující konstruktor. */
template <class T>

class CList {
public:

    /* 
     *  Konstruktor bez parametrů inicializuje novou prázdnou instanci seznamu.
     */
    CList(void) {
        this->m_First = NULL;
        this->m_Last = NULL;
        this->size = 0;
    };

    // pokud je potreba:

    /* Kopírující konstruktor bude ve třídě realizovnán, pokud automaticky
       generovaný kopírující konstruktor nevyhoví pro správnou funkci kopírování */    

    CList(const CList &zdroj) {
        this->m_First = NULL;
        this->m_Last = NULL;
        this->size = 0;

        for (int i = 0; i < zdroj.size; i++) {
            T temp;
            zdroj.Read(i, temp);
            this->InsEnd(temp);
        }
    };

    /* Destruktor odstraní prostředky alokované instancí. Bude realizován, pokud
       autoaticky generovaný destruktor pro správnou funkci nevyhovuje. */    
    ~CList(void){
        this->Vymazani();
    };

    void Vymazani(void){
        for (int i = size - 1; i >= 0; i--) {
            this->Delete(i);
        }
    }


    /* Přetížený operátor = provede hlubokou kopii seznamu. Bude realizován, pokud
       automaticky generovaný operátor = nevyhovuje pro správnou funkci kopírování. */    
    CList & operator=(const CList &kopie) {
        if (this == &kopie) return *this;

        //cout << "Kopirovani " << kopie.size << " prvku" << endl;

        this->Vymazani();

        for (int i = 0; i < kopie.size; i++) {
            T temp;
            kopie.Read(i, temp);
            this->InsEnd(temp);
        }
        return *this;
    };

    /* Metoda Size zjistí počet prvků v seznamu. */
    int Size(void) const {
        return size;
    };

    /* Spocitani poctu polozek projetim celeho listu */
    int RealSize(void) const {
        // TODO RealSize
        return 0;
    };   

    /* Metoda InsBefore vloží před stávající prvek seznamu na pozici pos nový prvek.
       Metoda vrací úspěch (true), pokud se vložení zdařilo a neúspěch (false), pokud
       se vložení nezdařilo. Neúspěchem operace skončí např. pokud je zadán index
       neexistujícího prvku. */
    bool InsBefore(int pos, const T & x) {

        // Kontrola spatne pozice - mimo rozsah
        if (pos >= size || pos < 0) return false;
        // Pri vkladani na zacatek pouzit odpovidajici metodu
        if (pos == 0) return InsStart(x);
        
        TItem * predchozi;
        TItem * aktualni;
        TItem * nasledujici;        

        // Pruchod odpredu
        // TODO Pruchod odzadu
        predchozi = NULL;
        aktualni = m_First;
        nasledujici = m_First->m_Next;

        // Dojed na zadanou pozici
        for (int i = 0; i < pos; i++) {
            predchozi = aktualni;
            aktualni = nasledujici;
            nasledujici = nasledujici->m_Next;
        }

        TItem *novy_prvek;

        novy_prvek = new TItem;

        novy_prvek->m_Prev = predchozi;
        novy_prvek->m_Next = aktualni;
        novy_prvek->m_Val = x;

        predchozi->m_Next = novy_prvek;
        aktualni->m_Prev = novy_prvek;

        size++;

        return true;
    };

    /* Metoda InsAfter vloží za stávající prvek seznamu na pozici pos nový prvek.
       Metoda vrací úspěch (true), pokud se vložení zdařilo a neúspěch (false),
       pokud se vložení nezdařilo. Neúspěchem operace skončí např. pokud je zadán
       index neexistujícího prvku. */
    bool InsAfter(int pos, const T & x) {

        // Kontrola spatne pozice - mimo rozsah
        if (pos >= size || pos < 0) return false;
        // Pri vkladani na konec pouzit odpovidajici metodu
        if (pos == size - 1) return InsEnd(x);
        
        TItem * predchozi;
        TItem * aktualni;
        TItem * nasledujici;        

        // Pruchod odpredu
        // TODO Pruchod odzadu
        predchozi = NULL;
        aktualni = m_First;
        nasledujici = m_First->m_Next;

        // Dojed na zadanou pozici
        for (int i = 0; i < pos; i++) {
            predchozi = aktualni;
            aktualni = nasledujici;
            nasledujici = nasledujici->m_Next;
        }

        TItem *novy_prvek;

        novy_prvek = new TItem;

        novy_prvek->m_Prev = aktualni;
        novy_prvek->m_Next = nasledujici;
        novy_prvek->m_Val = x;

        aktualni->m_Next = novy_prvek;
        nasledujici->m_Prev = novy_prvek;

        size++;

        return true;
    };

    /* Metoda InsStart vloží nový prvek do seznamu na první pozici (před stávající
       první prvek). Úspěch operace je signalizován vracením hodnoty true (k neúspěchu by nemělo dojít). */
    bool InsStart(const T & x) {

        TItem *novy_prvek;
        TItem *nasledujici = NULL;

        novy_prvek = new TItem;

        nasledujici = m_First;

        novy_prvek->m_Prev = NULL;
        novy_prvek->m_Next = nasledujici != NULL ? nasledujici : NULL;
        novy_prvek->m_Val = x;       
        
        if (nasledujici != NULL) nasledujici->m_Prev = novy_prvek;                    

        novy_prvek->m_Next = nasledujici;

        m_First = novy_prvek;

        if (size == 0) m_Last = novy_prvek;

        size++;

        return true;
    };

    /* Metoda InsEnd vloží nový prvek na konec seznamu (za poslední stávající
       prvek). Úspěch operace je signalizován vracením hodnoty true (k neúspěchu by
       nemělo dojít). */
    bool InsEnd(const T & x) {

        TItem *novy_prvek;
        TItem *predchozi = NULL;

        novy_prvek = new TItem;

        predchozi = m_Last;

        novy_prvek->m_Prev = predchozi != NULL ? predchozi : NULL;
        novy_prvek->m_Next = NULL;
        novy_prvek->m_Val = x;

        if (predchozi != NULL) predchozi->m_Next = novy_prvek;

        novy_prvek->m_Prev = predchozi;

        m_Last = novy_prvek;

        if (size == 0) m_First = novy_prvek;

        size++;

        return true;
    };

    /* Metoda Read načte obsah z prvku seznamu na pozici pos a uloží jej do
       druhého (výstupního) parametru. Za neúspěch považováno, pokud je požadované
       načtení neexistujícího prvku (nesprávná hodnota pos). V takovém případě metoda
       vrací false. Úspěšné provedení operace čtení je signalizováno návratovou
       hodnotou true. */
    bool Read(int pos, T & x) const {

        // Kontrola spatne pozice - mimo rozsah
        if (pos >= size || pos < 0) return false;

        // TODO pruchod bud odpredu nebo odzadu, v zavislosti na umisteni

        TItem * aktualni = NULL;
        //TItem * predchozi = NULL;
        TItem * nasledujici = NULL;

        // Pruchod odpredu
        aktualni = m_First;

        // Dojed na zadanou pozici
        for (int i = 0; i < pos; i++) {
            nasledujici = aktualni->m_Next;
            aktualni = nasledujici;
        }
        // Navrat hodnotu
        // TODO kontrola spravneho typu!
        //if(x==NULL) return false;
        x = aktualni->m_Val;
        return true;        
    };

    /* Testovaci vystup hodnot */
    void Print(void){
        cout << "(";
        for (int i = 0; i < size; i++) {
            T temp;
            Read(i, temp);
            cout << temp;
            if ( i < size - 1) cout << ", ";
        }
        cout << ")" << endl;
    }

    /* Metoda Delete smaže prvek seznamu na pozici pos. Po úspšném provedení
       operace vrací hodnotu true, při neúspěchu (neexistující prvek) vrací false. */
    bool Delete(int pos) {

        // Kontrola spatne pozice - mimo rozsah
        if (pos >= size || pos < 0 || size<=0) return false;        

        // Odstraneni jedineho prvku
        if (size == 1) {
            TItem * aktualni;
            aktualni = m_First;

            delete aktualni;

            m_First = NULL;
            m_Last = NULL;

            size = 0;

        }// Odstraneni posledniho prvku
        else if (pos == size - 1) {
            TItem * aktualni;
            TItem * predchozi;

            aktualni = m_Last;
            predchozi = m_Last->m_Prev;

            delete aktualni;

            m_Last = predchozi;
            m_Last->m_Next = NULL;

            size--;

        }// Odstraneni prvniho prvku
        else if (pos == 0) {
            TItem * aktualni;
            TItem * nasledujici;

            aktualni = m_First;
            nasledujici = m_First->m_Next;

            delete aktualni;

            m_First = nasledujici;
            m_First->m_Prev = NULL;

            size--;

        }// Odstraneni prvku uvnitr
        else {
            TItem * predchozi;
            TItem * aktualni;
            TItem * nasledujici;

            // TODO najed na odpovidajici prvek, tzn. priradit akt., nasl., pred.

            // Pruchod odpredu
            // TODO Pruchod odzadu
            predchozi = NULL;
            aktualni = m_First;
            nasledujici = m_First->m_Next;

            // Dojed na zadanou pozici
            for (int i = 0; i < pos; i++) {
                predchozi = aktualni;
                aktualni = nasledujici;
                nasledujici = nasledujici->m_Next;
            }

            // Dojeli jsme na zadanou pozici, ted muzeme mazat
            delete aktualni;

            predchozi->m_Next = nasledujici;
            nasledujici->m_Prev = predchozi;

            size--;
        }

        return true;
    };

protected:

    struct TItem {
        TItem * m_Next;
        TItem * m_Prev;
        T m_Val;
    };

    /* Členská proměnná m_First označuje počátek spojového seznamu. Začneme-li
       prvkem z m_First a postupujeme-li spojovým seznamem ve směru ukazatelů m_Next,
       pak projdeme celý spojový seznam od začátku do konce. Poslední prvek spojového
       seznamu má ukazatel m_Next nastaven na hodnotu NULL. Pokud je spojový seznam
       prázdný, je m_First nastaven na NULL. */
    TItem * m_First;

    /* Členská proměnná m_Last označuje poslední prvek spojového seznamu.
       Začneme-li prvkem z m_Last a postupujeme-li spojovým seznamem ve směru
       ukazatelů m_Prev, pak projdeme celý spojový seznam od konce do začátku. První
       prvek spojového seznamu má ukazatel m_Prev nastaven na hodnotu NULL. Pokud je
       spojový seznam prázdný, je m_Last nastaven na NULL. */
    TItem * m_Last;

    int size;
};



