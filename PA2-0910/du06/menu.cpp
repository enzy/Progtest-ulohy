/*
 * File:   Menu
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 25. duben 2010, 15:53
 *
 * Úkolem je navrhnout a implementovat třídy Menu, Polozka  a Submenu
 * pro realizaci víceúrovňové nabídky (vizte přiložený diagram tříd).
 *
 */

#include <iostream>
#include <string>
#include <typeinfo>


using namespace std;

class Polozka {
public:
    int id;
    string jmeno;
    Polozka * dalsi;

    Polozka() {
        id = 0;
        jmeno = "";
        dalsi = NULL;
        cout << " + prazdna" << endl;
    };

    Polozka(int nid, string njmeno) {
        id = nid;
        jmeno.assign(njmeno);
        dalsi = NULL;
        cout << " + samotna " << njmeno << endl;
    };

    Polozka(int nid, string njmeno, Polozka * ndalsi) {
        id = nid;
        jmeno.assign(njmeno);
        dalsi = ndalsi;
        cout << " + " << njmeno << " -> " << ndalsi->jmeno << endl;
    };    
};

class Submenu : public Polozka {
public:
    Polozka * submenu;

    Submenu() {        
        id = 0;
        jmeno = "";
        dalsi = NULL;
        submenu = NULL;
        cout << "+ submenu prazdne" << endl;
    };

    Submenu(int nid, string njmeno) {
        id = nid;
        jmeno.assign(njmeno);
        dalsi = NULL;
        submenu = NULL;
        cout << " + subsamotny " << njmeno << endl;
    };

    Submenu(int nid, string njmeno, Polozka * nsubmen, Polozka * ndalsi) {
        id = nid;
        jmeno.assign(njmeno);
        dalsi = ndalsi;
        submenu = nsubmen;
        cout << " + submenu " << njmeno << " -> " << ndalsi->jmeno << endl;
    };
};

class Menu {
public:
    Polozka * polozky;
    int pocetPolozek;

    /*
     * Konstruktor, který vytvoří menu a připadně do něj vloží zadané položky.
     */
    Menu() {
        polozky = new Polozka();
        pocetPolozek = 0;
    };

    Menu(Polozka * novaPolozka) {
        pocetPolozek = 0;
        polozky = novaPolozka;
        // projdi a prepocitej polozky
        Polozka * aktualni = polozky;

        while (aktualni) {
            pocetPolozek++;
            aktualni = aktualni->dalsi;
        }

        cout << "Vytvoreno menu s " << pocetPolozek << " polozkami" << endl;
    };

    /*
     * Destruktor, kterým se zruší všechny položky menu.
     */
    //~Menu() {}

    /*
     * Metodu vlozZa, která za danou položku (specifikovanou pomocí id,
     * což je první parametr metody) vloží novou položku (druhý parametr).
     * Návratová hodnota (typu bool) indikuje úspěch či neúspěch vložení
     * (true pro úspěch). Za neúspěch se považuje, i pokud by se vložením
     * porušila jednoznačnost id v menu. V takovém případě se samozřejmě nic
     * nevloží. Důležité upřesnění: pokud se položku nepodaří vložit,
     * neuvolňujte její paměť! Předpokládejte, že bude uvolněna volající stranou.
     */
    bool vlozZa(int pos, Polozka * npol) {
        pocetPolozek++;

        Polozka * aktualni = polozky;

        // dojed na konec
        while (aktualni->dalsi) {
            aktualni = aktualni->dalsi;
        }

        aktualni->dalsi = npol;

        cout << "Pridana polozka za pozici " << pos << " s id " << npol->id << ", " << npol->jmeno << endl;

        // najdi polozku;
        return false;
    }

    /*
     * Metodu vlozDo, která do submenu (specifikovaného pomocí id, což je první
     * parametr metody) vloží na první pozici novou položku (druhý parametr).
     * Návratová hodnota (typu bool) indikuje úspěch či neúspěch vložení.
     * Za neúspěch se považuje, i pokud by se vložením porušila jednoznačnost
     * id v menu. Za neúspěch se rovněž považuje, pokud položka s daným id není
     * submenu. Důležité upřesnění: pokud se položku nepodaří vložit,
     * neuvolňujte její paměť! Předpokládejte, že bude uvolněna volající stranou.
     */
    bool vlozDo(int pos, Polozka * npol) {
        pocetPolozek++;

        // najdi polozku

        // je to submenu?

        // vloz polozku
        return false;
    };

    /*
     * Metodu odeber, která odebere položku, specifikovanou pomocí id.
     * Návratová hodnota (typu bool) indikuje úspěch či neúspěch odebrání
     * (true - položka z menu úspěšně odebrána). V případě, že položkou je
     * submenu, odebere se i veškerý jeho obsah.
     */
    bool odeber(int rmid) {
        pocetPolozek--;
        return false;
    };

    /*
     * Metodu najdi, která vyhledá položku menu podle zadaného id
     * (první parametr). Návratová hodnota indikuje úspěch či neúspěch hledání.
     * Metoda vrátí celou cestu k položce (prostřednictvím druhého, výstupního
     * parametru), tj. posloupnost jmen submenu, ve kterých je položka vnořena,
     * oddělených lomítkem.
     */
    bool najdi(int fnid, string & cesta) const {
        return false;
    };

    /*
     * Přetížený operátor výstupu do streamu, kterým se menu vypíše tak,
     * že každá položka je na samostatném řádku, vnořené menu (tj. submenu)
     * je o tři mezery odsazené oproti nadřazenému menu. Položky na nejvyšší
     * úrovni menu mají nulové odsazení (z HTML výpisu to není zcela zřejmé).
     * Za každou položkou menu (= řádkou výpisu) je odřádkování (endl).
     * I za poslední řádkou.
     */
    friend std::ostream & operator<<(std::ostream & os, const Menu & menu) {
        Polozka * aktualni = menu.polozky;

        while (aktualni) {
            os << aktualni->jmeno << "(" << typeid(aktualni).name() << ")" << endl;
            if(typeid(aktualni)==typeid(Submenu)){
                os << "Prochazim submenu " << aktualni->jmeno << endl;
                os << menu.projdiSubmenu(os,(Submenu*)aktualni, 1);
            }            
            aktualni = aktualni->dalsi;
        }

        return os;
    };

    ostream & projdiSubmenu(ostream & os, Submenu * submenu, int level) const{

        Polozka * aktualni  = submenu->submenu;        

        while(aktualni){
            for (int i = 0; i < level*3; i++) {
                os << ' ';
            }

            os << aktualni->jmeno << endl;

            if(typeid(aktualni)==typeid(Submenu)){
                os << projdiSubmenu(os, (Submenu*)aktualni, level+1);
            }

            aktualni = aktualni->dalsi;

        }

        return os;
    }
};




