/*
 * File:   Autoservis.cpp
 * Author: Matej Simek | www.matejsimek.cz
 *
 * Created on 6. květen 2011, 11:56
 */


/*
 * Global definition
 */
#ifndef __PROGTEST__

#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

#define TOOLS_MAX  16

/**
 * Je struktura definující jeden automobil. Jeho složky jsou pole m_ToRepair a m_Internal.
 */
typedef struct TCar {
    /**
     * Představuje pole příznaků, jaké nářadí je třeba použít pro opravu. Nenulová hodnota na indexu i v tomto poli znamená, že k opravě je potřeba nástroj i. K opravě může být potřeba použít více nástrojů než jeden, dokonce nemusí být nastaven žádný (opravu lze zvládnout bez nástrojů).
     */
    char m_ToRepair[TOOLS_MAX];
    /**
     * Jsou další interní data pro implementaci automobilu. Vaše implementace do nich nesmí zasahovat, tedy nepotřebuje znát jejich strukturu.
     */
    char m_Internal[1024];
} TCAR;

/**
 * Debug variable for testing purpose, uncomment each "// if (debug) cout" (progtest compilation failed with uncommented couts)
 */
bool debug = true;

#endif /* __PROGTEST__ */

/**
 * Common struct for shared variables
 */
typedef struct TArguments {
    int (* allocTool) (int);
    int (* releaseTool) (int);
    TCAR * (* getCar) (void);
    int (* repair) (TCAR *);
    void (* dispatch) (TCAR *);

    int * toolStatus;

    pthread_mutex_t * mtxArguments;
    pthread_cond_t * condAvailableTools;
} TARGUMENTS;

/**
 * Auto mechanic worker thread, where all work is done and Cars are healed
 * @param Arguments
 * @return
 */
void * Worker(void * Arguments) {
    // pthread_t tid = pthread_self();
    // if (debug) cout << "[" << tid << "] Worker started." << endl;

    TARGUMENTS * Args = (TARGUMENTS *) Arguments;

    TCAR * car;

    while (1) {

        // Get next car to work on
        // if (debug) cout << "[" << tid << "] Getting next car to work on." << endl;
        car = Args->getCar(); // MUTEX??
        if (car == NULL) {
            break; // No cars to work on
        }

        // if (debug) cout << "[" << tid << "] Have car." << endl;

        // Enter the critical section
        pthread_mutex_lock(Args->mtxArguments);

        bool toolsAreAvailable = false;
        // Wait for available Tools
        while (!toolsAreAvailable) {
            int neededTools = 0;
            int availTools = 0;
            for (int i = 0; i < TOOLS_MAX; i++) {
                // If tool is need for repair
                if (car->m_ToRepair[i]) {
                    neededTools++;
                    // Check its availability
                    if (Args->toolStatus[i]) {
                        availTools++;
                    } else {
                        break;
                    }
                }
            }
            if (availTools == neededTools) toolsAreAvailable = true;
            // Wakeup after tool availability change
            if (!toolsAreAvailable) {
                // if (debug) cout << "[" << tid << "] Tools are not available, Im waiting for them." << endl;
                pthread_cond_wait(Args->condAvailableTools, Args->mtxArguments);
            }
        }

        // if (debug) cout << "[" << tid << "] Allocating necessary tools for car repair." << endl;

        // Alloc necessary tool for car repair and do consistency check
        for (int i = 0; i < TOOLS_MAX; i++) {
            // If tool is need for repair
            if (car->m_ToRepair[i]) {
                // Alloc and test it
                int resultCache = Args->allocTool(i);
                if (resultCache) {
                    Args->toolStatus[i]--;
                } else {
                    // if (debug) cout << "[" << tid << "] Consistency error: Tool " << i << " isnt available but shuld be." << endl;
                }
            }
        }

        // if (debug) cout << "[" << tid << "] Tools are allocated, leaving critical section." << endl;

        // Leave critical section
        pthread_mutex_unlock(Args->mtxArguments);


        // OK, lets do dirty and hard work on car
        // if (debug) cout << "[" << tid << "] Started work on car repair." << endl;
        int repairResult = Args->repair(car);
        if (!repairResult) {
            // if (debug) cout << "[" << tid << "] Car repair wasnt successful." << endl;
        }
        // if (debug) cout << "[" << tid << "] Ended work on car repair." << endl;

        // vrat vsechno naradi (releaseTool) a otestuj uspesnost
        // Enter the critical section
        pthread_mutex_lock(Args->mtxArguments);

        // if (debug) cout << "[" << tid << "] Releasing all alocated tools." << endl;

        // Release all used tools and do consistency check
        for (int i = 0; i < TOOLS_MAX; i++) {
            // If tool is need for repair
            if (car->m_ToRepair[i]) {
                // Release and test it
                int resultCache = Args->releaseTool(i);
                if (resultCache) {
                    Args->toolStatus[i]++;
                } else {
                    // if (debug) cout << "[" << tid << "] Consistency error: Tool " << i << " isnt releasable but shuld be." << endl;
                }
            }

        }
        // if (debug) cout << "[" << tid << "] Tools are released, notyfing others about it." << endl;
        pthread_cond_broadcast(Args->condAvailableTools);
        // Leave critical section
        pthread_mutex_unlock(Args->mtxArguments);

        // Dispatch repaired car to the customer
        // if (debug) cout << "[" << tid << "] Sending repair car back to customer." << endl;
        Args->dispatch(car);

        // And look for other work to do, no breaks :)
    }


    /* This is the end of the workshift, see you tomorrow */
    // if (debug) cout << "[" << tid << "] Worker ended." << endl;
    pthread_exit(NULL);
}

/**
 * Simuluje vlastní chod servisu. Vaším úkolem je její implementace. Funkce vytvoří vlákna pro jednotlivé automechaniky, zajistí jejich synchronizaci a ukončení. Rozhraní funkce je následující:
 * @param staffNr udává počet automechaniků v servisu. Pro každého automechanika musíte připravit vlastní vlákno.
 * @param tools je pole s informací o výbavě servisu. Pole má velikost TOOLS_MAX, každý prvek pole udává kolik kusů nářadí toho kterého typu je v servisu k dispozici (tools[0] udává počet kusů nářadí typu 0, tools[1] udává počet kusů nářadí typu 1, ...).
 * @param allocTool je ukazatel na funkci, kterou se alokuje nářadí. Funkce má parametrem číslo od 0 do TOOLS_MAX-1 udávající typ nářadí, které si automechanik chce rezervovat. Pokud je volání úspěšné, je nářadí rezervováno pro volající vlákno a funkce vrátí nenulovou hodnotu. Pokud je volání neúspěšné (např. všechno nářadí daného typu je již zabrané), funkce vrátí nulu (a rezervaci samozřejmě neprovede).
 * @param releaseTool je ukazatel na funkci, kterou automechanik vrací nářadí. Funkce je komplementární k funkci allocTool. Vrací návratovou hodnotu nenula, pokud se nářadí úspěšně vrátilo nebo 0, pokud volající automechanik neměl dané nářadí rezervované.
 * @param getCar je funkce, kterou si automechanik zažádá o další automobil k opravě. Pokud nějaký automobil čeká na opravu, funkce vrátí ukazatel na jeho popis. Pokud již žádný další automobil nečeká, funkce vrátí hodnotu NULL.
 * @param repair je ukazatel na funkci implementující vlastní opravu automobilu. Implementace vlastní opravy je realizovaná v testovacím prostředí. Úkolem vlákna automechanika je před jejím zavoláním zarezervovat potřebné nářadí. Funkce má parametrem opravovaný automobil. Implementace této funkce kontroluje, zda volající vlákno vlastní všechno potřebné nářadí. Pokud ano, návratovou hodnotou funkce je nenula. Pokud volající vlákno nevlastní potřebné nářadí, funkce vrátí hodnotu 0.
 * @param dispatch je ukazatel na funkci, kterým se opravený automobil vrací zpět zákazníkovi. Testovací prostředí implementuje tuto funkci, předáním opraveného automobilu se uvolní prostředky (alokovaná paměť).
 */
void CarService(int staffNr,
        int * tools,
        int (* allocTool) (int),
        int (* releaseTool) (int),
        TCAR * (* getCar) (void),
        int (* repair) (TCAR *),
        void (* dispatch) (TCAR *)) {

    // if (debug) cout << "Called CarService, staff number: " << staffNr << endl;

    // Prepare thread properties
    pthread_t * threadArray = (pthread_t *) malloc(staffNr * sizeof ( pthread_t));
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
    void *dummy;

    // Prepare mutex and conditional variables
    pthread_mutex_t mtxArguments;
    pthread_mutex_init(&mtxArguments, NULL);

    pthread_cond_t condAvailableTools;
    pthread_cond_init(&condAvailableTools, NULL);

    // Copy initial status of tool array for internal use (availability checking)
    int * toolStatus = (int *) malloc(TOOLS_MAX * sizeof (int));
    for (int i = 0; i < TOOLS_MAX; i++) {
        toolStatus[i] = tools[i];
    }

    // Create shared variables
    TARGUMENTS Args;
    TARGUMENTS * ArgsP;
    Args.allocTool = allocTool;
    Args.releaseTool = releaseTool;
    Args.getCar = getCar;
    Args.repair = repair;
    Args.dispatch = dispatch;
    Args.toolStatus = toolStatus;
    Args.mtxArguments = &mtxArguments;
    Args.condAvailableTools = &condAvailableTools;
    ArgsP = &Args;

    // Start thread workers
    for (int i = 0; i < staffNr; i++) {
        int resultCache = pthread_create(&threadArray[i], &thread_attr, Worker, (void *) ArgsP);
        if (resultCache) {
            // if (debug) cout << "pthread_create() failed." << endl;
            exit(EXIT_FAILURE);
        }
    }

    // Wait for thread workers end
    // if (debug) cout << "Waiting for threads to join..." << endl;
    for (int i = 0; i < staffNr; i++) {
        int resultCache = pthread_join(threadArray[i], &dummy);
        if (resultCache) {
            // if (debug) cout << "pthread_join() failed." << endl;
            exit(EXIT_FAILURE);
        } else {
            // if (debug) cout << "Joined thread number " << i << endl;
        }
    }
    // if (debug) cout << "All threads are joined, now clean up..." << endl;


    // Cleanup
    pthread_cond_destroy(&condAvailableTools);
    pthread_mutex_destroy(&mtxArguments);

    free(toolStatus);
    toolStatus = NULL;

    free(threadArray);
    threadArray = NULL;

    // End
    // if (debug) cout << "OK, I'm done. See ya!" << endl;
}









