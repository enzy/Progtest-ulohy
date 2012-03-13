/* 
 * File:   CFileSystem.h - Adresářový strom
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 17. duben 2010, 16:03
 */

/*
 * Úkolem je realizovat třídu CFileSystem, která bude simulovat funkci OS při
 * práci s adresáři na disku.

 * Instance třídy CFileSystem představuje diskový oddíl. Pomocí rozhraní této
 * třídy můžeme v tomto oddílu pracovat s jeho obsahem. Pro jednoduchost
 * předpokládáme, že máme pouze rozhraní, které vytváří a maže adresáře.
 * Vůbec tedy neuvažujeme obyčejné soubory.
 */

/*
 * Poznámky k zadání:
 * Pro práci s řetězci použijte C API z hlavičkového souboru cstring.
 *
 * Kontrola je velmi detailní, testovací prostředí prochází vygenerovaným
 * stromem a kontroluje správnost odkazů.
 *
 * Před úpravou obsahu (MkDir, RmDir) se ujistěte, že na vstupu je správně
 * zadaná cesta. Pokud je cesta zadaná nesprávně, rovnou vraťte chybu a
 * neprovádějte žádné úpravy obsahu.
 *
 * Pokud neexistuje žádný vytvořený adresář, je nastaveno m_Root na NULL.
 * Pokud je obsah prázdný, bude m_Root roven NULL.
 *
 * Úloha je trochu náročnější, referenční řešení má 270 řádek.
 */

#include <iostream>
#include <cstring>

using namespace std;

class CFileSystem {
public:

    int rootdirCount;

    /*
     * Implicitní konstruktor. Inicializuje instanci třídy na prázdný obsah
     * (tedy instance obsahuje pouze kořenový adresář /, členská proměnná
     * m_Root tedy bude mít hodnotu NULL.
     */
    CFileSystem(void) {
        m_Root = NULL;
        m_Last = NULL;
        rootdirCount = 0;

        // TODO implicitni konstruktor
    };

    /*
     * Kopírující konstruktor. Bude implementován, pokud automaticky vytvořený 
     * kopírující konstruktor nestačí.
     */
    /*CFileSystem(const CFileSystem &zdroj) {
        // TODO kopirujici konstruktor
    };*/

    /*
     *  Operátor =. Bude implementován, pokud automaticky vytvořený operátor = nestačí.
     */
    CFileSystem & operator=(const CFileSystem &kopie) {
        if (this == &kopie) return *this;
        return *this;
        // TODO prirarovaci operator
    };

    /*
     * Destruktor. Bude implementován, pokud automaticky vytvořený destruktor nestačí.
     */
    ~CFileSystem(void) {
        // TODO destruktor
        for (int i = rootdirCount - 1; i >= 0; i--) {
            Delete(i);
        }
    };

    /* Metoda InsEnd vloží nový prvek na konec seznamu (za poslední stávající
       prvek). Úspěch operace je signalizován vracením hodnoty true (k neúspěchu by
       nemělo dojít). */
    bool InsRootEnd(const char * dirName) {
        TDir *novy_prvek;
        TDir *predchozi = NULL;
        novy_prvek = new TDir;
        predchozi = m_Last;
        novy_prvek->m_Prev = predchozi != NULL ? predchozi : NULL;
        novy_prvek->m_Next = NULL;
        novy_prvek->m_Name = (char*) calloc(strlen(dirName) + 1, sizeof (char));
        novy_prvek->m_Name = strcpy(novy_prvek->m_Name, dirName);
        novy_prvek->m_Sub = NULL;
        if (predchozi != NULL) predchozi->m_Next = novy_prvek;
        novy_prvek->m_Prev = predchozi;
        m_Last = novy_prvek;
        if (rootdirCount == 0) m_Root = novy_prvek;
        rootdirCount++;
        return true;
    };

    /* Metoda InsStart vloží nový prvek do seznamu na první pozici (před stávající
       první prvek). Úspěch operace je signalizován vracením hodnoty true (k neúspěchu by nemělo dojít). */
    bool InsRootStart(const char * dirName) {
        TDir *novy_prvek = new TDir;
        TDir *nasledujici = NULL;
        nasledujici = m_Root;
        novy_prvek->m_Prev = NULL;
        novy_prvek->m_Next = nasledujici != NULL ? nasledujici : NULL;
        novy_prvek->m_Name = (char*) calloc(strlen(dirName) + 1, sizeof (char));
        novy_prvek->m_Name = strcpy(novy_prvek->m_Name, dirName);
        novy_prvek->m_Sub = NULL;
        if (nasledujici != NULL) nasledujici->m_Prev = novy_prvek;
        novy_prvek->m_Next = nasledujici;
        m_Root = novy_prvek;
        if (rootdirCount == 0) m_Last = novy_prvek;
        rootdirCount++;
        return true;
    };

    /* Metoda InsAfter vloží za stávající prvek seznamu na pozici pos nový prvek.
       Metoda vrací úspěch (true), pokud se vložení zdařilo a neúspěch (false),
       pokud se vložení nezdařilo. Neúspěchem operace skončí např. pokud je zadán
       index neexistujícího prvku. */
    bool InsRootAfter(int pos, const char * dirName) {
        // Kontrola spatne pozice - mimo rozsah
        if (pos >= rootdirCount || pos < 0) return false;
        // Pri vkladani na konec pouzit odpovidajici metodu
        if (pos == rootdirCount - 1) return InsRootEnd(dirName);
        TDir * predchozi;
        TDir * aktualni;
        TDir * nasledujici;
        // Pruchod odpredu
        predchozi = NULL;
        aktualni = m_Root;
        nasledujici = m_Root->m_Next;
        // Dojed na zadanou pozici
        for (int i = 0; i < pos; i++) {
            predchozi = aktualni;
            aktualni = nasledujici;
            nasledujici = nasledujici->m_Next;
        }
        TDir *novy_prvek = new TDir;
        novy_prvek->m_Prev = aktualni;
        novy_prvek->m_Next = nasledujici;
        novy_prvek->m_Name = (char*) calloc(strlen(dirName) + 1, sizeof (char));
        novy_prvek->m_Name = strcpy(novy_prvek->m_Name, dirName);
        novy_prvek->m_Sub = NULL;
        aktualni->m_Next = novy_prvek;
        nasledujici->m_Prev = novy_prvek;
        rootdirCount++;
        return true;
    };

    /* Metoda Read načte obsah z prvku seznamu na pozici pos a uloží jej do
       druhého (výstupního) parametru. Za neúspěch považováno, pokud je požadované
       načtení neexistujícího prvku (nesprávná hodnota pos). V takovém případě metoda
       vrací false. Úspěšné provedení operace čtení je signalizováno návratovou
       hodnotou true. */
    char * ReadRoot(int pos) const {
        // Kontrola spatne pozice - mimo rozsah
        if (pos >= rootdirCount || pos < 0) return false;
        TDir * aktualni = NULL;
        TDir * nasledujici = NULL;
        // Pruchod odpredu
        aktualni = m_Root;
        // Dojed na zadanou pozici
        for (int i = 0; i < pos; i++) {
            nasledujici = aktualni->m_Next;
            aktualni = nasledujici;
        }
        // Navrat hodnotu        
        char * dirName = NULL;
        int nameLenght = strlen(aktualni->m_Name);
        dirName = (char*) calloc(nameLenght + 1, sizeof (char));

        if (aktualni->m_Name == NULL || nameLenght < 1) return dirName;

        dirName = strcpy(dirName, aktualni->m_Name);
        return dirName;
    };

    /*                 
     * Přetížený operátor << zobrazí obsah disku do zadaného výstupního proudu. 
     * Zobrazen je adresářový strom v semigrafické podobě, podle ukázek níže.
     * Výpis obsahuje vždy jeden adresář na řádek, prvním řádkem výpisu je
     * kořenový adresář /. V ukázkovém běhu je výpis zahrnut do komentáře,
     * první a poslední řadek komentáře obsahující pomlčky není součástí výpisu.
     * V paměti je adresářový strom uložen jako k-ární strom. Každému adresáři 
     * odpovídá jeden uzel typu TDir. Členská promenná m_Name obsahuje odkaz na
     * ASCIIZ řetězec se jménem adresáře. Proměnná m_Sub odkazuje na první
     * podadresář daného adresáře nebo má hodnotu NULL, pokud adresář nemá
     * žádné podadresáře. Proměnná m_Next obsahuje odkaz na další adresář na
     * stejné úrovni. Např. obsah kořenového adresáře získáme tak, že procházíme
     * spojový seznam počínaje m_Root ve směru ukazatelů m_Next.
     * Je požadováno, aby adresáře byly ve spojovém seznamu m_Next uloženy 
     * seřazené vzestupně podle abecedy. Toho nejsnáze docílíte zatřiďováním
     * při vytváření adresářů.
     */
    friend std::ostream & operator<<(std::ostream& os, const CFileSystem& q) {

        // TODO Vypis

        // Rekurzivni volani vypisu v pripade podadresaru?

        os << "/" << endl;
        for (int i = 0; i < q.rootdirCount; i++) {
            char * temp = NULL;
            temp = q.ReadRoot(i);
            os << "+-/" << temp << endl;
            char * retezec = q.VypisPotomky(i);
            os << retezec;
            free(retezec);
            free(temp);
        }

        return os;
    };

    char * VypisPotomky(int pozice) const {
        if (pozice >= rootdirCount || pozice < 0) return false;
        TDir * aktualni = NULL;
        TDir * nasledujici = NULL;
        // Pruchod odpredu
        aktualni = m_Root;
        // Dojed na zadanou pozici
        for (int i = 0; i < pozice; i++) {
            nasledujici = aktualni->m_Next;
            aktualni = nasledujici;
        }

        char * retezec = (char*) calloc(1024, sizeof (char));
        ;

        int uroven = 1;
        while (aktualni != NULL) {
            aktualni = ZiskejAVypisPotomka(aktualni, uroven++, retezec);            
        }
        return retezec;
    };

    /**
     * Metoda MkDir slouží v vytvoření nového adresáře. Má dva parametry -
     * cestu se jménem adresáře a příznak, zda má případně vytvářet chybějící
     * adresáře "po cestě".
     *
     * Jméno adresáře je udáno jako absolutní cesta (první znak tedy musí být
     * lomítko), jména adresářů jsou oddělena lomítky. Ve jménu adresáře mohou
     * být obsažené libovolné znaky anglické abecedy (malá i velká písmena A-Z,
     * a-z) a číslice (0-9). Jiné znaky nejsou povolené. Délka jména adresáře
     * není omezená, musí být nenulová. Malá a velká písmenka se rozlišují.
     *
     * Boolský příznak CreateSubdirs udává, zda se při volání mají vytvářet i
     * mezilehlé adresáře, které jsou v zadané cestě obsažené, ale dosud
     * neexistují. Jestliže je hodnota nastavena na false, metoda ohlásí chybu
     * pokud mezilehlé adresáře neexistují. Pokud je příznak nastaven na true,
     * metoda vytvoří všechny potřebné mezilehlé adresáře. jedná se tedy o
     * analogii přepínače -p UNIXového příkazu mkdir.
     *
     * Metoda oznámí úspěch (true), pokud se jí podařilo adresář vytvořit.
     * Pokud vytváření selže, vrací false. Selhání může být způsobeno nesprávnými
     * znaky ve jménech adresářů, neudáním úplné cesty, pokusem o vytvoření již
     * existujícího adresáře nebo chybějícími mezilehlými adresáři (pokud byl
     * příznak CreateSubdirs=false).
     **/
    bool MkDir(const char * DirName, bool CreateSubdirs) {
        // TODO vytvoreni adresare

        // Kontrola cesty
        int nameLenght = strlen(DirName);
        if (nameLenght < 2 || DirName[0] != '/') return false;

        // TODO kontrola cesty

        // Zjisteni poctu lomitek v ceste
        int slashCount = 0;
        int * slashPositions = (int*) calloc(nameLenght, sizeof (int));
        for (int i = 0; i < nameLenght; i++) {
            if (DirName[i] == '/') {
                slashPositions[slashCount++] = i;
                // Kontrola, zdali nejdou lomitka za sebou
                if (i != 0 && DirName[i - 1] == DirName[i]) {
                    free(slashPositions);
                    return false;
                }
            }
        }

        cout << "Pocet lomitek v:\"" << DirName << "\": " << slashCount << endl;

        // Zjisteni delky nazvu adresaru mezi lomitky
        cout << "Delky nazvu adresaru: ";
        int * dirNameLenghts = (int*) calloc(slashCount, sizeof (int));
        TDir * parent = NULL;
        for (int i = 0; i < slashCount; i++) {
            if (i != slashCount - 1) {
                dirNameLenghts[i] = slashPositions[i + 1] - 1 - slashPositions[i];
            } else {
                if (slashPositions[i] == nameLenght - 1) {
                    // posledni lomitko nechceme pocitat
                    slashCount--;
                    break;
                } else {
                    dirNameLenghts[i] = nameLenght - 1 - slashPositions[i];
                }
            }

            // Ziskani nazvu adresare
            char * subdirName = NULL;
            subdirName = (char*) calloc(dirNameLenghts[i] + 1, sizeof (char));
            subdirName = strncpy(subdirName, DirName + 1 + slashPositions[i], dirNameLenghts[i]);

            cout << "(" << subdirName << ") " << dirNameLenghts[i] << ", ";

            if (i == 0) {
                parent = InsRootEndAndGetPointer(subdirName);
            } else {
                parent = MakeSubDir(parent, subdirName);
            }

            free(subdirName);
        }
        cout << endl;
        free(slashPositions);
        free(dirNameLenghts);

        // Vytvoreni prislusnych adresaru, stromove
        cout << "Pocet adresaru k vytvoreni: " << slashCount << endl;

        /*
         * zkontroluj, jestli adresar neexistuje a pak ho vytvor
         */

        //for (int i = 0; i < slashCount; i++) {}



        return true;

    };

    /**
     * Metoda RmDir slouží k výmazu adresáře případně i s jeho podadresáři.
     * Metoda má rovněž dva parametry - jméno mazaného adresáře a příznak,
     * zda má mazat i případně neprázdné adresáře.
     *
     * Pro udání jména adresáře platí stejná pravidla jako ve volání MkDir.
     * Boolský příznak RemoveNonempty udává, zda má metoda vymazat případné
     * neprázdné adresáře (při true je vymaže) nebo zda má v takové chvíli
     * hlásit chybu (při RemoveNonempty=false).
     *
     * Metoda hlásí úspěch návratovou hodnotou true, neúspěch pak hodnotou
     * false. Příčinou neúspěchu je zadání nesprávné cesty, pokus o výmaz
     * neexistujícího adresáře nebo pokus o výmaz neprázdného adresáře
     * (s RemoveNonempty = false.
     *
     * Pozor - metodu lze zavolat na kořenový adresář.
     * Volání X . RmDir ("/", true ) vymaže celý obsah. 
     **/
    bool RmDir(const char * DirName, bool RemoveNonempty) {
        // smazani poadresare
        return false;
    };

    /* Metoda Delete smaže prvek seznamu na pozici pos. Po úspšném provedení
       operace vrací hodnotu true, při neúspěchu (neexistující prvek) vrací false. */
    bool Delete(int pos) {

        // Kontrola spatne pozice - mimo rozsah
        if (pos >= rootdirCount || rootdirCount < 0 || rootdirCount<=0) return false;

        // Odstraneni jedineho prvku
        if (rootdirCount == 1) {
            TDir * aktualni;
            aktualni = m_Root;

            free(aktualni->m_Name);

            if(aktualni->m_Sub!=NULL){
                free(aktualni->m_Sub->m_Name);
                delete aktualni->m_Sub;
            }

            delete aktualni;

            m_Root = NULL;
            m_Last = NULL;

            rootdirCount = 0;

        }// Odstraneni posledniho prvku
        else if (pos == rootdirCount - 1) {
            TDir * aktualni;
            TDir * predchozi;

            aktualni = m_Last;
            predchozi = m_Last->m_Prev;

            free(aktualni->m_Name);

            if(aktualni->m_Sub!=NULL){
                free(aktualni->m_Sub->m_Name);
                delete aktualni->m_Sub;
            }

            delete aktualni;

            m_Last = predchozi;
            m_Last->m_Next = NULL;

            rootdirCount--;

        }// Odstraneni prvniho prvku
        else if (pos == 0) {
            TDir * aktualni;
            TDir * nasledujici;

            aktualni = m_Root;
            nasledujici = m_Root->m_Next;

            free(aktualni->m_Name);

            if(aktualni->m_Sub!=NULL){
                free(aktualni->m_Sub->m_Name);
                delete aktualni->m_Sub;
            }

            delete aktualni;

            m_Root = nasledujici;
            m_Root->m_Prev = NULL;

            rootdirCount--;

        }// Odstraneni prvku uvnitr
        else {
            TDir * predchozi;
            TDir * aktualni;
            TDir * nasledujici;

            // TODO najed na odpovidajici prvek, tzn. priradit akt., nasl., pred.

            // Pruchod odpredu
            // TODO Pruchod odzadu
            predchozi = NULL;
            aktualni = m_Root;
            nasledujici = m_Root->m_Next;

            // Dojed na zadanou pozici
            for (int i = 0; i < pos; i++) {
                predchozi = aktualni;
                aktualni = nasledujici;
                nasledujici = nasledujici->m_Next;
            }

            // Dojeli jsme na zadanou pozici, ted muzeme mazat
            free(aktualni->m_Name);

            if(aktualni->m_Sub!=NULL){
                free(aktualni->m_Sub->m_Name);
                delete aktualni->m_Sub;
            }

            delete aktualni;

            predchozi->m_Next = nasledujici;
            nasledujici->m_Prev = predchozi;

            rootdirCount--;
        }

        return true;
    };


protected:

    /*
     * Po přidání prvního adresáře (např. "abc") bude m_Root odkazovat na záznam
     * se jménem "abc". Přidáme-li ještě adresář "def", pak m_Root odkazuje na
     * "abc" a m_Root -> m_Next odkazuje na "def".
     */
    struct TDir {
        TDir * m_Next;
        TDir * m_Prev;
        TDir * m_Sub;
        char * m_Name;
        int subdirCount;
    };
    TDir * m_Root; // obdoba m_First
    TDir * m_Last;

    TDir * MakeSubDir(TDir * ParentDir, const char * dirName) {
        ParentDir->m_Sub = new TDir;

        // TODO Vice podadresaru

        ParentDir->m_Sub->m_Name = (char*) calloc(strlen(dirName) + 1, sizeof (char));
        ParentDir->m_Sub->m_Name = strcpy(ParentDir->m_Sub->m_Name, dirName);
        ParentDir->m_Sub->m_Next = NULL;
        ParentDir->m_Sub->m_Prev = NULL;
        ParentDir->m_Sub->m_Sub = NULL;
        ParentDir->m_Sub->subdirCount = 0;
        ParentDir->subdirCount++;

        return ParentDir->m_Sub;
    };

    TDir * InsRootEndAndGetPointer(const char * dirName) {
        TDir *novy_prvek = NULL;
        TDir *predchozi = NULL;

        novy_prvek = new TDir;
        predchozi = m_Last;

        novy_prvek->m_Prev = predchozi != NULL ? predchozi : NULL;
        novy_prvek->m_Next = NULL;
        novy_prvek->m_Name = (char*) calloc(strlen(dirName) + 1, sizeof (char));
        novy_prvek->m_Name = strcpy(novy_prvek->m_Name, dirName);
        novy_prvek->m_Sub = NULL;
        novy_prvek->subdirCount = 0;

        if (predchozi != NULL) predchozi->m_Next = novy_prvek;
        novy_prvek->m_Prev = predchozi;
        m_Last = novy_prvek;

        if (rootdirCount == 0) m_Root = novy_prvek;
        rootdirCount++;

        return novy_prvek;
    };

    TDir * ZiskejAVypisPotomka(TDir * rodic, int uroven, char * stringSubdirs) const {
        if (rodic->m_Sub == NULL) return NULL;

        int delkaNazvu = strlen(rodic->m_Sub->m_Name);
        int delkaRetezce = 4 + uroven + delkaNazvu + 1; // 1+3

        char * retezec = (char*) calloc(delkaRetezce + 1, sizeof (char));

        // Sestav retezec
        retezec[0] = '|';
        for (int i = 0; i < uroven; i++) {
            retezec[i + 1] = ' ';
        }
        retezec[uroven+1]='+';
        retezec[uroven+2]='-';
        retezec[uroven+3]='/';

        for (int i = 0; i < delkaNazvu; i++) {
            retezec[i + 1 + uroven+3] = rodic->m_Sub->m_Name[i];
        }

        retezec[delkaRetezce-1]='\n';

        // Spoj ho s uz existujicim
        strcat(stringSubdirs, retezec);

        free(retezec);

        return rodic->m_Sub;
    };
};



