/* 
 * File:   Vlakno.h
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 18. duben 2010, 23:47
 */

#include "Instruction.h"

#ifndef _VLAKNO_H
#define	_VLAKNO_H

class Vlakno {
public:
    int id;
    int priority;
    list<Instruction> instructions;
    list<Instruction>::iterator it;

    /**
     * Vytvor nove vlakno s intrukcemi
     *
     * @param instructionsToParse string s instrukcemi     
     */
    Vlakno(string instructionsToParse) {
        parseInstructions(instructionsToParse);
        it = instructions.begin();
    };

    /**
     * Pridej instrukci do fronty
     *
     * @param inst     
     */
    void addInstruction(Instruction * inst) {
        instructions.push_back(*inst);
    }

    /**
     * Parsovani radku s intrukcemi
     *
     * @param instructionsToParse string s instrukcemi
     */
    void parseInstructions(string instructionsToParse) {
        /*
         Format vstupu:
          1:5:cpu 6,io-12,cpu-10,io-27,create-2,cpu-1,create-3,create-3
          2:6:cpu-50,create-4,cpu-50,sleep-4,cpu-30
          3:5:cpu-1,io-70
          4:5:cpu-11,io-8,cpu-20,create-3
         */

        // Nacteni id a priority procesu
        char tmp1, tmp2;
        stringstream ss(stringstream::in | stringstream::out);
        ss << instructionsToParse;

        ss >> id >> tmp1 >> priority >> tmp2;
        // Kontrola
        if (tmp1 != SEPARATOR || tmp2 != SEPARATOR) {
            cout << "Chyba vstupu" << endl;
            exit(EXIT_FAILURE);
        }

        // Debug vypis
        //cout << "# Vytvoreno vlakno s id: " << id << " s prioritou: " << priority << endl;

        // Nacteni instrukci
        int position = instructionsToParse.find_last_of(":") + 1;
        int positon2 = position;

        while (position < instructionsToParse.size() - 1 && position > 0 && position) {

            string instructionName;
            int instructionParameter;

            // Najdi jmeno instrukce
            positon2 = instructionsToParse.find_first_of(" -", position);
            instructionName = instructionsToParse.substr(position, positon2 - position);

            // Najdi parametr instrukce
            position = positon2 + 1;
            positon2 = instructionsToParse.find_first_of(",", position);

            if (positon2 == 0) positon2 = instructionsToParse.size() - 1;

            istringstream is(instructionsToParse.substr(position, positon2 - position));
            is >> instructionParameter;

            position = positon2 + 1;

            // Debug vypis
            // cout << "Nalezena isntrukce: " << instructionName << " s parametrem: " << instructionParameter << endl;

            // Rozpoznani typu instrukce a nasledne pridani do fronty instrukci
            if (instructionName.find(ICPU) != string::npos) {
                addInstruction(new Instruction(id, instructions.size(), instructionParameter, 0, 0, 0));
            } else if (instructionName.find(IIO) != string::npos) {
                addInstruction(new Instruction(id, instructions.size(), 0, instructionParameter, 0, 0));
            } else if (instructionName.find(ISLEEP) != string::npos) {
                addInstruction(new Instruction(id, instructions.size(), 0, 0, instructionParameter, 0));
            } else if (instructionName.find(ICREATE) != string::npos) {
                addInstruction(new Instruction(id, instructions.size(), 0, 0, 0, instructionParameter));
            }

        }

    }

    /**
     * Vraci, zdali ma vlakno co napraci, tedy aktivni instrukce
     *
     * @return bool     
     */
    bool hasWork() {
        return instructions.size();
    }

    /**
     * Simulace prace vlakna, provedeni jedne instrukce z fronty
     *     
     */
    void doWork() {
        // spust dalsi instrukci v seznamu        
        if (it->hasWork()) {
            it->doWork();
            it++;
        } else {
            it = instructions.erase(it);
        }
        if (it == instructions.end()) it = instructions.begin();
    }


};

#endif	/* _VLAKNO_H */

