/* 
 * File:   main.c - Symetrie matice
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 7. listopad 2009, 16:19
 */

#include <stdio.h>
#include <stdlib.h>

void print_error(void){
    printf("Nespravny vstup.\n");
}

int symetrie_vertikalni(int** matice, int vyska, int sirka){    

    if (sirka==1) return 1;

    int pocet_pruchodu, i, j;
    
    if (sirka % 2 == 0) pocet_pruchodu = sirka / 2;
    else pocet_pruchodu = (sirka+1)/2 -1;
    
    /* projed sloupce */
    for (i=0; i < pocet_pruchodu; i++){
        /* projed sloupec po radcich */
        for (j=0; j < vyska; j++){
            /* pokud se hodnoty nerovnaji, vrat false */
            if (matice[j][i] != matice[j][sirka - 1 - i]) return 0;
        }
    }
    /* nenasli jsme neshodne prvky, matice je symetricka podle ver. osy */
    return 1;
}

int symetrie_horizontalni(int** matice, int vyska, int sirka) {
    
    if (vyska==1) return 1;

    int pocet_pruchodu, i, j;

    if (vyska % 2 == 0) pocet_pruchodu = vyska / 2;
    else pocet_pruchodu = (vyska + 1) / 2 - 1;

    /* projed radky */
    for (i = 0; i < pocet_pruchodu ; i++) {
        /* projed radek po sloupeckach */
        for (j = 0; j < sirka; j++) {
            /* pokud se hodnoty nerovnaji, vrat false */
            if (matice[i][j] != matice[vyska - 1 - i][j]) return 0;
        }
    }
    /* nenasli jsme neshodne prvky, matice je symetricka podle hor. osy */
    return 1;
}

int symetrie_stredova(int** matice, int vyska, int sirka){
    
    if (vyska==1 && symetrie_vertikalni(matice, vyska, sirka)==1) return 1;
    if (sirka==1 && symetrie_horizontalni(matice, vyska, sirka)==1) return 1;

    /*if (sirka != vyska) return 0;*/
    /* odtud mame zajistenou stejnou sirku a vysku matice */

    if (sirka == 1 && vyska == 1) return 1;

    int pocet_pruchodu, i, j;

    if (vyska % 2 == 0) pocet_pruchodu = vyska / 2;
    else pocet_pruchodu = (vyska+1)/2;

    /* projed tabulku dvemi smery - odzacatku a od konce */
    for (i=0; i < pocet_pruchodu; i++){
        for (j=0; j < sirka; j++){
            if (matice[i][j] != matice[vyska-1-i][sirka-1-j]) return 0;
        }
    }
    /* nenasli jsme neshodne prvky, matice je symetricka podle stredu */
    return 1;
}

int main(void) {

    int pocet_radek, pocet_sloupcu, pocet_prvku, pocet_prvku_nacteny;
    int incheck, i, j;
    int** data = NULL;

    printf("Zadejte pocet radek a pocet sloupcu:\n");
    incheck = scanf("%d%d", &pocet_radek, &pocet_sloupcu);
    if (incheck != 2 || pocet_radek <= 0 || pocet_sloupcu <= 0) {
        print_error();
        return (EXIT_FAILURE);
    }
    incheck = 0;

    pocet_prvku = pocet_radek * pocet_sloupcu;
    printf("Zadejte prvky matice (%d):\n", pocet_prvku);

    /* zaber pamet pro pocet radek */
    data = (int**) malloc(pocet_radek * sizeof (int*));
    /* jestli se nam nepovede pamet zabrat, dej o tom vedet */
    if (data == NULL) {
        print_error();
        return (EXIT_FAILURE);
    }
    pocet_prvku_nacteny = 0;
    /* postupne zaber pamet pro jednotlive bunky tabulky a nacti do nich vstup */
    for (i = 0; i < pocet_radek; i++) {
        /* zaber pamet pro aktualni radek */
        data[i] = (int*) malloc(pocet_sloupcu * sizeof (int));
        /* jestli se nam nepovede pamet zabrat, dej o tom vedet */
        if (data[i] == NULL) {
            print_error();
            return (EXIT_FAILURE);
        }
        /* nacti jednotlive bunky aktualniho radku */
        for (j = 0; j < pocet_sloupcu; j++) {
            incheck = scanf("%d", &data[i][j]);
            /* kontrola zadanych hodnot, nesmysly striktne odmitneme */
            if (incheck != 1) {
                print_error();
                return (EXIT_FAILURE);
            }
            incheck=0;
            pocet_prvku_nacteny++;
        }
    }

    if(pocet_prvku!=pocet_prvku_nacteny) {
        print_error();
        return (EXIT_FAILURE);
    }

    /* Rozhodovaci logika */
    if (symetrie_horizontalni(data, pocet_radek, pocet_sloupcu)==1){
        printf("Matice je symetricka podle horizontalni osy.\n");
    } else printf("Matice neni symetricka podle horizontalni osy.\n");

    if (symetrie_vertikalni(data, pocet_radek, pocet_sloupcu)==1){
        printf("Matice je symetricka podle vertikalni osy.\n");
    } else printf("Matice neni symetricka podle vertikalni osy.\n");

    if (symetrie_stredova(data, pocet_radek, pocet_sloupcu)==1){
        printf("Matice je symetricka podle stredu.\n");
    } else printf("Matice neni symetricka podle stredu.\n");


    /* vyprazdneni pole */
    for (i = 0; i < pocet_radek; i++) {
        free(data[i]);
    }
    free(data);

    /* Rozlouceni */
    return (EXIT_SUCCESS);
}

