/* 
 * File:   main.c - Třetí největší sudé číslo
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 31. říjen 2009, 18:30
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {

    int pocet, vstup, max1, max2, max3;
    int incheck1, incheck2;

    printf("Zadejte pocet cisel:\n");
    incheck1 = scanf("%i", &pocet);
    if (incheck1 != 1 || pocet<1) {
        printf("Nespravny vstup.\n");
        return (EXIT_FAILURE);
    }

    max1 = INT_MIN;
    max2 = INT_MIN;
    max3 = INT_MIN;    

    printf("Zadejte %d cisel:\n", pocet);

    int i;
    for (i = 0; i < pocet; i++) {
        incheck2 = scanf("%i", &vstup);
        if (incheck2 != 1) {printf("Nespravny vstup.\n"); return (EXIT_FAILURE);}

        if (vstup % 2 != 0) continue;

        if(vstup==max3 || vstup==max2 || vstup==max1) continue;

        if (vstup > max1) {
            if (max3 < max2) max3 = max2;
            if (max2 < max1) max2 = max1;
            max1 = vstup;
        } else if (vstup > max2) {
            if (max3 < max2) max3 = max2;
            max2 = vstup;
        } else if (vstup > max3) {
            max3 = vstup;
        }        
    }

    if (max3 == INT_MIN || max3 == max2 || max3 == max1) {
        printf("Treti nejvetsi sude cislo neexistuje.\n");
        return (EXIT_SUCCESS);
    }

    printf("Treti nejvetsi sude cislo: %d\n", max3);

    return (EXIT_SUCCESS);
}

