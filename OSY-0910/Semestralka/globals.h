/* 
 * File:   globals.h - Globální konstanty a includy pro projekt
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 18. duben 2010, 23:47
 */

#ifndef _GLOBALS_H
#define	_GLOBALS_H

#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <string>
#include <list>

using namespace std;

#define OR ||
#define AND &&

// Jednotlive prefixy instrukci
#define ICPU "cpu"
#define IIO "io"
#define ICREATE "create"
#define ISLEEP "sleep"

// Nastaveni oddelovacu
#define SEPARATOR ':'
#define INSTRUCTION_SEPARATOR ','

// Nastaveni casoveho kvanta
#define KLIK 100 // casove kvantum v ms

/* Globalni countery prace cpu, io a spanku */
int cpuIterations = 0;
int ioIterations = 0;
int sleepIterations = 0;
int createIterations = 0;

/* Jednotlive radky vstupu pro zpracovani */
list<string> inputInstructionDefenitionBlocks;


/* Simulace prace vlakna */
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

  #include <windows.h>

  inline void delay( unsigned long ms ){
    Sleep( ms );
   }

#else  /* presume POSIX */

  #include <unistd.h>

  inline void delay( unsigned long ms ){
    usleep( ms * 1000 );
  }

#endif
 /* --- */

#endif	/* _GLOBALS_H */

