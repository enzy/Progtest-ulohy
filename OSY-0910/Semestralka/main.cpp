/* 
 * File:   Simulace plánování vláken (OSY - semestrální práce)
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 18. duben 2010, 23:47
 * Deadline: 2.05.2010 23:59:59

 * Úkolem je realizovat program v C/C++, který bude simulovat zjednodušený model
 * plánování vláken. Pro implemetaci vláken použijte POSIXovou knihovnu pthread.
 * Pro synchronizaci použijte vhodné nástroje na principu blokování
 * (mutex, semafor, ...) nikoliv aktivního čekání. Použití aktivního čekání
 * znamená automaticky ztrátu 50 % bodů.

 * Systém umožňuje provádět současně jednu výpočetní a jednu V/V operaci,
 * tzn. že vlákna budou soutěžit o CPU a V/V jednotku. Přidělování CPU je na
 * základě statických priorit 1-10 (maximální priorita je 10), vlákna se stejnou
 * prioritou jsou plánována metodou round robin. Časové kvantum je implicitně
 * nastaveno na 100ms, v programu bude definovana symbolická konstanta, kterou
 * půjde časové kvantum jednoduše změnit. Režie na přepnutí kontextu je zanedbána.
 * Přidělování V/V jednotky je na principu FIFO.

 * Chování vláken je popsáno bloky instrukcí, které jsou aplikaci simulátoru
 * zadané na standardním vstupu (jeden řádek = popis jednoho bloku instrukcí).

 * Formát definice bloku instrukcí:

ID:Priorita:Instrukce1,Instrukce2,Instrukce3,...

Kde:
 * ID je celé číslo jednoznačně identifikující blok instrukcí.
 * Priorita je celé číslo 1-10 udávající prioritu threadu, který bude instrukce provádět.
 * Instrukce udávají vlastní prováděnou činnost. Jsou kódované následovně:
    o cpu - bude prováděn výpočet po zadaný počet časových kvant.
    o io - budou prováděné V/V operace po zadaný počet časových kvant.
    o create - bude vytvořen nový thread, thread bude vykonávat posloupnost instrukcí zadaného čísla.
    o sleep - thread bude nečinný po zadaný počet časových kvant.

Příklad vstupních dat:

1:5:cpu 6,io-12,cpu-10,io-27,create-2,cpu-1,create-3,create-3
2:6:cpu-50,create-4,cpu-50,sleep-4,cpu-30
3:5:cpu-1,io-70
4:5:cpu-11,io-8,cpu-20,create-3

 * Jednotlivá vlákna budou implementována pomocí POSIXových vláken (1:1),
 * simulátor načte vstupní data, vytvoří první vlákno a nechá jej provádět blok
 * instrukcí s nejnižším ID. Provádění tohoto vlákna případně vytvoří další vláka,
 * která budou provádět definované bloky instrukcí. Thread končí (a je korektně
 * uvolněn) jakmile dokončí poslední instrukci z bloku instrukcí, který měl
 * přiřazen. Simulace se ukončí poté, kdy v systému nezůstane žádné pracující
 * vlákno.

 * Každému vytvořenému vláknu bude přiřazen jednoznačný celočíselný identifikátor.
 * Každé vlákno bude na standardním výstupu zobrazovat informace o tom, co provádí:

 * Pokud zahájí výpočet (instrukce cpu):

        Vlakno <ID_vlakna> [<cislo_bloku_instrukci>] zahajuje cpu na dobu <pocet_kvant>

 * Pokud ukončí výpočet (dokončí instrukci cpu):

        Vlakno <ID_vlakna> [<cislo_bloku_instrukci>] ukoncilo cpu.

 * Pokud zahájí V/V operaci:

        Vlakno <ID_vlakna> [<cislo_bloku_instrukci>] zahajuje V/V na dobu <pocet_kvant>.

 * Pokud ukončí V/V operaci:

        Vlakno <ID_vlakna> [<cislo_bloku_instrukci>] ukoncilo V/V.

 * Pokud zahájí spánek (sleep):

        Vlakno <ID_vlakna> [<cislo_bloku_instrukci>] zahajuje sleep na dobu <pocet_kvant>

 * Pokud je probuzeno (skončí sleep):

        Vlakno <ID_vlakna> [<cislo_bloku_instrukci>] ukoncilo sleep.

 * Pokud vytvoří nový thread (sokončí sleep):

        Vlakno <ID_vlakna> [<cislo_bloku_instrukci>] vytvorilo novy thread <cislo_noveho_threadu> [<cislo_bloku_instrukci_noveho_threadu>].

 * Pokud se ukončí:

        Vlakno <ID_vlakna> [<cislo_bloku_instrukci>] dobehlo.

 * Uvedené výpisy jsou pouze vzory, údaje v lomených závorkách budou
 * pochopitelne nahrazené relevantními informacemi.

 * Odevzdávejte archiv (.zip, .tgz), který obsahuje všechny náležitosti
 * (zdrojové kódy, Makefile, zprávu popisující Vaše řešeni a případná
 * testovací data). Úloha nebude kontrolovaná strojově, ale cvičícími.

 */

#include "Vlakno.h"

int main(int argc, char** argv) {

    bool debug = false;

    if (debug) {
        // Testovaci data
        istringstream is("1:5:cpu 6,io-12,cpu-10,io-27,create-2,cpu-1,create-3,create-3\n2:6:cpu-50,create-4,cpu-50,sleep-4,cpu-30\n3:5:cpu-1,io-70\n4:5:cpu-11,io-8,cpu-20,create-3");

        // Nacitani vstupnich dat, dokud na vstupu neni prazdny radek
        while (!is.eof()) {
            string tmp;
            getline(is, tmp);
            if (tmp.size() < 2) break; // pri prazdnem radku ukonci nacitani
            inputInstructionDefenitionBlocks.push_back(tmp);
        }
    } else {
        // Nacitani vstupnich dat, dokud na vstupu neni prazdny radek
        while (!cin.eof()) {
            string tmp;
            getline(cin, tmp);
            if (tmp.size() < 2) break; // pri prazdnem radku ukonci nacitani
            inputInstructionDefenitionBlocks.push_back(tmp);
        }
    }

    // Fronta s jednotlivymi vlakny
    list<Vlakno*> frontaVlaken;
    // Ukazatel(iterator) na vlakno
    list<Vlakno*>::iterator it2;
    // Ukazatel(iterator) na radek vstupu
    list<string>::iterator it;

    // Vytvorit vsechny vlakna, postupne po radcich
    for (it = inputInstructionDefenitionBlocks.begin(); it != inputInstructionDefenitionBlocks.end(); it++) {
        frontaVlaken.push_back(new Vlakno(*it));
    }

    // Spusteni a stridani vlaken
    // Spust kazde vlakno s pocty operaci, tedy kvantem, vynasobenym prioritou a pote spust dalsi

    Vlakno * aktualni = frontaVlaken.front();
    while (frontaVlaken.size()) {
        // Stridej vsechny vlakna po sobe
        for (it2 = frontaVlaken.begin(); it2 != frontaVlaken.end(); it2++) {
            aktualni = *it2;
            // Podle priority vykonej odpovidajici pocet instrukci - cyklu
            for (int i = 0; i < aktualni->priority; i++) {
                if (aktualni->hasWork()) {
                    aktualni->doWork();
                } else {
                    // Pokud vlakno uz nema co napraci, vyradime ho z fronty
                    it2 = frontaVlaken.erase(it2);
                    break;
                }
            }
        }
    }

    // Vsechna vlaka provedla svoji praci, nyni muzeme koncit


    // Vypis statistky
    cout << "\nStatistics:\nCPU iterations: " << cpuIterations << endl;
    cout << "IO iterations: " << ioIterations << endl;
    cout << "Create iterations: " << createIterations << endl;
    cout << "Sleep iterations: " << sleepIterations << endl << endl;

    // Ukoncit program
    return (EXIT_SUCCESS);
}

