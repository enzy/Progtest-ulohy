/* 
 * File:   main.c - Průsečíky úseček
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 25. říjen 2009, 19:33
 *
 * Inspired at: http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float velikost(int Vx, int Vy){
    return sqrtf(Vx*Vx + Vy*Vy);
}

int skalarni_soucin(int v1x, int v1y, int v2x, int v2y){
    return v1x*v2x + v1y*v2y;
}

int rovnobeznost(int v1x, int v1y, int v2x, int v2y){
    if(skalarni_soucin(v1x,v1y,-v2y,v2x)==0) return 1;
    else return 0;
}

/* --- */

float vypocet_ua_citatel(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    x2=0;y2=0;
    return (x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3);
}
float vypocet_ua_jmenovatel(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    return (y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1);
}
float vypocet_ua(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    return  vypocet_ua_citatel(x1, y1, x2, y2, x3, y3, x4, y4) /
            vypocet_ua_jmenovatel(x1, y1, x2, y2, x3, y3, x4, y4);
}

/* --- */

float vypocet_ub_citatel(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    x4=0;y4=0;
    return (x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3);
}
float vypocet_ub_jmenovatel(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    return vypocet_ua_jmenovatel(x1, y1, x2, y2, x3, y3, x4, y4);
}
float vypocet_ub(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    return  vypocet_ub_citatel(x1, y1, x2, y2, x3, y3, x4, y4) /
            vypocet_ub_jmenovatel(x1, y1, x2, y2, x3, y3, x4, y4);
}

/* --- */

int existuje_prusecik(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    float ua, ub;
    ua = vypocet_ua(x1, y1, x2, y2, x3, y3, x4, y4);
    ub = vypocet_ub(x1, y1, x2, y2, x3, y3, x4, y4);    

    if( ua>=0 && ua<=1 && ub>=0 && ub<=1 ) return 1;
    return 0;
}

float prusecikX(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    float ua;
    ua = vypocet_ua(x1, y1, x2, y2, x3, y3, x4, y4);

    return x1 + ua * (x2 - x1);
}
float prusecikY(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    float ua;
    ua = vypocet_ua(x1, y1, x2, y2, x3, y3, x4, y4);

    return y1 + ua * (y2 - y1);
}

int main(void) {

    /* Koncove body */
    int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy,
    /* Souradnice vektoru */
        ABx, ABy, CDx, CDy, ACx, ACy, BDx, BDy, BCx, BCy;

    float velikostAB, velikostCD;

    /* Prusecik */
    float px, py;

    int kontrola_vstupu1, kontrola_vstupu2;

    /* Vstup prvni usecky AB */
    printf("Zadejte souradnice koncovych bodu prvni usecky:\n");
    kontrola_vstupu1 = scanf("%i%i%i%i", &Ax, &Ay, &Bx, &By);
    if(kontrola_vstupu1!=4) {printf("Nespravny vstup.\n");return(EXIT_FAILURE);}   
    
    ABx = Bx-Ax;
    ABy = By-Ay;
    velikostAB = velikost(ABx,ABy);
    if(velikostAB==0) {printf("Nespravny vstup.\n");return(EXIT_FAILURE);}

    /* Vstup druhe usecky CD */
    printf("Zadejte souradnice koncovych bodu druhe usecky:\n");
    kontrola_vstupu2 = scanf("%i%i%i%i", &Cx, &Cy, &Dx, &Dy);
    if(kontrola_vstupu2!=4) {printf("Nespravny vstup.\n");return(EXIT_FAILURE);}
    
    CDx = Dx-Cx;
    CDy = Dy-Cy;
    velikostCD = velikost(CDx, CDy);
    if(velikostCD==0) {printf("Nespravny vstup.\n");return(EXIT_FAILURE);}

    BCx = Cx - Bx;
    BCy = Cy - By;

    ACx = Cx - Ax;
    ACy = Cy - Ay;    

    BDx = Dx - Bx;
    BDy = Dy - By;    

    /* Rovnobeznost */
    /*if(rovnobeznost(ABx, ABy, CDx, CDy)==1){*/
    if(vypocet_ua_jmenovatel(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy)==0 &&
       vypocet_ub_jmenovatel(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy)==0 ){

        /* Lezi na jedne primce */
        /*if(rovnobeznost(ABx, ABy, BCx, BCy)==1){*/
        
        if(/*existuje_prusecik(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy)==1 ||*/
           rovnobeznost(ABx, ABy, BCx, BCy)==1 &&
          (velikost(ACx, ACy)+velikost(BDx, BDy) <= velikost(ABx, ABy)+velikost(CDx, CDy)))
        {
           printf("Usecky lezi na spolecne primce a prekryvaji se.\n");
           return (EXIT_SUCCESS);
        }

        if(rovnobeznost(ABx, ABy, BCx, BCy)==1){
            printf("Usecky lezi na spolecne primce a neprekryvaji se.\n");
            return (EXIT_SUCCESS);
        }

        printf("Usecky jsou rovnobezne.\n");
        return (EXIT_SUCCESS);        

    } else if(skalarni_soucin(ABx, ABy, CDx, CDy)==0){
        printf("Usecky jsou na sebe kolme.\n");        
    } else {
        printf("Usecky jsou ruznobezne.\n");
    }

    if(existuje_prusecik(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy)==1)
    {
        px = prusecikX(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy);
        py = prusecikY(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy);
        printf("Prusecik: %g, %g\n",px, py);
    }
    else
        printf("Prusecik neexistuje.\n");                       

    return (EXIT_SUCCESS);
}

