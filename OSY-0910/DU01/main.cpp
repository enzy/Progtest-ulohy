/*
 * File:   main.cpp - Zrychlení výpočtu více vlákny
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 11. duben 2010, 2:00
 */

#ifndef __PROGTEST__

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/unistd.h>

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
#endif /* __PROGTEST__ */

pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex2 = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

typedef void (*validator_t)(struct TProblem*);

typedef void* (*pthreadFunkce)(void*);

typedef struct TProblem * (*generator_t) (void);

typedef struct {
    generator_t gen;
    validator_t val;
} GenAVal;

typedef void (*solver_t)(GenAVal*);

typedef struct frontaItem FrontaItem;

struct frontaItem {
    generator_t gen;
    //GenAVal arg;
    FrontaItem* dalsi;
    validator_t val;
    bool validace;
    TProblem *problem;
    //pthreadFunkce ptFunkce;
};

struct ReseniKValidaci {
    validator_t val;
    struct TProblem prob;
};

FrontaItem * ZacatekFronty; // odtud se bere
FrontaItem * KonecFronty; // sem se vklada blbecku!

int PocetPolozekFronty;
int PocetDokoncenychGeneratoru;

bool BeziciVypocet = false;

/*
 */


void VlozDoFronty(generator_t gen, validator_t val) {
    FrontaItem * novyPrvek = new FrontaItem;

    //novyPrvek->ptFunkce = fce;
    novyPrvek->gen = gen;
    novyPrvek->val = val;
    novyPrvek->dalsi = 0;
    novyPrvek->validace = false;

    // zacatek krize
    pthread_mutex_lock(&mutex);

    if (!ZacatekFronty) {
        KonecFronty = ZacatekFronty = novyPrvek;
    } else {
        KonecFronty->dalsi = novyPrvek;
        KonecFronty = novyPrvek;
    }
    PocetPolozekFronty++;


    pthread_mutex_unlock(&mutex);
    // konec krize

    // notifikuj vlakna, ze byl pridany novy prvek

}

void VlozValidaciReseniDoFronty(GenAVal * genval, TProblem * problem) {
    FrontaItem * novyPrvek = new FrontaItem;

    novyPrvek->val = genval->val;
    novyPrvek->validace = true;
    //novyPrvek->arg = problem;
    novyPrvek->problem = problem;
    novyPrvek->dalsi = 0;

    // zacatek krize
    pthread_mutex_lock(&mutex);

    if (!ZacatekFronty) {
        KonecFronty = ZacatekFronty = novyPrvek;
    } else {
        KonecFronty->dalsi = novyPrvek;
        KonecFronty = novyPrvek;
    }
    PocetPolozekFronty++;


    pthread_mutex_unlock(&mutex);
}

FrontaItem * VyberFrontu() {
    FrontaItem * ctenyPrvek = NULL;
    // je treba dat lock
    pthread_mutex_lock(&mutex);
    if (ZacatekFronty) {
        ctenyPrvek = ZacatekFronty;
        ZacatekFronty = ZacatekFronty->dalsi;

        PocetPolozekFronty--;

        if (ZacatekFronty == NULL) {
            KonecFronty = NULL;
        }
    }
    pthread_mutex_unlock(&mutex);

    return ctenyPrvek;
}

double getVzdalenost(struct TPoint * Bod1, struct TPoint * Bod2) {
    return sqrt(
            (Bod2->X - Bod1->X)*(Bod2->X - Bod1->X)
            +
            (Bod2->Y - Bod1->Y)*(Bod2->Y - Bod1->Y)
            );
}

void ResProblem(GenAVal * genAval) {
    TProblem * problem;

    double minVzdalenost;

    while ((problem = genAval->gen())) {
        // projit vsechny kombinace body a zjistit nejmensi vzdalenost

        int pocetBodu = problem->PointsNr - 1;
        int pocetBodu2 = problem->PointsNr;

        minVzdalenost = getVzdalenost(problem->Points, problem->Points + 1);

        for (int i = 0; i < pocetBodu; i++) {
            for (int j = i + 1; j < pocetBodu2; j++) {

                double vzdalenost = getVzdalenost(problem->Points + i, problem->Points + j);

                if (vzdalenost < minVzdalenost) minVzdalenost = vzdalenost;
            }
        }

        //printf("Vypoctena vzdalenost: %g\n", minVzdalenost);

        problem->MinDist = minVzdalenost;

        //VlozDoFronty((pthreadFunkce) genAval->val, problem);
        VlozValidaciReseniDoFronty(genAval, problem);
        //genAval->val(problem);

    }
    // notifikovat frontu, ze byla dokoncena uloha
}

void VyzobavejFrontu(void) {

    while (BeziciVypocet) {
        // zacatek krize        
        FrontaItem * fi = VyberFrontu();
        // konec krize

        if (!fi) {

            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
            continue;
        }
        //fi->ptFunkce(fi->arg);

        if (!fi->validace) {
            GenAVal* gav = new GenAVal;
            gav->gen = fi->gen;
            gav->val = fi->val;
            ResProblem(gav);
            delete gav;

        } else {
            fi->val(fi->problem);
        }
        pthread_mutex_lock(&mutex);
        PocetDokoncenychGeneratoru++;
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&mutex);



        delete fi;

    }
}

void SmazFrontu(void) {
    pthread_mutex_lock(&mutex);
    while (FrontaItem * fi = VyberFrontu()) {
        //        delete fi->arg;
        delete fi;
    }
    ZacatekFronty = KonecFronty = NULL;
    PocetPolozekFronty = 0;
    pthread_mutex_unlock(&mutex);
}

// void VlozDoFrontyValidator(ReseniKValidaci * reseni) {}

/*
Funkce ThreadedSolver spustí vlastní výpočet. Je zodpovědná za vytvoření vláken,
synchronizaci, ukončení vláken a za uvolnění alokovaných prostředků.
Parametry funkce mají následující význam:

 * generators je pole ukazatelu na funkce generatoru. Pole obsahuje genThrNr
   položek, každá obsahuje ukazatel (adresu) funkce jednoho generátoru.

   Generátor je funkce, která po vyvolání vrátí jako návratovou adresu jeden
   problém k vyřešení. Návratová hodnota generátoru je ukazatel na kompletně
   připravenou strukturu TProblem, struktura obsahuje vyplněné souřadnice bodů.
   Ve struktuře není vyplněna složka MinDist.

   Vámi implementovaná funkce ThreadedSolver vytvoří pro každý takový generátor
   vlákno, v tomto vláknu bude cyklicky volat příslušnou funkci generátoru.
   Požadavky na výpočty bude předávat výpočetním vláknům (viz níže). Pokud funkce
   generátoru vrátí hodnotu NULL, znamená to, že generátor již nemá žádné další
   požadavky na výpočet. Vaše implementace je zodpovědná a to, že se po navrácení
   hodnoty NULL z generátoru ukončí vlákno příslušného generátoru.

 * validator je ukazatel na funkci, které mají být předané vyřešené problémy.
   Funkce validátoru převezme vypočtená data, zkontroluje je a dále je zpracuje.
   Dále se validátor stará o uvolnění paměti, která byla alokovaná pro souřadnice
   (Points).

 * solverThrNr udává, kolik má být vytvořeno vláken pro vlastní výpočty. Vámi
   implementovaná funkce ThreadedSolver je zodpovědná za vytvoření těchto vláken,
   za jejich rovnoměrné využití pro výpočty, za jejich vzájemnou synchronizaci
   a za jejich ukončení po vypočtení všech zadání z generátorů.
 */

/* funkce realizujici paralelni vypocet */
int ThreadedSolver(generator_t * generators, int pocetGeneratoru,
        validator_t validator, int pocetVlaken) {

    // inicializace synchronizacnich prostedku

    pthread_attr_t pattr;
    pthread_attr_init(&pattr);
    pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE);

    // generators[0]() vrati TProblem

    // inicializace fronty

    pthread_t * vlakna = new pthread_t[pocetVlaken];
    if (!vlakna) return 0;

    // Vkladani do fronty

    ZacatekFronty = KonecFronty = 0;

    for (int i = 0; i < pocetGeneratoru; i++) {

        VlozDoFronty(generators[i], validator);
    }

    // spusteni potrebneho poctu vlaken „typu“ G (jako parametr maji generujici fci napr. Generator, DummyGenerator atd.)

    // spusteni potrebneho poctu vlaken „typu“ V (jako parametr maji overovaci fci napr. DummyValidator atd.)

    BeziciVypocet = true;

    for (int i = 0; i < pocetVlaken; i++) {
        if (pthread_create(vlakna + i, &pattr, (void*(*)(void*))VyzobavejFrontu, NULL)) {
            SmazFrontu();
            delete [] vlakna;
            return 0;
        }
    }

    // cekani na dokonceni vlaken G a V

    while (1)/*(PocetDokoncenychGeneratoru < pocetGeneratoru * 2)*/ {
        pthread_mutex_lock(&mutex);
        if (PocetPolozekFronty < 1) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
        pthread_mutex_lock(&mutex2);
        pthread_cond_wait(&cond2, &mutex2);
        pthread_mutex_unlock(&mutex2);
    }

    BeziciVypocet = false;

    pthread_cond_broadcast(&cond);

    for (int i = 0; i < pocetVlaken; i++) {
        pthread_join(vlakna[i], NULL);
    }

    SmazFrontu();
    delete [] vlakna;
    pthread_attr_destroy(&pattr);


    return (1); // 1=ok, 0=fail
}


#ifndef __PROGTEST__

struct TProblem * Generator(void) { /* ukazkovy generator - pro ucely testovani */
    struct TProblem * X;
    int i;
    static int Cnt = 10;

    if (Cnt == 0) return ( NULL);
    Cnt--;

    X = (struct TProblem *) malloc(sizeof ( *X));
    X -> PointsNr = 30;
    X -> Points = (struct TPoint *) malloc(X -> PointsNr * sizeof ( *X -> Points));
    X -> MinDist = 0;

    for (i = 0; i < X -> PointsNr; i++) {
        X -> Points[i] . X = 1000000.0 * rand() / RAND_MAX;
        X -> Points[i] . Y = 1000000.0 * rand() / RAND_MAX;
    }
    return ( X);
}

struct TProblem * DummyGenerator(void) {
    return ( NULL);
}

void DummyValidator(struct TProblem * X) { /* ukazkovy validator - pro ucely testovani */
    printf("Min distance = %f\n", X -> MinDist);
    free(X -> Points);
    free(X);
}

int main(int argc, char * argv []) {
    struct TProblem * (*generators[3]) (void) = {Generator, DummyGenerator, DummyGenerator};

    ThreadedSolver(generators, 3, DummyValidator, 4);


    return (0);
}
#endif /* __PROGTEST__ */

/*
Poznámky:
 * Pro výpočet nejbližší dvojice bodů použijte kvadratický algoritmus.

 * Pro rozdělení práce mezi výpočetní vlákna použijte upravený algoritmus producent-konzument.

 * Musíte správně vyřešit synchronizaci vláken při ukončování (kdy končí poslední generátor).

 * Za chybu bude považováno, pokud se "ztratí" některá zadání. Dejte pozor zejména při ukončování.

 * Přestože jeden generátor skončí, mohou ostatní ještě generovat další data.

 * Kontroluje se doba výpočtu problému pro různý počet výpočetních vláken a poměr
 * času využití CPU a reálného času. Je proto potřeba správně přidělovat práci
 * výpočetním vláknům. Pokud má výpočetní vlákno volno a je co počítat, pak musí
 * dostat přidělenu práci.

 * Použijte blokující čekání, vyhněte se aktivnímu čekání. Pokud využijete aktivní
 * čekání, snadno překročíte maximální časový limit.

 * Dejte pozor na inicializaci globálních proměnných. Testování spustí funkci
 * ThreadedSolver několikrát. Pokud ve Vaší implementaci použijete globální proměnné,
 * nespoléhejte se na to, že jsou vyplněné nulami či správně inicializované.
 * (Funkce generátoru v ukázce např. půjde použít také pouze jednou.)

 * Pokud dostanete varování podobné:


       warning: ISO C++ forbids casting between pointer-to-function and pointer-to-object

      bude nejspíše problém s předáváním ukazatele na funkci (např. generátoru) threadu. Místo:

        ....
          pthread_create ( &thr[i], &attr, genProc, (void *) generators[i] )
        ....

      použijte pro přetypování trik s union:

       ....
        union
         {
           void * voidPtr;
           struct TProblem * ( *funcPtr ) ( void );
         }  tricky;

        ....
         tricky . funcPtr = generators[i];
         pthread_create ( &thr[i], &attr, genProc, tricky . voidPtr )
        ....


 * Výše uvedené řešení není ideální, za určitých podmínek by nemuselo fungovat
 * velmi nestandardní platformy). Zcela korektní řešení by se nepokoušelo převádět
 * ukazatel-na-funkci na ukazatel-na-data, například tím, že by předávalo ukazatel
 * na ukazatel na funkci.
 */
