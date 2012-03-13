/* 
 * File:   main.c - Shoda jmena souboru
 * Author: Matej Simek - www.matejsimek.cz
 *
 * Created on 29. listopad 2009, 10:49
 */

#include <stdio.h>
#include <stdlib.h>

#define boolean char
#define true 1
#define false 0

#define AND &&
#define OR ||

boolean shodne(const char* mask, const char* file, int pm, int pf) {
    if (mask[pm] == file[pf]) return true;
    return false;
}

boolean najdi_vyskyt(const char* mask, const char* file, int pm, int pf, int* pozice_masky, int* pozice_jmena) {
    int i;    
    /* Shodnost vyrazu */
    boolean shodnost = true;
    boolean vyskyt = false;

    /* Projdi celou masku */
    while (mask[pm]) {

        /* Na aktualni * a na dalsi je tez *   */
        if (mask[pm] == '*' AND mask[pm + 1] == '*') {
            pm++;
            continue;
        }
        /* Na aktualni * a na dalsi konec */
        if (mask[pm] == '*' AND !mask[pm + 1]) {
            break;
        }
        /* Na aktualni * posun se na normal znak */
        if (mask[pm] == '*') {
            /* Pokud znak za * neodpovida znaku ve jmene, jed dal */

            /* Projed cele jmeno souboru a pro kazdy vyskyt znaku shodnym v masce,
             * zavolej fci, ktera porovna cast do *
             * vysledky mezi sebou logicky secist a pokud bude jeden pravda,
             * preskocit v pozici masky a jmena na danou pozici
             */
            i = pf;
            while(file[i]){
                if(mask[pm+1]==file[i]){
                    vyskyt = vyskyt OR najdi_vyskyt(mask, file, pm+1, i, &pm, &pf);
                }
                i++;
            }
            if(!vyskyt){
                shodnost = false;
                break;
            }            
        }/* Pokud je v masce normalni znak, srovnej ho se znakem ve jmene */
        else {
            if (!shodne(mask, file, pm, pf)) {
                shodnost = false;
                break;
            }
            pm++;
            pf++;
            if (!mask[pm] AND file[pf]) {
                shodnost = false;
                break;
            }
        }
    }

    if(shodnost){
        *pozice_masky = pm;
        *pozice_jmena = pf;
    }

    return shodnost;

}

int fileMatch(const char * mask, const char * file) {    
    int pm = 0;    
    int pf = 0;
    
    return (int) najdi_vyskyt(mask, file, pm, pf, &pm, &pf);
}


#ifndef __PROGTEST__

int main(void) {
    int x;

    x = fileMatch("pokus", "pokus");
    printf("%d (1) : pokus | pokus\n", x);

    x = fileMatch("pokus", "pokusy");
    printf("%d (0) : pokus | pokusy\n", x);

    x = fileMatch("pokus*", "pokusny");
    printf("%d (1) : pokus* | pokusny\n", x);

    x = fileMatch("*pokus*", "Toto je pokusny text.");
    printf("%d (1) : *pokus* | Toto je pokusny text.\n", x);

    x = fileMatch("*prog*Prog*", "Neni nad programovani na Progtestu.");
    printf("%d (1) : *prog*Prog* | Neni nad programovani na Progtestu.\n", x);

    x = fileMatch("*prog*Prog*", "Opravdovi programatori programuji programy tak, ze na Progtestu odevzdaji program na prvni pokus.");
    printf("%d (1) : *prog*Prog* | Opravdovi programatori programuji programy tak, ze na Progtestu odevzdaji program na prvni pokus.\n", x);

    x = fileMatch("Prog*test", "Progtest");
    printf("%d (1) : Prog*test | Progtest\n", x);

    x = fileMatch("Prog*test", "Progtrest");
    printf("%d (0) : Prog*test | Progtrest\n", x);

    x = fileMatch("Prog[test]", "Prog[test]");
    printf("%d (1) : Prog[test] | Prog[test]\n", x);

    x = fileMatch("**********", "");
    printf("%d (1) : ********** | \n", x);

    printf("%d (0) : abc*ghi | abcdghix\n", fileMatch("abc*ghi", "abcdghix"));
    printf("%d (1) : ir*%Il*W*F | ir*Db&%IlO/WEF\n", fileMatch("ir*%Il*W*F", "ir*Db&%IlO/WEF"));
    printf("%d (1) : *1&Tg***:c | G*UTOU1&Tg;oJVWU5j&:c\n", fileMatch("*1&Tg***:c", "G*UTOU1&Tg;oJVWU5j&:c"));
    printf("%d (1) : 'jm*s$Bx`uu** | 'jmb8J**>lls$Bx`uuk32 ABn\n", fileMatch("'jm*s$Bx`uu**", "'jmb8J**>lls$Bx`uuk32 ABn"));
    printf("%d (0) : **2wbkO*$. | aS@F3xclPV%/\n", fileMatch("**2wbkO*$.", "aS@F3xclPV%/"));
    printf("%d (0) : ***DoT*\"** | CX!EpUl ]_4V#<.C-jK#]nSRP\\\n", fileMatch("***DoT*\"**", "CX!EpUl ]_4V#<.C-jK#]nSRP\\"));
    printf("%d (0) : **CYb--7'3 | oZ'DZc..8(4\n", fileMatch("**CYb--7'3", "oZ'DZc..8(4"));

    return (EXIT_SUCCESS);
}
#endif


/* __PROGTEST__ */

