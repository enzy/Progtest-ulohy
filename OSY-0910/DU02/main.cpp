/* 
 * File:   main.cpp - Zrychlení výpočtu paralelně běžícícími procesy
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 9. květen 2010, 10:03
 */

#ifndef __PROGTEST__

#include <cstdlib>
#include <iostream>
#include <math.h>
using namespace std;

/* 2D souradnice */
struct TPoint {
    double X, Y;
};

/* jeden pozadavek na vypocet nejblizsi dvojice bodu */
struct TProblem {
    int PointsNr; /* pocet bodu v uloze */
    struct TPoint * Points; /* pole souradnic jednotlivych bodu */
    double MinDist; /* vypocteny vysledek - vzdalenost nejblizsi dvojice bodu */
};

#define MAX_POINTS 2000

#endif /* __PROGTEST__ */

#include <sys/wait.h>

typedef struct TProblem * (*generator_t) (void);
typedef void (*validator_t)(struct TProblem*);

/*
 * Funkce ForkingSolver spustí vlastní paralelní výpočet. Je zodpovědná za vytvoření
 * procesů, synchronizaci, ukončení procesů a za uvolnění alokovaných prostředků.
 *
 * @param generators je pole ukazatelu na funkce generatoru.
 * Pole obsahuje genProcNr položek, každá obsahuje ukazatel (adresu) funkce jednoho generátoru.\n\n
 * Generátor je funkce, která po vyvolání vrátí jako návratovou adresu jeden
 * problém k vyřešení. Návratová hodnota generátoru je ukazatel na kompletně
 * připravenou strukturu TProblem, struktura obsahuje vyplněné souřadnice bodů.
 * Ve struktuře není vyplněna složka MinDist.\n\n
 * Vámi implementovaná funkce ForkingSolver vytvoří pro každý takový generátor
 * proces, v tomto procesu bude cyklicky volat příslušnou funkci generátoru.
 * Požadavky na výpočty bude předávat výpočetním procesům (viz níže).
 * Pokud funkce generátoru vrátí hodnotu NULL, znamená to, že generátor již nemá
 * žádné další požadavky na výpočet. Vaše implementace je zodpovědná a to,
 * že se po navrácení hodnoty NULL z generátoru ukončí proces příslušného
 * generátoru.
 *
 * @param genProcNr pocet polozek pole generators, každá obsahuje ukazatel (adresu) funkce jednoho generátoru.
 *
 * @param validator je ukazatel na funkci, které mají být předané vyřešené problémy.
 * Funkce validátoru převezme vypočtená data, zkontroluje je a dále je zpracuje.
 * POZOR - na rozdíl od verze se thready se se validátor nestará o uvolnění paměti,
 * která byla alokovaná pro problém, to je vaše povinnost.
 *
 * @param solverProcNr udává, kolik má být vytvořeno procesů pro vlastní výpočty.
 * Vámi implementovaná funkce ForkingSolver je zodpovědná za vytvoření těchto
 * procesů, za jejich rovnoměrné využití pro výpočty, za jejich vzájemnou
 * synchronizaci a za jejich ukončení po vypočtení všech zadání z generátorů.
 *
 * @return 1 ok, 0 fail
 *
 */

int debug = false;
int ForkingSolver(struct TProblem * (**generators) (void), int genProcNr, void (*validator) (struct TProblem *), int solverProcNr);
double getVzdalenost(struct TPoint * Bod1, struct TPoint * Bod2);
void ResProblem(TProblem * problemKreseni);
void ZpracovavejGenerator(generator_t gen, validator_t val, int &runningSolvers, int solverProcNr);

/* --- */

int ForkingSolver(
        struct TProblem * (**generators) (void),
        int genProcNr,
        void (*validator) (struct TProblem *),
        int solverProcNr) {

    // pro kazdy generator vlastni proces, v procesu cyklicke volani generatoru    
    int generatorPID;
    int runningGenerators = 0;
    int runningSolvers = 0;

    for (int i = 0; i < genProcNr; i++) {
        runningGenerators++;
        /*if (runningGenerators + runningSolvers >= solverProcNr) {
            int toWait = runningGenerators + runningSolvers;
            for (int j = 0; j < toWait; j++) {
                wait(NULL);
            }
        }*/
        generatorPID = fork();
        if (generatorPID == -1) exit(EXIT_FAILURE);
        if (generatorPID == 0) {
            ZpracovavejGenerator(generators[i], validator, runningSolvers, solverProcNr);
            runningGenerators--;
            exit(EXIT_SUCCESS);
        }
    }

    // Pockej na dokonceni vsech procesu
    int pid = wait(NULL);
    while (pid > 0) {
        pid = wait(NULL);
    }

    //cout << "Konec" << endl;

    return 1;
}

void ZpracovavejGenerator(generator_t gen, validator_t val, int &runningSolvers, int solverProcNr) {
    TProblem * problem;
    int solverPID;
    int count = 0;
    while ((problem = gen())) {
        runningSolvers++;
        count++;
        if (runningSolvers >= solverProcNr) {
            for (int i = 0; i < runningSolvers; i++) {
                wait(NULL);
            }
            runningSolvers -= solverProcNr;
        }
        solverPID = fork();
        if (solverPID == -1) exit(EXIT_FAILURE);
        if (solverPID == 0) {
            ResProblem(problem);
            val(problem);
            if (problem != NULL && problem->Points != NULL) free(problem->Points);
            if (problem != NULL) free(problem);
            runningSolvers--;
            exit(EXIT_SUCCESS);
        }
        if (problem != NULL && problem->Points != NULL) free(problem->Points);
        if (problem != NULL) free(problem);
    }
    for (int i = 0; i < count; i++) {
        wait(NULL);
    }
    if (problem != NULL && problem->Points != NULL) free(problem->Points);
    if (problem != NULL) free(problem);
}

/** 
 * @param Bod1
 * @param Bod2
 * @return Vzdalenost mezi dvema body
 */
double getVzdalenost(struct TPoint * Bod1, struct TPoint * Bod2) {
    return sqrt(
            (Bod2->X - Bod1->X)*(Bod2->X - Bod1->X)
            +
            (Bod2->Y - Bod1->Y)*(Bod2->Y - Bod1->Y)
            );
}

/**
 * @param problemKreseni 
 */
void ResProblem(TProblem * problemKreseni) {
    // projit vsechny kombinace body a zjistit nejmensi vzdalenost
    int pocetBodu = problemKreseni->PointsNr - 1;
    int pocetBodu2 = problemKreseni->PointsNr;
    // ziskani prvni vzdalenosti
    double minVzdalenost = getVzdalenost(problemKreseni->Points, problemKreseni->Points + 1);
    // projeti vsech moznych kombinaci
    for (int i = 0; i < pocetBodu; i++) {
        for (int j = i + 1; j < pocetBodu2; j++) {
            double vzdalenost = getVzdalenost(problemKreseni->Points + i, problemKreseni->Points + j);
            if (vzdalenost < minVzdalenost) minVzdalenost = vzdalenost;
        }
    }
    // ulozeni vysledku
    problemKreseni->MinDist = minVzdalenost;

    // TODO oznameni ukonceni
}



#ifndef __PROGTEST__

struct TProblem * Generator(void) { /* ukazkovy generator - pro ucely testovani */
    struct TProblem * X;
    int i;
    static int Cnt = 10;

    if (Cnt == 0) return (NULL);
    Cnt--;

    X = (struct TProblem *) malloc(sizeof (*X));
    X -> PointsNr = MAX_POINTS;
    X -> Points = (struct TPoint *) malloc(X -> PointsNr * sizeof (*X -> Points));
    X -> MinDist = 0;

    for (i = 0; i < X -> PointsNr; i++) {
        X -> Points[i] . X = 1000000.0 * rand() / RAND_MAX;
        X -> Points[i] . Y = 1000000.0 * rand() / RAND_MAX;
    }
    return (X);
}

struct TProblem * DummyGenerator(void) {
    return (NULL);
}

void DummyValidator(struct TProblem * X) { /* ukazkovy validator - pro ucely testovani */
    cout << "PID: " << getpid() << " | PPID: " << getppid() << " | Min distance = " << X -> MinDist << endl;
    //free(X -> Points);
    //free(X);
}

int main(int argc, char * argv []) {
    struct TProblem * (*generators[12]) (void) = {Generator, DummyGenerator,
        Generator, DummyGenerator, Generator, Generator, DummyGenerator, Generator,
        DummyGenerator, Generator};

    ForkingSolver(generators, 12, DummyValidator, 12);


    return (EXIT_SUCCESS);
}
#endif /* __PROGTEST__ */


