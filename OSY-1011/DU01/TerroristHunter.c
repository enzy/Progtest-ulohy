/*
 * File:   TerroristHunter
 * Author: Matej Simek | www.matejsimek.cz
 *
 * Created on 15. březen 2011, 18:02
 */


#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

    /*
        Úkolem je vytvořit část programu, který umožní rychlou analýzu záběrů z kamerového systému letiště.
     * Implementace bude využívat vlákna k tomu, aby na předložených snímcích vyhledala obrazy hledaných teroristů.

        Předpokládáme, že na letišti existuje kamerový systém, který na požádání dodá aktuální snímek.
     * Tomuto systému budeme říkat scanner. Dále předpokládáme, že existuje obrazová databáze hledaných
     * teroristů. Pro jednoduchost předpokládáme, že databáze má nejvýše 32 položek.
     * Konečně, máme k dispozici rozhraní na zodpovědného bezpečnostního pracovníka (officer),
     * kterému předáváme vyhodnocené snímky s informací, zda na nich jsou nějací z hledaných teroristů nebo ne.

        Vaším úkolem bude implementovat část programu, která bude žádat o snímky ze scanneru,
     * ve získaném snímku bude vyhledávat hledané teroristy z databáze a o výsledcích hledání
     * bude informovat příslušného pracovníka (officer). Naší snahou je zpracovávat co nejvíce
     * snímků a co nejrychleji. Proto použijeme řešení s vlákny, které dokáže výpočetní zátěž
     * rozložit na více procesorů/jader.

        Vaším úkolem je využít následující rozhraní a implementovat funkci TerroristHunter:
     */

    /*
     Struktura TIMAGE reprezentuje jeden obrázek. Složka m_W udává šířku a m_H výšku obrázku.
     * Vlastní obrazová data jsou předaná v podobě 2D pole m_Data. Toto pole obsahuje m_H řádek
     * a m_W sloupců, v každém je uložena "barva" - odstín šedé od 0 do 254. Hodnota 255 má
     * zvláštní význam, viz níže.
     */
    typedef struct TImage {
        int m_W;
        int m_H;
        unsigned char ** m_Data;
    } TIMAGE;

    /*
        Funkce TerroristHunter je rozhraní pro Vaší implementaci. Tato funkce zajišťuje celý proces
        získávání/zpracování/odevzdávání obrazů. Má následující parametry:

        - databaseSize udává počet hledaných teroristů v databázi. Celkový počet je vždy maximálně 32.
        - database je pole odkazů na obrázky hledaných teroristů. Pole má vyplněno prvních databaseSize položek.
        - threads je počet pracovních vláken, která má Vaše implementace vytvořit.
          Mezi tato vlákna se bude rozkládat výpočetní zátěž.
        - scanner je ukazatel na funkci generující snímek. Vaše implementace bude tuto funkci volat vždy,
          když bude potřebovat další snímek ke zpracování. Funkce vrátí ukazatel na načtený snímek nebo hodnotu NULL,
          pokud již další snímky nejsou k dispozici (konec směny).
        - officer je ukazatel na funkci, které budete odevzdávat zpracované a vyhodnocené snímky.
          Parametrem funkce je zpracovaný snímek (ukazatel na něj, tak jak jste jej obdrželi z funkce scanner)
          a pole příznaků, kteří teroristé byli na snímku detekování. Druhý parametr (hodnotu int) chápejte jako
          bitové pole, kde každému bitu odpovídá jeden terorista z databáze (bit 0 bude mít hodnotu 1 pokud
          obrázek obsahoval teroristu z database[0], bit 1 bude mít hodnotu 1 pokud obrázek obsahoval teroristu
          z database[1], ...). Každý zpracovávaný obrázek předávejte touto funkcí pouze jednou, v okamžiku kdy již
          vyhodnotíte přítomnost/nepřítomnost všech hledaných teroristů z databáze.
     */

#endif /* __PROGTEST__ */    

    int power(int x, int y) {
        int i, res;
        res = 1;
        for (i = 0; i < y; i++) {
            res *= x;
        }
        return (int)res;
    }

    /*
        Vaše implementace bude mít následující hrubou strukturu:

        Inicializuje Vaše struktury (fronty), do kterých budete ukládat zpracovávané požadavky.

        Vytvoří threads vláken, každé z těchto vláken bude podle potřeby vyhledávat teroristy
        z databáze ve zpracovávaných obrazech.

        Vytvoří vlákno, které bude volat scanner a bude rozdělovat práci mezi ostatní vlákna.
        Pro tuto činnost můžete vytvořit buď samostatné nové vlákno, nebo můžete využít vlákno,
        které vyvolalo funkci TerroristHunter.

        Vlákna zpracovávají příchozí požadavky, po vyhodnocení vracejí výsledky hledání
        (i výsledky negativní) rozhraním officer.

        Po zpracování všech požadavků (scanner začne vracet NULL) ukončíte vytvořená vlákna,
        uvolníte Vámi alokované prostředky a vrátíte se z volání TerroristHunter.

     */

    typedef struct TInstruction {
        int id; /* For consistency check */
        int itemsToScan; /* number of non scanned terrorists, must equal 0 at the end  */
        int itemsScanned; /* number of scanned terrorists, must equal databaseSize at the end */
        int scanResult;
        TIMAGE * imageToScan; /* image to scan in for terrorists */
    } TINSTRUCTION;

    typedef struct TWaitLine {
        struct TWaitLine * NextInLine;
        TINSTRUCTION * Instruction;
    } TWAITLINE;

    typedef struct TArguments {
        int databaseSize; /* Number of terrorists in database */
        TIMAGE ** database; /* Terrorists images database */
        TIMAGE * (* scanner) (void); /* scanner */
        void (* officer) (TIMAGE *, int); /* function to pass results */

        TWAITLINE * WaitLineFirst;
        TWAITLINE * WaitLineLast;
        int WaitLineSize;

        pthread_mutex_t * mtxInstructionArray; /* mutex for accessing items in array */        
        pthread_mutex_t /* * mtxCondFreeSpaceInArray,*/ *mtxCondAvailableItemsToControl, *mtxCondNoItemsInArray;
        pthread_cond_t /* * condFreeSpaceInArray,*/ *condAvailableItemsToControl, *condNoItemsInArray;
    } TARGUMENTS;


    void WaitLineAddToEnd(TARGUMENTS * Args, TINSTRUCTION * Ins){
        TWAITLINE * newItem = (TWAITLINE *) malloc(sizeof(* newItem));

        newItem->Instruction = Ins;
        newItem->NextInLine = NULL;

        if(Args->WaitLineSize == 0){
            Args->WaitLineFirst = newItem;
            Args->WaitLineLast = newItem;
        } else {
            Args->WaitLineLast->NextInLine = newItem;
            Args->WaitLineLast = newItem;
        }
        Args->WaitLineSize++;
        /*printf("WaitLine: Add to end\n");*/
    }

    void WaitLineDeleteFirst(TARGUMENTS * Args){        
        if(Args->WaitLineSize == 0){
            if(Args->WaitLineFirst != NULL || Args->WaitLineLast != NULL){
                Args->WaitLineFirst = NULL;
                Args->WaitLineLast = NULL;
            }
            return;
        } else if(Args->WaitLineSize == 1){
            free(Args->WaitLineFirst);
            Args->WaitLineFirst = NULL;
            Args->WaitLineLast = NULL;
            Args->WaitLineSize = 0;            
        } else {
            TWAITLINE * newFirst;
            newFirst = Args->WaitLineFirst->NextInLine;
            free(Args->WaitLineFirst->Instruction);
            free(Args->WaitLineFirst);
            Args->WaitLineFirst = NULL;
            Args->WaitLineFirst = newFirst;
            Args->WaitLineSize--;            
        }        
        /*printf("WaitLine: Delete first\n");*/
    }
    
    unsigned char WaitLineTryDetachOrphan(TARGUMENTS * Args, TWAITLINE * Orphan){
        /*printf("WaitLine: Try detach orphan\n");*/
        if(Args->WaitLineFirst == Orphan){            
            Args->WaitLineFirst = Args->WaitLineFirst->NextInLine;            
            Args->WaitLineSize--;
            return 1;
        }
        return 0;
    }
    
    unsigned char WaitLineDeleteOrphan(TARGUMENTS * Args, TWAITLINE * Orphan){
        /*printf("WaitLine: Delete Orphan\n");*/
        if(Args->WaitLineFirst != Orphan){
            free(Orphan->Instruction);
            free(Orphan);
            return 1;
        }
        return 0;
    }
    
    /**
     * Check wait line implementation for errors
     * 
    unsigned char WaitLineUnitTest(TARGUMENTS * Args){
        TWAITLINE * First = Args->WaitLineFirst;
        TWAITLINE * Last = Args->WaitLineLast;
        TWAITLINE * Cache1, * Cache2, * Cache3;
        int Size = Args->WaitLineSize;
        
        if(Size != 0) return 0;
        if(First!=NULL && Last!=NULL) return 0;
        
        WaitLineAddToEnd(Args, NULL);
        if(First!=Last) return 0;
        WaitLineAddToEnd(Args, NULL);
        WaitLineAddToEnd(Args, NULL);
        WaitLineAddToEnd(Args, NULL);
        WaitLineAddToEnd(Args, NULL);
        
        if(Args->WaitLineSize != 5) return 0;
        if(First!=NULL && Last!=NULL) return 0;
        
        WaitLineDeleteFirst(Args);
        WaitLineDeleteFirst(Args);
        WaitLineDeleteFirst(Args);
        WaitLineDeleteFirst(Args);
        WaitLineDeleteFirst(Args);
        
        if(Args->WaitLineSize != 0) return 0;        
        
        WaitLineAddToEnd(Args, NULL);
        WaitLineAddToEnd(Args, NULL);
        WaitLineAddToEnd(Args, NULL);
        WaitLineAddToEnd(Args, NULL);                
        
        Cache1 = Args->WaitLineFirst;
        Cache2 = Cache1->NextInLine;
        Cache3 = Cache2->NextInLine;
        
        if(WaitLineTryDetachOrphan(Args, Cache2)) return 0;
        if(WaitLineTryDetachOrphan(Args, Cache3)) return 0;
        
        if(!WaitLineTryDetachOrphan(Args, Cache1)) return 0;        
        if(Args->WaitLineFirst != Cache2) return 0;
        if(Args->WaitLineSize != 3) return 0;
        
        if(!WaitLineDeleteOrphan(Args, Cache1)) return 0;
        if(Args->WaitLineSize != 3) return 0;
        
        WaitLineDeleteFirst(Args);
        WaitLineDeleteFirst(Args);
        WaitLineDeleteFirst(Args);
        
        if(Args->WaitLineSize != 0) return 0;                                   
        
        printf("WaitLineUnitTest: TEST COMPLETED SUCCESSFULLY\n");
        return 1;        
    }
     */ 
    

    /**
     * 
     * @param whatToFind Terrorist image from database
     * @param whereFind Source image from camera
     * @return unsigned char
     */
    unsigned char searchInMatrix(TIMAGE * terroristImage, TIMAGE * cameraImage) {
        int cameraWidth, cameraHeight, terroristWidth, terroristHeight; /* Dimensions */
        int cameraX, cameraY, terroristX, terroristY; /* Positions */
        int xSteps, ySteps;
        unsigned char searchResult, singleSearchResult;

        cameraWidth = cameraImage->m_W;
        cameraHeight = cameraImage->m_H;
        terroristWidth = terroristImage->m_W;
        terroristHeight = terroristImage->m_H;

        xSteps = cameraWidth - terroristWidth + 1;
        ySteps = cameraHeight - terroristHeight + 1;

        searchResult = 0;
        singleSearchResult = 1;

        cameraX = cameraY = terroristX = terroristY = 0;

        /* Steps through source matrix */
        for (cameraX = 0; cameraX < xSteps; cameraX++) {
            for (cameraY = 0; cameraY < ySteps; cameraY++) {
                singleSearchResult = 1;
                /* Compare destination matrix */
                for (terroristX = 0; terroristX < terroristWidth; terroristX++) {
                    for (terroristY = 0; terroristY < terroristHeight; terroristY++) {
                        /* If it isnt background */
                        if (terroristImage->m_Data[terroristY][terroristX] != 255) {
                            if (terroristImage->m_Data[terroristY][terroristX] != cameraImage->m_Data[cameraY + terroristY][cameraX + terroristX]) {
                                singleSearchResult = 0;
                                break;
                            }
                        }
                    }
                    if (!singleSearchResult) break;
                }

                if (singleSearchResult) {
                    searchResult = singleSearchResult;
                    break;
                }
            }
            if (searchResult) break;
        }
        return searchResult;
    }    

    /**
     * Worker thread to do heavy and dirty jobs
     */ 
    void * workers(void * Arguments) {
        /*
         * Zivotni cyklus:
         * 1. ze scanneru se precte obrazek pro kontrolu, vytvori se TInstruction a prida se do pole pro zpracovani
         * 2. worker si precte posledni polozku v poli, dekrementuje itemsToScan, zapamatuje si id a pozici, provede vypocet
         * 3. worker ulozi do polozky vysledek a inkrementuje itemsScanned, zkontroluje pritom id
         * 4. vse pokracuje, dokud itemsToScan neni 0 a itemsScanned neni rovno databaseSize
         * 5. v tom pripade se polozka vezme, preda officerovi vysledky a vymaze instrukcy z pole
         */

        pthread_t tid;
        int insID, itemsScanned, itemsToScan, terroristId, terroristMatch, cacheImageScanResult;

        TARGUMENTS * Args;
        TIMAGE * imageToControl, * cacheImageToControl;        
        TINSTRUCTION * instruction;
        TWAITLINE * WaitLineActual;

        tid = pthread_self();
        Args = (TARGUMENTS *) Arguments;        
        
        
        /*pthread_mutex_lock(Args->mtxInstructionArray);
        printf("WORKER [%d]: Cekam na available items to control...\n", (int)tid);
        pthread_cond_wait(Args->condAvailableItemsToControl, Args->mtxInstructionArray);
        printf("WORKER [%d]: Uz jsou dostupne polozky...\n", (int)tid);
        pthread_mutex_unlock(Args->mtxInstructionArray);*/

        while (1) {            

            /* CRITICAL - mtxInstructionArray */
            /*printf("WORKER [%d]: Cekam na pristup do isntruction array...\n", (int)tid);*/
            pthread_mutex_lock(Args->mtxInstructionArray);
            /*printf("WORKER [%d]: Byl mi udelen pristup do isntruction array.\n", (int)tid);*/

            /* If array is empty, unlock array mutex and wait for signal of available items and then restart loop */
            if (Args->WaitLineSize < 1) {
                pthread_cond_broadcast(Args->condNoItemsInArray);                
                /*printf("WORKER [%d]: Cekam na items to control... (WaitLineSize < 1)\n", (int)tid);*/
                pthread_cond_wait(Args->condAvailableItemsToControl, Args->mtxInstructionArray);
                /*printf("WORKER [%d]: Probudil jsem se z cekani na items to control... (insIndex = -1)\n", (int)tid);*/
                pthread_mutex_unlock(Args->mtxInstructionArray);
                continue;
            }

            /* Array has some instructions to compute, load needed data */
            /* instruction = &InsArray->instruction[insIndex]; */
            WaitLineActual = Args->WaitLineFirst;
            instruction = WaitLineActual->Instruction;
            insID = instruction->id;
            itemsScanned = instruction->itemsScanned;
            itemsToScan = terroristId = instruction->itemsToScan;
            imageToControl = instruction->imageToScan;

            /* TEST */
            /*pthread_mutex_unlock(Args->mtxInstructionArray);*/

            /*instruction->itemsToScan--;*/

            /* 
             * If all operations on item is done
             */
            if (itemsToScan <= 0) { /* TODO if equals -1 */
                /* 
                 * Maybe some nice instructions is here for suicide
                 */
                if(itemsToScan == -666){
                    /*printf("WORKER [%d]: Uvolnuji mutex instruction array.\n", (int)tid);*/
                    pthread_mutex_unlock(Args->mtxInstructionArray);
                    break;
                }
                /* 
                 * And all operations is computed
                 */
                else if (itemsScanned == Args->databaseSize) {
                    /* Then snstructions is complete, send results to officier */
                    Args->officer(instruction->imageToScan, instruction->scanResult);
                    /* TEST */
                    /*pthread_mutex_lock(Args->mtxInstructionArray);*/

                    /* Then delete item from array */
                    instruction->itemsToScan = -1;
                    /*printf("WORKER [%d]: Ukoncuji instrukci %d\n", (int)tid, insID);*/
                    /*InsArray->lastItem--;*/
                    WaitLineDeleteFirst(Args);
                    /* Signal free space in array to work CONTROL */
                    /* pthread_cond_broadcast(Args->condFreeSpaceInArray); */
                    /* Unlock array mutex */
                    /*printf("WORKER [%d]: Uvolnuji mutex instruction array.\n", (int)tid);*/
                    pthread_mutex_unlock(Args->mtxInstructionArray);
                    /* CRITICAL END - mtxInstructionArray */
                    /* and skip to next item */
                    continue;
                }/* 
                  * There is still some computation which isnt done but its in progress
                  */
                else {
                    /* TODO: Skocit na dalsi polozku? Co kdyz ta se vykona drive a smaze se jeste pred touto prave dokoncovanou? - PRUSER! */
                    /* Co takhle ji smazat uz rovnou pouze zpusobem, ze se na nej dalsi nedostanou? Puvodni vlakno by si melo pamatovat pozici, takze by to melo byt ok
                     * puvodni vlakno v tom pripade musi ale udelat uklid a predani vysledku samo!! */
                    /* TODO: OVERIT korektnost!!!! */

                    /* TEST */
                    /*pthread_mutex_lock(Args->mtxInstructionArray);*/
                    
                    /* InsArray->lastItem--; */
                    WaitLineTryDetachOrphan(Args, WaitLineActual);
                    /*printf("WORKER [%d]: Mazu posledni instrukci WaitLineTryDetachOrphan()\n", (int)tid);*/
                    /*printf("WORKER [%d]: Uvolnuji mutex instruction array.\n", (int)tid);*/
                    pthread_mutex_unlock(Args->mtxInstructionArray);
                    /* CRITICAL END - mtxInstructionArray */
                    continue;
                }
            }/* 
              * There is some operations on item which needs calculation
              */
        else {
            /*printf("WORKER [%d]: Je to najaka pracicka ^^\n", (int) tid);*/

            /* TEST */
            /*pthread_mutex_lock(Args->mtxInstructionArray);*/

            /* Take one item to compute (terroristId)*/
            instruction->itemsToScan--;
            /* Unlock array for others */
            /*printf("WORKER [%d]: Uvolnuji mutex instruction array.\n", (int) tid);*/
            pthread_mutex_unlock(Args->mtxInstructionArray);
            /* CRITICAL END - mtxInstructionArray */

            /* Lets do some heavy work now */
            /*printf("WORKER [%d]: Posilam data na vypocet (ID:%d, T:%d)\n", (int) tid, insID, terroristId - 1);*/
            terroristMatch = searchInMatrix(Args->database[terroristId - 1], imageToControl);

            /* Contruct result part */
            if (terroristMatch) {
                terroristMatch = power(2, terroristId - 1);
            } else {
                terroristMatch = 0;
            }

            /* Save results and do some control work */

            /* CRITICAL - mtxInstructionArray */
            pthread_mutex_lock(Args->mtxInstructionArray);

            /*printf("WORKER [%d]: Data vypoctena, zapisuji vysledky...\n", (int) tid);*/
            instruction->itemsScanned++;
            instruction->scanResult += terroristMatch;

            /* If this is last operation on item, pass it to the officier */
            if (instruction->itemsToScan == 0 && instruction->itemsScanned == Args->databaseSize) {
                /*printf("WORKER [%d]: Vsechny ukony v aktualni instrukci dokonceny.\n", (int) tid);*/
                /* Then snstructions is complete, send results to officier */
                cacheImageToControl = instruction->imageToScan;
                cacheImageScanResult = instruction->scanResult;

                /* If its still in wait line */
                if (Args->WaitLineFirst == WaitLineActual) {
                    WaitLineDeleteFirst(Args);
                }/* Its propably orphan */
                else {
                    WaitLineDeleteOrphan(Args, WaitLineActual);
                }
                /* Signal free space in array to work CONTROL */
                /* pthread_cond_broadcast(Args->condFreeSpaceInArray); */
                /*printf("WORKER [%d]: Uvolnuji mutex instruction array.\n", (int) tid);*/
                pthread_mutex_unlock(Args->mtxInstructionArray);

                /*printf("WORKER [%d]: Posledni vypocet, odevzdavam oficirovi...\n", (int) tid);*/
                Args->officer(cacheImageToControl, cacheImageScanResult);
            } else {
                /* Unlock array mutex */
                /*printf("WORKER [%d]: Uvolnuji mutex instruction array.\n", (int) tid);*/
                pthread_mutex_unlock(Args->mtxInstructionArray);
                /* CRITICAL END - mtxInstructionArray */
            }


            /* and skip to next item */
            continue;
            /* } */
        }
        /**/

        /* TODO */

    } /* ENDLESS LOOP ]:-> */


    /* This is the end of worker live, rest in peace bro */
    /*printf("[%d] Giving up, I'm done\n", (int) tid);*/
    pthread_exit(NULL);
}

/**
 * TerroristHunter
 * @param databaseSize
 * @param database
 * @param threads
 * @param scanner
 * @param officer
 */
void TerroristHunter(int databaseSize, TIMAGE ** database, int threads, TIMAGE *(* scanner) (void), void (* officer) (TIMAGE *, int)) {

    int i = 0, resultCache = 0, imageCount = 0;
    void *dummy;

    TARGUMENTS Args;
    TARGUMENTS * ArgsP;
    TINSTRUCTION * newInstructionKill;

    TIMAGE * imageToControl;

    pthread_mutex_t mtxInstructionArray;
    pthread_mutex_t /*mtxCondFreeSpaceInArray,*/ mtxCondAvailableItemsToControl, mtxCondNoItemsInArray;
    pthread_cond_t /* condFreeSpaceInArray,*/ condAvailableItemsToControl, condNoItemsInArray;
    pthread_t * threadArray;
    pthread_attr_t thread_attr;

    if (threads < 1) {
        perror("ERROR: No threads to run!\n");
        exit(EXIT_FAILURE);
    }

    threadArray = (pthread_t *) malloc(threads * sizeof ( pthread_t));
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

    pthread_mutex_init(&mtxInstructionArray, NULL);

    /*pthread_mutex_init(&mtxCondFreeSpaceInArray, NULL);*/
    pthread_mutex_init(&mtxCondAvailableItemsToControl, NULL);
    pthread_mutex_init(&mtxCondNoItemsInArray, NULL);
    /*pthread_cond_init(&condFreeSpaceInArray, NULL);*/
    pthread_cond_init(&condAvailableItemsToControl, NULL);
    pthread_cond_init(&condNoItemsInArray, NULL);


    Args.databaseSize = databaseSize;
    Args.database = database;
    Args.scanner = scanner;
    Args.officer = officer;

    Args.WaitLineFirst = NULL;
    Args.WaitLineLast = NULL;
    Args.WaitLineSize = 0;

    Args.mtxInstructionArray = &mtxInstructionArray;

    /*Args.mtxCondFreeSpaceInArray = &mtxCondFreeSpaceInArray;*/
    Args.mtxCondAvailableItemsToControl = &mtxCondAvailableItemsToControl;
    Args.mtxCondNoItemsInArray = &mtxCondNoItemsInArray;
    /*Args.condFreeSpaceInArray = &condFreeSpaceInArray;*/
    Args.condAvailableItemsToControl = &condAvailableItemsToControl;
    Args.condNoItemsInArray = &condNoItemsInArray;

    ArgsP = &Args;

    /* Connected list Unit test */
    /*if (WaitLineUnitTest(ArgsP) == 0) exit(EXIT_FAILURE);*/

    /* Create given number of threads */
    for (i = 0; i < threads; i++) {
        resultCache = pthread_create(&threadArray[i], &thread_attr, workers, (void *) ArgsP);
        if (resultCache) {
            perror("pthread_create() failed. \n");
            exit(EXIT_FAILURE);
        }
    }
    /*printf("Created %d threads\n", i);*/

    /*
     * Zivotni cyklus:
     * 1. ze scanneru se precte obrazek pro kontrolu, vytvori se TInstruction a prida se do pole pro zpracovani
     * 2. worker si precte posledni polozku v poli, dekrementuje itemsToScan, zapamatuje si id a pozici, provede vypocet
     * 3. worker ulozi do polozky vysledek a inkrementuje itemsScanned, zkontroluje pritom id
     * 4. vse pokracuje, dokud itemsToScan neni 0 a itemsScanned neni rovno databaseSize
     * 5. v tom pripade se polozka vezme, preda officerovi vysledky a vymaze instrukcy z pole
     */

    /* run scanner and plan threads here */
    imageToControl = scanner();
    while (imageToControl != NULL) {
        TINSTRUCTION * newInstruction = (TINSTRUCTION *) malloc(sizeof (* newInstruction));

        newInstruction->id = imageCount++;
        newInstruction->imageToScan = imageToControl;
        newInstruction->itemsScanned = 0;
        newInstruction->itemsToScan = databaseSize;
        newInstruction->scanResult = 0;

        /* CRITICAL - mtxInstructionArray */
        pthread_mutex_lock(&mtxInstructionArray);

        WaitLineAddToEnd(ArgsP, newInstruction);

        pthread_cond_broadcast(&condAvailableItemsToControl);
        /*printf("CONTROL: Vlozena nova istrukce c.%d do fronty na pozici %d.\n", imageCount, Args.WaitLineSize);*/

        pthread_mutex_unlock(&mtxInstructionArray);
        /* CRITICAL END - mtxInstructionArray */                

        /* Try to load next image to scan */
        imageToControl = scanner();
    }

    /* Wait for empty array */
    /*pthread_mutex_lock(&mtxInstructionArray);
    if (Args.WaitLineSize > 0) {
        printf("CONTROL: Cekam na prazdnou frontu pro vlozeni instrukci k ukonceni...\n");
        pthread_cond_wait(&condNoItemsInArray, &mtxInstructionArray);
        printf("CONTROL: Fronta prazdna, chystam se vkladat instrukce k ukonceni...\n");

    }
    pthread_mutex_unlock(&mtxInstructionArray);*/


    /* TODO: Notify (insert instructions to kill) workers about end */


    /*printf("CONTROL: Cekam na pristup k instruction array...\n");*/

    /* CRITICAL - mtxInstructionArray */
    pthread_mutex_lock(&mtxInstructionArray);

    newInstructionKill = (TINSTRUCTION *) malloc(sizeof (* newInstructionKill));
    newInstructionKill->id = imageCount++;
    newInstructionKill->itemsToScan = -666; /* devil constant */
    newInstructionKill->imageToScan = NULL;
    newInstructionKill->itemsScanned = 0;
    newInstructionKill->scanResult = 0;
    
    WaitLineAddToEnd(ArgsP, newInstructionKill);

    /*printf("CONTROL: EXIT: Instrukce pro ukonceni vlozena...\n");*/

    pthread_cond_broadcast(&condAvailableItemsToControl);
    pthread_mutex_unlock(&mtxInstructionArray);
    /* CRITICAL END - mtxInstructionArray */

    /* Now wait for threads to join */
    /*printf("Waiting for threads to join...\n");*/
    /* Threads join */
    for (i = 0; i < threads; i++) {
        resultCache = pthread_join(threadArray[i], &dummy);
        if (resultCache) {
            perror("pthread_join() failed. \n");
            exit(EXIT_FAILURE);
        } else {
            /*printf("Joined thread number %d\n", i);*/
        }
    }
    /*printf("All threads are joined, now clean up..\n");*/


    /* Cleanup */

    while (Args.WaitLineSize > 0) {
        WaitLineDeleteFirst(ArgsP);
    }
    if (newInstructionKill != NULL) {
        newInstructionKill = NULL;
    }

    pthread_cond_destroy(&condNoItemsInArray);
    pthread_cond_destroy(&condAvailableItemsToControl);
    /*pthread_cond_destroy(&condFreeSpaceInArray);*/
    pthread_mutex_destroy(&mtxCondNoItemsInArray);
    pthread_mutex_destroy(&mtxCondAvailableItemsToControl);
    /*pthread_mutex_destroy(&mtxCondFreeSpaceInArray);*/
    pthread_mutex_destroy(&mtxInstructionArray);
    free(threadArray);
    threadArray = NULL;

    /* Exit */
    /*printf("OK, I'm done. See ya!\n");*/
    /*pthread_exit(NULL);*/
}

