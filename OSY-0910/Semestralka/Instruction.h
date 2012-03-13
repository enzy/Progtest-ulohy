/* 
 * File:   Instruction.h
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 18. duben 2010, 23:47
 */

#include "globals.h"

#ifndef _INSTRUCTION_H
#define	_INSTRUCTION_H

class Instruction {
public:
    int cpuCycles;
    int ioCycles;
    int sleepCycles;
    int callThread;
    int threadID;
    int blockID;    

    /**
     * Vytvoreni nove instrukce
     *
     * @param parentID threadID
     * @param bloID block ID
     * @param cpu cpuCycles
     * @param io ioCycles
     * @param slp sleepCycles
     * @param thrd callThread     
     */
    Instruction(int parentID, int bloID, int cpu, int io, int slp, int thrd) {        
        threadID = parentID;
        blockID = bloID;
        cpuCycles = cpu;
        ioCycles = io;
        sleepCycles = slp;
        callThread = thrd;        
    };

    /**
     * Vraci, zdali ma instrukce co napraci
     *
     * @return bool
     */
    bool hasWork() {
        return cpuCycles + ioCycles + sleepCycles + callThread;
    }

    /**
     * Provede jeden cykl instrukce
     *     
     */
    void doWork() {
        if (cpuCycles) {
            cpuCycles--;
            cout << "Vlakno " << threadID << " [" << blockID << "] zahajuje cpu. Zbyva dob " << cpuCycles << "." << endl;
            // Simulace prace vlakna
            delay(KLIK);
            cpuIterations++;
            cout << "Vlakno " << threadID << " [" << blockID << "] ukoncilo cpu." << endl;
        }
        if (ioCycles) {
            ioCycles--;
            cout << "Vlakno " << threadID << " [" << blockID << "] zahajuje V/V. Zbyva dob " << ioCycles << "." << endl;
            // Simulace prace vlakna
            delay(KLIK);
            ioIterations++;
            cout << "Vlakno " << threadID << " [" << blockID << "] ukoncilo V/V." << endl;
        }
        if (sleepCycles) {
            sleepCycles--;
            cout << "Vlakno " << threadID << " [" << blockID << "] zahajuje sleep. Zbyva dob " << sleepCycles << "." << endl;
            sleepIterations++;
            cout << "Vlakno " << threadID << " [" << blockID << "] ukoncilo sleep." << endl;
        }
        if (callThread) {
            cout << "Vlakno " << threadID << " [" << blockID << "] vytvorilo novy thread " << callThread << " [<cislo_bloku_instrukci_noveho_threadu>]." << endl;
            createIterations++;
            // TODO Vytvorit a zavolat vlakno s instrukcemi
            callThread = 0;
        }
        if(!(cpuCycles + ioCycles + sleepCycles + callThread)){
            cout << "Vlakno " << threadID << " [" << blockID << "] dobehlo." << endl;
            // TODO Ukoncil vlakno, nejspis presunout nekam jinam, spis do obshluhy vlakna
        }
    }
};

#endif	/* _INSTRUCTION_H */

