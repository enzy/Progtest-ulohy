/* 
 * File:   main.c - Semestralni uloha: OCR pro sedmisegmentovy displej
 * Auth||: Matej Simek - www.matejsimek.cz
 *
 * Created on 1. prosinec 2009, 22:35
 */

/*

 Rastr: 5x9 bodů
 Jeden segment: 3body svisle/vod||ovne
 Rozsviceny bod = '*'
 Zhasnuty bod = ' '
 Neurcity bod = '?'
 Mezi cislicemi 1 sloupec mezer
 Spravny vstup = vsechny radky stejna delka && pocet radku 9

 Vystup: ASCII prectene znaky, neurcity znak kde na vstupu jsou pouze otazniky - ?,
 na vstupu spatne nactene body a neodpovidaji - X

 Chybny vstup - nesmyslna data:
 * jiné znaky než mezera, hvězdička nebo otazník,
 * délky jednotlivých řádek nejsou stejné (některé jsou delší, jiné kratší),
 * není k dispozici dostatek řádek vstupu (9 řádek je výška),
 * jednotlivé cifry na vstupu nejsou oddělené sloupcem mezer,
 * délka řádek na vstupu neodpovídá násobku šířky znaku (plus oddělujícím sloupcům mezer).

 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* -------------------------------------------------------------------------- */

#define ZNAK_SIRKA 5
#define ZNAK_VYSKA 9

#define DOLNI_MEZ_HODNOSTI 0.25
#define TOLERANCE 0.0277778

/* -------------------------------------------------------------------------- */

/* Referencni pole pro urcovani segmentu prislusejicim cislicim */
const int segmenty[10][7] = {
    {1, 1, 1, 0, 1, 1, 1},
    {0, 0, 1, 0, 0, 1, 0},
    {1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 0, 1, 1},
    {0, 1, 1, 1, 0, 1, 0},
    {1, 1, 0, 1, 0, 1, 1},
    {1, 1, 0, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 1, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1}
};

const int pocet_segmentu[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

const double hodnost_hvezda = 0.333333;
const double hodnost_otaznik = 0.166666;
const double hodnost_mezera = 0;

/* -------------------------------------------------------------------------- */

/* 2D pole se vstupem */
char** vstup;
int sirka;
int vyska = ZNAK_VYSKA;
int pocet_cislic; /* (sirka+1) / 5 */

/* Nastaveni vypisu debug casti */
char debug =                 0;
char dbg_kontrola_radku =    0;
char dbg_pocet_cislic =      0;
char dbg_rozpoznej_znak =    0;
char dbg_shoda_znaku =       1;
int  dbg_pocet_vstupu =      18; /* 0 - 19 */
int  dbg_pocet_vstupu_od =   18; /* 0 - 19 */
char demence =               1;

/* -------------------------------------------------------------------------- */

void    print_error             (int ln);
double  gethod                  (char znak, int segment, int ref_cislo);
int     getdef                  (char znak);
int     getdef2                 (char znak);
int     getdef3                 (char znak);
char    kontrola_cislice        (int ref_znak, int pozice_znaku);
void    kontrola_radku          (char* radek);
char    kontrola_segmentu       (int segment, int ref_cislo, int pozice_znaku);
void    nacti_data              (FILE * inputfile);
void    nacti_prvniradek        (FILE * inputfile);
char    rozpoznej_znak          (int pozice_znaku);
double  shoda_segmentu          (int segment, int ref_cislo, int pozice_znaku);
double  shoda_znaku             (int ref_znak, int pozice_znaku);
char    testuj_nacitani_vstupu  (void);

/* -------------------------------------------------------------------------- */

double gethod(char znak, int segment, int ref_cislo) {
    double hodnost;
    hodnost = 0;

    if (znak == '*') hodnost = hodnost_hvezda;
    else if (znak == '?') hodnost = hodnost_otaznik;
    else if (znak == ' ') hodnost = hodnost_mezera;

    if (segmenty[ref_cislo][segment] == 0) hodnost *= -1;

    return hodnost;
}

double shoda_segmentu(int segment, int ref_cislo, int pozice_znaku) {
    int vlevo;
    vlevo = pozice_znaku * (ZNAK_SIRKA + 1);
    /* Kazdy segment ma 3 casti */
    double shoda;
    shoda = 0;
    int i;
    for (i = 0; i < 3; i++) {
        if (segment == 0 || segment == 3 || segment == 6) {
            if (segment == 0) {
                shoda += gethod(vstup[0][vlevo + i + 1], segment, ref_cislo);
            } else if (segment == 3) {
                shoda += gethod(vstup[4][vlevo + i + 1], segment, ref_cislo);
            } else if (segment == 6) {
                shoda += gethod(vstup[8][vlevo + i + 1], segment, ref_cislo);
            }
        }/* Pro segmenty  */
        else {
            if (segment == 1) {
                shoda += gethod(vstup[i + 1][vlevo], segment, ref_cislo);
            } else if (segment == 2) {
                shoda += gethod(vstup[i + 1][vlevo + 4], segment, ref_cislo);
            } else if (segment == 4) {
                shoda += gethod(vstup[i + 5][vlevo], segment, ref_cislo);
            } else if (segment == 5) {
                shoda += gethod(vstup[i + 5][vlevo + 4], segment, ref_cislo);
            }
        }
    }
    if(demence){
        if(shoda > 0.5) shoda = 1;
    }
    return shoda;
}

char platnost_segmentu(int segment, int ref_cislo, int pozice_znaku){
    int vlevo;
    vlevo = pozice_znaku * (ZNAK_SIRKA + 1);
    ref_cislo = 0;
    double shoda;
    shoda = 0;
    int i;
    char obsahuje_znak;
    obsahuje_znak = 0;

    for (i = 0; i < 3; i++) {
        if (segment == 0 || segment == 3 || segment == 6) {
            if (segment == 0) {
                if(getdef(vstup[0][vlevo + i + 1])) {
                    obsahuje_znak = 1;
                    break;
                }
            } else if (segment == 3) {
                if(getdef(vstup[4][vlevo + i + 1])) {
                    obsahuje_znak = 1;
                    break;
                }
            } else if (segment == 6) {
                if(getdef(vstup[8][vlevo + i + 1])) {
                    obsahuje_znak = 1;
                    break;
                }
            }
        }/* Pro segmenty  */
        else {
            if (segment == 1) {
                if(getdef(vstup[i + 1][vlevo])) {
                    obsahuje_znak = 1;
                    break;
                }
            } else if (segment == 2) {
                if(getdef(vstup[i + 1][vlevo + 4])) {
                    obsahuje_znak = 1;
                    break;
                }
            } else if (segment == 4) {
                if(getdef(vstup[i + 5][vlevo])) {
                    obsahuje_znak = 1;
                    break;
                }
            } else if (segment == 5) {
                if(getdef(vstup[i + 5][vlevo + 4])) {
                    obsahuje_znak = 1;
                    break;
                }
            }
        }
    }

    if(obsahuje_znak){
        for (i = 0; i < 3; i++) {
        if (segment == 0 || segment == 3 || segment == 6) {
            if (segment == 0) {
                if(!getdef(vstup[0][vlevo + i + 1])) return 0;
            } else if (segment == 3) {
                if(!getdef(vstup[4][vlevo + i + 1])) return 0;
            } else if (segment == 6) {
                if(!getdef(vstup[8][vlevo + i + 1])) return 0;
            }
        }/* Pro segmenty  */
        else {
            if (segment == 1) {
                if(!getdef(vstup[i + 1][vlevo])) return 0;
            } else if (segment == 2) {
                if(!getdef(vstup[i + 1][vlevo + 4])) return 0;
            } else if (segment == 4) {
                if(!getdef(vstup[i + 5][vlevo])) return 0;
            } else if (segment == 5) {
                if(!getdef(vstup[i + 5][vlevo + 4])) return 0;
            }
        }
    }
    }

    return 1;
}

double shoda_znaku(int ref_znak, int pozice_znaku) {

    double hodnost_cisla;
    hodnost_cisla = 0;

    /* Projdi vsech sedm segmentu a urci celkovou hodnost cisla */
    int i;
    for (i = 0; i < 7; i++) {
        /* TODO platnost segmentu */
        if(platnost_segmentu(i, ref_znak, pozice_znaku)){
            hodnost_cisla += shoda_segmentu(i, ref_znak, pozice_znaku);
        } else{            
                hodnost_cisla -= shoda_segmentu(i, ref_znak, pozice_znaku);            
        }
    }
    /* Vydel hodnost cislo celkovym poctem segmentu pro navraceni celkove shody */    
    return hodnost_cisla / pocet_segmentu[ref_znak];

}

char vsechny_otazniky(int pozice_znaku){
    int vlevo, i, segment, otazniku;
    vlevo = pozice_znaku * (ZNAK_SIRKA + 1);
    
        for(segment = 0; segment < 7; segment++){
            for (i = 0; i < 3; i++) {
                if (segment == 0 || segment == 3 || segment == 6) {
                        if (segment == 0) {
                            if(getdef3(vstup[0][vlevo + i + 1])) {
                                otazniku++;
                            }
                        } else if (segment == 3) {
                            if(getdef3(vstup[4][vlevo + i + 1])) {
                                otazniku++;
                            }
                        } else if (segment == 6) {
                            if(getdef3(vstup[8][vlevo + i + 1])) {
                                otazniku++;
                            }
                    }
                    }/* Pro segmenty  */
                else {
                    if (segment == 1) {
                        if(getdef3(vstup[i + 1][vlevo])) {
                            otazniku++;
                        }
                        } else if (segment == 2) {
                        if(getdef3(vstup[i + 1][vlevo + 4])) {
                            otazniku++;
                        }
                    } else if (segment == 4) {
                        if(getdef3(vstup[i + 5][vlevo])) {
                            otazniku++;
                        }
                    } else if (segment == 5) {
                        if(getdef3(vstup[i + 5][vlevo + 4])) {
                            otazniku++;
                        }
                    }
                }
            }
        }    

    if(otazniku >= 21) return 1;

    return 0;
}

/* TODO Predelat s prihlednutim k nove naprogramovanem vyhodnocevani */
char rozpoznej_znak(int pozice_znaku) {
    // TODO: rozpoznej_znak
    int i;
    double hodnosti[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double max;
    max = 0;
    int pozice_max;
    pozice_max = 0;
    int vlevo;
    vlevo = pozice_znaku * (ZNAK_SIRKA + 1);

    /* Spocitej hodnost kazde cislice a najdi maximum */
    if(debug && dbg_shoda_znaku) printf("\nHodnosti pro pozici %i: ", pozice_znaku);

    for (i = 0; i < 10; i++) {
        hodnosti[i] = shoda_znaku(i, pozice_znaku);
        if (hodnosti[i] > max) {
            max = hodnosti[i];
            pozice_max = i;
        }
        if(debug && dbg_shoda_znaku) printf("%i: %f ", i, hodnosti[i]);
    }
    if(debug && dbg_shoda_znaku) printf("\n");

    

    /* Umela demence snimaciho programu */
    if(demence){
        /* TODO: kdyz segment nesviti cely, pripadne je svitici bod mimo segment, vrat X */
        if(!kontrola_cislice(pozice_max, pozice_znaku)){            
            return 'X';
        }

        if(vsechny_otazniky(pozice_znaku)){
            return '?';
        }

        if(
                vstup[0][vlevo+1]=='?' &&
                vstup[0][vlevo+2]=='?' &&
                vstup[0][vlevo+3]=='?' &&
                vstup[4][vlevo+1]=='?' &&
                vstup[4][vlevo+2]=='?' &&
                vstup[4][vlevo+3]=='?' &&
                vstup[8][vlevo+1]=='?' &&
                vstup[8][vlevo+2]=='?' &&
                vstup[8][vlevo+3]=='?' &&
                vstup[1][vlevo]=='?' &&
                vstup[2][vlevo]=='?' &&
                vstup[3][vlevo]=='?' &&
                vstup[5][vlevo]=='?' &&
                vstup[6][vlevo]=='?' &&
                vstup[7][vlevo]=='?' &&
                vstup[1][vlevo+4]=='?' &&
                vstup[2][vlevo+4]=='?' &&
                vstup[3][vlevo+4]=='?' &&
                vstup[5][vlevo+4]=='?' &&
                vstup[6][vlevo+4]=='?' &&
                vstup[7][vlevo+4]=='?'
                ){
            return '?';
        }

        /* TODO: kontrola souvisejici s otazniky, vydedukovat jak postupovat */
        /* pokud je segment nejaky cely z otazniku, prozkoumej dalsi cislice, jestli tento segment taky nemaji */
        if(pozice_max==3){
            if(
                    vstup[4][vlevo+1]=='?' &&
                    vstup[4][vlevo+2]=='?' &&
                    vstup[4][vlevo+3]=='?' &&
                    vstup[8][vlevo+1]=='?' &&
                    vstup[8][vlevo+2]=='?' &&
                    vstup[8][vlevo+3]=='?'
                    )
            {                
                return '?';
            }
        }        
        /*if(pozice_max==6 && max - hodnosti[5] < 0.34){
            return '?';
        }*/
        /*if(pozice_max==9 && max - hodnosti[3] < 0.32){
            return '?';
        }*/
        if(pozice_max==1 && max-hodnosti[7] < 0.0279){
            return '?';
        }
        if(pozice_max==7 && (max-hodnosti[1] < 0.08334)){
            return '?';
        }
        if(pozice_max==7){            
                if(shoda_segmentu(0, 7, pozice_znaku) < 0.5) return '?';
        }
        if(pozice_max==0){
            if(shoda_segmentu(3, 8, pozice_znaku) > 0.5) return '?';
        }

    }

    /* Pokud existuje vice cislic se stejnym hodnocenim */
    /* TODO: Predelat - p||ovnavat jestli se od  maxima cislo nevzdaluje o hodnotu tolerance */
    for (i = 0; i < 10; i++) {
        if (i != pozice_max && (
                hodnosti[i] == max ||
                hodnosti[i] >= max - TOLERANCE
                )
                ) {            
            return '?';
        }
    }

    /* Jinak vrat maximum, pokud neni mensi nez dolni mez */

    if (debug && dbg_rozpoznej_znak) printf(" | H%.2f: ", max);

    if (max > DOLNI_MEZ_HODNOSTI) {        
        return pozice_max + '0';
    }

    /* Pokud je hodnost pod mezi, vrat X */    
    return 'X';
}

char kontrola_cislice(int ref_znak, int pozice_znaku){
    char defekt;
    defekt = 0;
    int vlevo;
    vlevo = pozice_znaku * (ZNAK_SIRKA + 1);

    /* Projdi vsech sedm segmentu a zjisti pritomnost defektu */
    int i;
    for (i = 0; i < 7; i++) {
        if(!kontrola_segmentu(i, ref_znak, pozice_znaku)){            
            return 0;
        }
    }

    /* Projdi prazdne misto mezi segmenty na pritomnost sviticich bodu */
    for(i = 0; i < 3; i++){
        /* Vnitrek */
        if(vstup[i+1][vlevo+1]=='*') return 0;
        if(vstup[i+1][vlevo+2]=='*') return 0;
        if(vstup[i+1][vlevo+3]=='*') return 0;
        if(vstup[i+5][vlevo+1]=='*') return 0;
        if(vstup[i+5][vlevo+2]=='*') return 0;
        if(vstup[i+5][vlevo+3]=='*') return 0;
    }
    /* Kotrola rohu */
    if(vstup[0][vlevo]=='*') return 0;
    if(vstup[0][vlevo+4]=='*') return 0;
    if(vstup[4][vlevo]=='*') return 0;
    if(vstup[4][vlevo+4]=='*') return 0;
    if(vstup[8][vlevo]=='*') return 0;
    if(vstup[8][vlevo+4]=='*') return 0;

    return !defekt;
}

char kontrola_segmentu(int segment, int ref_cislo, int pozice_znaku) {
    int vlevo;    
    vlevo = pozice_znaku * (ZNAK_SIRKA + 1);
    /* Kazdy segment ma 3 casti */
    int shoda = 0;
    int i;

    if(segmenty[ref_cislo][segment] == 1){

        for (i = 0; i < 3; i++) {
            if (segment == 0 || segment == 3 || segment == 6) {
                if (segment == 0) {
                    shoda += getdef(vstup[0][vlevo + i + 1]);
                } else if (segment == 3) {
                    shoda += getdef(vstup[4][vlevo + i + 1]);
                } else if (segment == 6) {
                    shoda += getdef(vstup[8][vlevo + i + 1]);
                }
            }/* Pro segmenty  */
            else {
                if (segment == 1) {
                    shoda += getdef(vstup[i + 1][vlevo]);
                } else if (segment == 2) {
                    shoda += getdef(vstup[i + 1][vlevo + 4]);
                } else if (segment == 4) {
                    shoda += getdef(vstup[i + 5][vlevo]);
                } else if (segment == 5) {
                    shoda += getdef(vstup[i + 5][vlevo + 4]);
                }
            }
        }

        if(shoda!=3) return 0;
    }
    else {
        for (i = 0; i < 3; i++) {
            if (segment == 0 || segment == 3 || segment == 6) {
                if (segment == 0) {
                    shoda += getdef2(vstup[0][vlevo + i + 1]);
                } else if (segment == 3) {
                    shoda += getdef2(vstup[4][vlevo + i + 1]);
                } else if (segment == 6) {
                    shoda += getdef2(vstup[8][vlevo + i + 1]);
                }
            }/* Pro segmenty  */
            else {
                if (segment == 1) {
                    shoda += getdef2(vstup[i + 1][vlevo]);
                } else if (segment == 2) {
                    shoda += getdef2(vstup[i + 1][vlevo + 4]);
                } else if (segment == 4) {
                    shoda += getdef2(vstup[i + 5][vlevo]);
                } else if (segment == 5) {
                    shoda += getdef2(vstup[i + 5][vlevo + 4]);
                }
            }
        }

        if (shoda >= 1) return 0;
    }

    return 1;
}

int getdef(char znak) {
    if (znak == '*') return 1;
    else if (znak == '?') return 1;
    return 0;
}

int getdef2(char znak) {
    if (znak == '*') return 1;
    return 0;
}

int getdef3(char znak) {
    if (znak == '?') return 1;
    return 0;
}

void kontrola_radku(char* radek) {
    int j;
    if (debug && dbg_kontrola_radku) printf("Kontrola radku:%s", radek);

    for (j = 0; j < sirka - 1; j++) {
        /* testovani na zakazany znak */
        if (radek[j] != '*' && radek[j] != '?' && radek[j] != ' ') {
            if (debug && dbg_kontrola_radku) printf("Jejda: sloupec %d | ", j);
            print_error(__LINE__);
        }
        /* testovani na mezeru mezi cislicemi */
        if (j != 0 && j != sirka - 1 && (j + 1) % 6 == 0 && radek[j] != ' ') {
            if (debug && dbg_kontrola_radku) printf("Jejda: sloupec %d | ", j);
            print_error(__LINE__);
        }
    }
}

void nacti_prvniradek(FILE * inputfile) {
    int i, j;
    /* Nacti prvni radek postupne a spocitej sirku */
    int limit_radky;
    limit_radky = (6 * 5) + 1;
    int pozice_sloupce;
    pozice_sloupce = 0;
    char konec_radku;
    konec_radku = 0;
    char* cast_radku;
    cast_radku = NULL;
    i = 1;

    while (!konec_radku) {
        /* Vyhraz misto pro cast prvni radky */
        cast_radku = (char*) calloc(limit_radky, sizeof (char));
        if (cast_radku == NULL) print_error(__LINE__);

        /* Vyhraz misto na dalsi prvky */
        vstup[0] = (char*) realloc(vstup[0], (sirka + limit_radky + 1) * sizeof (char));
        if (vstup[0] == NULL) print_error(__LINE__);

        /* Nacti cast prvni radky a zkontroluj nacteni */
        if (fgets(cast_radku, limit_radky, inputfile) == NULL) print_error(__LINE__);

        /* Projed celou nactenou cast */
        for (j = 0; j < limit_radky; j++) {

            if (cast_radku[j] != 0) {

                /* dokud nenarazis na konec radku, zkontroluj zkopiruj obsah */
                if (cast_radku[j] == '\n') {
                    konec_radku = 1;
                } else {
                    if (cast_radku[j] != '*' && cast_radku[j] != '?' && cast_radku[j] != ' ') print_error(__LINE__);
                }
                vstup[0][pozice_sloupce++] = cast_radku[j];

            } else {
                sirka += j;
                break;
            }
        }

        /* Zvys krok */
        if (i < 10) {
            limit_radky *= i;
        }

        /* Inkrementace zakladniho kroku */
        i++;

        /* Uvolni pamet */
        if (cast_radku != NULL) free(cast_radku);

    }
}

void nacti_data(FILE * inputfile) {
    int i;
    /* Vyhrazeni mista pro radky */
    vstup = (char**) calloc(ZNAK_VYSKA, sizeof (char*));
    if (vstup == NULL) print_error(__LINE__);

    /* Nacti prvni radek a spocitej sirku */
    nacti_prvniradek(inputfile);

    /* Nespravna sirka */
    if (sirka % 6 != 0) {
        if (debug) printf("Nespravna sirka: %d | ", sirka);
        print_error(__LINE__);
    }

    /* Spocitej pocet cislic */
    pocet_cislic = sirka / 6;
    if (debug && dbg_pocet_cislic) printf("Pocet cislic: %d\n", pocet_cislic);

    /* Zkotroluj 1. radek */
    kontrola_radku(vstup[0]);

    /* Nacti sloupce 2-9*/
    for (i = 1; i < ZNAK_VYSKA; i++) {
        vstup[i] = (char*) calloc(sirka + 1, sizeof (char));
        /* Nacti uz cely radek najednou */
        if (fgets(vstup[i], sirka + 1, inputfile) == NULL) print_error(__LINE__);
        /* projdi vstupni radku a otestuj, zdali neobsahuje nepovolene znaky */
        kontrola_radku(vstup[i]);
    }

}

/* -------------------------------------------------------------------------- */

int main(void) {

    /* Testovani programu */
    if (debug) {
        printf("Test nacitani vstupu\n");
        testuj_nacitani_vstupu();
        exit(EXIT_SUCCESS);
    }


    int i;
    i = 0;

    /* TODO: Rozhodovaci logika */

    nacti_data(stdin);

    for (i = 0; i < pocet_cislic; i++) {
        printf("%c", rozpoznej_znak(i));
    }
    printf("\n");


    /* Uklid, sbohem a satecek */
    for (i = 0; i < vyska; i++) {
        if (vstup[i] != NULL) free(vstup[i]);
    }
    if (vstup != NULL) free(vstup);

    return (EXIT_SUCCESS);
}

char testuj_nacitani_vstupu(void) {
    FILE * test_file;
    FILE * output_file;
    int pocet_testovacich_souboru = dbg_pocet_vstupu;
    int i, j;

    for (i = dbg_pocet_vstupu_od; i <= pocet_testovacich_souboru; i++) {

        if (i == 6 || i == 7 || i == 8 || i == 9 || i == 10) {
            continue;
        }

        vstup = NULL;
        sirka = 0;
        pocet_cislic = 0;

        char *filepath = "res/0000_in.txt";
        char *filepath2 = "res/0000_out.txt";

        if (i < 10) {
            filepath[7] = i + 48;
            filepath2[7] = i + 48;
        } else {
            filepath[6] = i / 10 + 48;
            filepath[7] = i % 10 + 48;
            filepath2[6] = i / 10 + 48;
            filepath2[7] = i % 10 + 48;
        }

        test_file = fopen(filepath, "r");
        output_file = fopen(filepath2, "r");

        printf(filepath);

        if (test_file == NULL) print_error(__LINE__);

        printf(": nacitam data...\n");

        nacti_data(test_file);

        for (j = 0; j < pocet_cislic; j++) {
            printf("%c", rozpoznej_znak(j));
        }

        char ref_vystup[10000];

        fscanf(output_file, "%s", ref_vystup);
        printf("\n%s\n", ref_vystup);

        fclose(test_file);
        fclose(output_file);
    }

    return 1;
}

void print_error(int ln) {
    if (debug) printf("Nespravny vstup. [%d])\n", ln);
    else printf("Nespravny vstup.\n");

    exit(EXIT_FAILURE);
}

