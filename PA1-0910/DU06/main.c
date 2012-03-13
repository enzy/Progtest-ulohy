/* 
 * File:   main.c - Deleni koristi mezi 3 loupezniky
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 22. listopad 2009, 0:10
 */

#include <stdio.h>
#include <stdlib.h>

#define boolean char
#define true 1
#define false 0

#define MAX_PRVKU 200
#define POCET_MIST 3 /* pocet mist + 1 mezera pro string s poradim prvku */

int* predmety;
int pocet_predmetu;
int celkova_cena;
int podil;
int pocet_spravnych_kombinaci;
int** spravne_kombinace;
int* pouzita_kombinace;
boolean dostatek_kombinaci;

void print_error(void);
void print_fairly(void);
void print_notfairly(void);
int char_int(char znak);
boolean is_cislo(char znak);
boolean testuj_kombinaci(char* retezec, int delka);
void projdi_kombinace(int parent, int deep, int n, char* cesta);
boolean existuji_3_kombinace_spolecne(void);

int main(void) {
    int i, incheck;
    /* Nacteni poctu predmetu */
    printf("Zadejte pocet predmetu:\n");
    incheck = scanf("%d", &pocet_predmetu);
    if (incheck != 1 || pocet_predmetu < 3) {
        print_error();
        return (EXIT_FAILURE);
    }
    /* Nacteni jednotlivych predmetu */
    predmety = (int*) calloc(pocet_predmetu, sizeof (int));    
    if (predmety == NULL) {
        print_error();
        return (EXIT_FAILURE);
    }
    printf("Zadejte cenu jednotlivych predmetu:\n");
    for (i = 0; i < pocet_predmetu; i++) {
        incheck = 0;
        incheck = scanf("%d", &predmety[i]);
        if (incheck != 1 || predmety[i] < 1) {
            print_error();
            return (EXIT_FAILURE);
        }        
        celkova_cena += predmety[i];
    }

    /* Celkova cena musi byt delitelna 3 */
    if (celkova_cena % 3 != 0) {
        print_notfairly();
        return (EXIT_SUCCESS);
    }
    // Kazdy clen musi byt <= podilu    
    podil = celkova_cena / 3;
    for (i = 0; i < pocet_predmetu; i++) {
        if (predmety[i] > podil) {
            print_notfairly();
            return (EXIT_SUCCESS);
        }
    }

    /* Vyhodnoceni */
    dostatek_kombinaci = false;
    
    projdi_kombinace(0, 0, pocet_predmetu, NULL);

    /* printf("Pocet kombinaci: %d\n", pocet_spravnych_kombinaci); */

    if (dostatek_kombinaci || pocet_spravnych_kombinaci >= pocet_predmetu) {
        print_fairly();
    } else {
        print_notfairly();
    }

    /* Uklid, sbohem a satecek */
    if (predmety != NULL) free(predmety);    
    for (i = 0; i < pocet_spravnych_kombinaci; i++) {
        if (spravne_kombinace[i] != NULL) free(spravne_kombinace[i]);
    }
    if (spravne_kombinace != NULL) free(spravne_kombinace);

    return (EXIT_SUCCESS);
}

/* -------------------------------------------------------------------------- */

void projdi_kombinace(int parent, int deep, int n, char* cesta) {

    if (deep > n - 2) return;
    if (pocet_spravnych_kombinaci >= pocet_predmetu) return;
    if(dostatek_kombinaci) return;

    int i;

    /* Zapis cesty od rodice k potomku */
    char* nova_cesta = NULL;
    /* Nulu vypisovat nechceme */
    if (deep > 0) {
        int ncesta_ln = deep * POCET_MIST;
        int desitky, jednotky;

        nova_cesta = (char*) calloc(ncesta_ln, sizeof (char));
        if (deep > 1) {
            for (i = 0; i < ncesta_ln - POCET_MIST; i++) {
                nova_cesta[i] = cesta[i];
            }
        }
        desitky = parent / 10;
        jednotky = parent % 10;

        nova_cesta[ncesta_ln - 3] = desitky ? desitky + '0' : ' ';
        nova_cesta[ncesta_ln - 2] = jednotky + '0';
        nova_cesta[ncesta_ln - 1] = ' ';

        /* Debug vypis */
        /* printf("%s\n", nova_cesta); */

        if(testuj_kombinaci(nova_cesta, ncesta_ln)){
            dostatek_kombinaci = existuji_3_kombinace_spolecne();
        }

    }

    for (i = parent + 1; i <= n; i++) {
        projdi_kombinace(i, deep + 1, n, nova_cesta);
    }

    free(nova_cesta);
}

/* -------------------------------------------------------------------------- */

boolean existuji_3_kombinace_spolecne(void){
    int i, j, k, cisel;
    boolean obsahuje_aktualni;
    
    obsahuje_aktualni = false;
    cisel = 0;
    
    /* Obsahuje tabulka vsechny prvky? */
    for(i=1; i <= pocet_predmetu; i++){
        for(j=0; j < pocet_spravnych_kombinaci; j++){
            for(k=0; k < pocet_predmetu-2; k++){
                if(spravne_kombinace[j][k]==i) {
                    obsahuje_aktualni=true;
                    break;
                }
            }
            if(obsahuje_aktualni) break;
        }
        if(obsahuje_aktualni) cisel++;
        obsahuje_aktualni = false;
    }
    
    if(cisel == pocet_predmetu) return true;
    return false;
}

/* -------------------------------------------------------------------------- */

boolean testuj_kombinaci(char* retezec, int delka) {
    int i, j, hodnota, pozice;
    j = 0;
    hodnota = 0;
    pozice = 0;
    /* Pole pro docasne ukladani pouzite kombinace, uklada se pozice v poli + 1 */
    pouzita_kombinace = (int*) calloc(pocet_predmetu-2, sizeof (int));
    
    for (i = 0; i < pocet_predmetu-2; i++) pouzita_kombinace[i] = 0;
        
    /* Secteme hodnoty jednotlivych pozic a ulozime jednotlive pozice */
    for(i=0; i < delka; i++){

        if(!is_cislo(retezec[i])) continue;

        /* Pokud je cislo na dve mista, obsahuje desitky a jednotky, pr. 10 */
        if (    (i+1 < delka) &&
                (is_cislo(retezec[i]) && is_cislo(retezec[i + 1]))  ) {
            /* Pricti desitky k pozici */
            pozice = char_int(retezec[i]) * 10;
            /* Pricti jednotky k pozici, jednotky jsou v cislovani od 1 */
            pozice += char_int(retezec[i + 1] - 1);
            /* Pricti k celkove hodnote hodnotu na aktualni pozici */
            hodnota += predmety[pozice];
            /* Uloz pozici tak, aby prvni pozice byla 1 */
            pouzita_kombinace[j++] = pozice + 1;
            /* i+2 cislo je dvouznakove | j++ pricti dalsi prvek  */
            i++;
        }/* Jinak je to cislo pouze s jednotkami */
        else if (is_cislo(retezec[i])) {
            /* Pricti jednotky k pozici */
            pozice = char_int(retezec[i] - 1);
            /* Pricti k celkove hodnote hodnotu na aktualni pozici */
            hodnota += predmety[pozice];
            /* Uloz pozici tak, aby prvni pozice byla 1 */
            pouzita_kombinace[j++] = pozice + 1;            
        }/* Pokud to neni cislo, posun se na dalsi pozici */             
    }
    /* Debug vypis 
     *      printf("Hodnota: %d\n", hodnota);
     *      printf("Reseni: %s\n", retezec); */

    /* Pokracujeme pouze, pokud jsme nasli odpovidajici kombinaci */
    if (hodnota != podil) {
        /* Uvolni pamet a vrat neuspech */
        free(pouzita_kombinace);
        return false;
    }

    pocet_spravnych_kombinaci++;

    /*printf("Nasel jsem spravne reseni\n");*/

    /* Zvetsi pole, aby se nam tam vesla dalsi kombinace = pridej novy radek */
    spravne_kombinace = (int**) realloc(spravne_kombinace, pocet_spravnych_kombinaci * sizeof (int*));
    /* Inicializuj radek */
    spravne_kombinace[pocet_spravnych_kombinaci - 1] = (int*) calloc(pocet_predmetu-2, sizeof (int));
    /* Napln radek pozicemi prvku, pokud je kam */
    if (spravne_kombinace[pocet_spravnych_kombinaci - 1] != NULL) {
        for (i = 0; i < pocet_predmetu-2; i++) {
            spravne_kombinace[pocet_spravnych_kombinaci - 1][i] = pouzita_kombinace[i];
            /* printf("%d ", pouzita_kombinace[i]); */
        }
        /* printf("\n"); */
    }
    
    /* Uvolni pamet a vrat uspesne nalezeni */
    free(pouzita_kombinace);
    return true;
}

/* -------------------------------------------------------------------------- */

int char_int(char znak) {
    if (znak >= '0' && znak <= '9') return znak - '0';
    return 0;
}

boolean is_cislo(char znak) {
    if (znak >= '0' && znak <= '9') return true;
    return false;
}

void print_error(void) {
    printf("Nespravny vstup.\n");
}

void print_fairly(void) {
    printf("Predmety lze rozdelit spravedlive.\n");
}

void print_notfairly(void) {
    printf("Predmety nelze rozdelit spravedlive.\n");
}
