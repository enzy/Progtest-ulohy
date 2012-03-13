/* 
 * File:   main.c - Nejčastěji se vyskytující čísla
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 31. říjen 2009, 20:32
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int main(void) {

    int pocet, vstup, pocetcisel, max;
    int i,j, pozice;
    int incheck1, incheck2;

    printf("Zadejte pocet cisel:\n");
    incheck1 = scanf("%i", &pocet);
    if (incheck1 != 1 || pocet<1 || pocet>=10000) {printf("Nespravny vstup.\n"); return (EXIT_FAILURE);}

    int pole[10000][2];
    int nejvice[10000];
    pocetcisel=0;
    max = INT_MIN;

    printf("Zadejte cisla:\n");
    
    for(i=0; i<pocet; i++){
        incheck2 = scanf("%i", &vstup);
        if (incheck2 != 1) {printf("Nespravny vstup.\n"); return (EXIT_FAILURE);}

        pozice = i;

        for(j=0; j<=i; j++){
            if(pole[j][0]==vstup) {
                pozice = j;                
                break;
            }            
        }        
        pole[pozice][0]=vstup;
        pole[pozice][1]++;

        if(pole[pozice][1]>max) max=pole[pozice][1];
    }       

    for(i=0; i<pocet; i++){
        if(pole[i][1]==0 || pole[i][1]!=max) continue;

        nejvice[pocetcisel++]=pole[i][0];        
    }

    if(pocetcisel==1){
        printf("Nejcasteji se opakujici cislo se vyskytlo %dx a bylo to cislo: %d.\n", max, nejvice[0]);
    } else{
        printf("Nejcasteji se opakujici cisla se vyskytla %dx a byla to cisla:", max);
        for(i=0; i<pocetcisel; i++){
            if(i<pocetcisel-2) printf(" %d,",nejvice[i]);
            else if(i<pocetcisel-1) printf(" %d",nejvice[i]);
            else printf(" a %d.\n",nejvice[i]);
        }
    }    

    return (EXIT_SUCCESS);
}

