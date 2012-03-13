/* 
 * File:   main.c
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 25. říjen 2009, 16:24
 */

#include <stdio.h>
#include <stdlib.h>

int existence_4uhelniku(int a, int b, int c, int d){
    int obvod = a + b + c + d;
    double pulobvod = (double)obvod / 2;

    return a<pulobvod && b<pulobvod && c<pulobvod && d<pulobvod;
}

int min_uhlopricka(int a, int b){
    if(a<b) return b-a;
    if(a>b) return a-b;
    if(a==b) return 0;
    return 0;
}

int max_uhlopricka(int a, int b){
    return a+b;
}

int min(int a, int b){
    if(a<b) return a;
    if(a>b) return b;
    if(a==b) return a;
    return 0;
}

int max(int a, int b){
    if(a>b) return a;
    if(a<b) return b;
    if(a==b) return a;
    return 0;
}

int main(void) {

    int a, b, c, d;
    int pocet_vstupu;
    int minAC, maxAC, minBD, maxBD;
    
    a = 0;    b = 0;    c = 0;    d = 0;
    
    printf("Zadejte velikosti stran ctyruhelniku:\n");

    pocet_vstupu = scanf("%d%d%d%d", &a, &b, &c, &d);
    
    if(pocet_vstupu!=4){
        printf("Nespravny vstup.\n");
        return (EXIT_FAILURE);
    }
    if(a<1 || b<1 || c<1 || d<1){
        printf("Nespravny vstup.\n");
        return (EXIT_FAILURE);
    }

    if(existence_4uhelniku(a,b,c,d)) {
        printf("Ctyruhelnik existuje.\n");

        minAC = max(min_uhlopricka(a,b),min_uhlopricka(d,c));
        maxAC = min(max_uhlopricka(a,b),max_uhlopricka(d,c));

        minBD = max(min_uhlopricka(a,d),min_uhlopricka(b,c));
        maxBD = min(max_uhlopricka(a,d),max_uhlopricka(b,c));

        printf("Velikost uhlopricky AC: interval (%d,%d)\n",minAC, maxAC);
        printf("Velikost uhlopricky BD: interval (%d,%d)\n",minBD, maxBD);
    } else{
        printf("Ctyruhelnik neexistuje.\n");
    }

    return (EXIT_SUCCESS);
}





