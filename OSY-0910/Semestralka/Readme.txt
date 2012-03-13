
Simulace plánování vláken (OSY - semestrální práce)
Author: Matěj Šimek - www.matejsimek.cz

Deadline: 2.05.2010 23:59:59

Popis souborů:
globals.h - globální nastavení a includy
main.cpp - hlavní běh programu a simulace
Instruction.h - třída instrukce a její metody
Vlakno.h - třída vlákna a její metody

Popis řešení:
1. načtení bloků instrukcí
- vstupní formát: ID:Priorita:Instrukce1,Instrukce2,Instrukce3,...
- ukončení vstupu prázdným řádkem
- proměnná debug nastavená na true provede vložení a spuštění testovacích dat

2. vytvoření fronty pro jednotlivá vlákna a její naplnění (po řádcích)

3. spuštění a střídání vláken po sobě
- každé vlákno je spuštěno na počet iterací rovající se násobku jeho priority
- poté přijde na řadu další vlákno ve frontě
- tento proces se opakuje, dokud každé vlákno má co na práci - instrukce, které 
  vykonávat
- simulace práce vlákna pomocí čekání jedno kvantum času, určený počet ms v
  globální proměnné KLIK, pro simulaci slouží funkce delay(ms)
  
4. doběhnutí všech instrukcí

5. výpis statistiky


Popis třídy Vlakno:
- ma svoje ID, prioritu a frontu intrukci (reprezentovane tridou Instruction)
- pri vytvoreni se parsuje string s blokem instrukci instructionsToParse, tuto
  funkci zastava metoda parseInstructions(), ktera nacte nejdrive id, prioritu
  a pote postupne plni frontu instrukcemi, ktere ziska ze zpracovani retezce
- pro plneni fronty slouzi metoda addInstruction()
- metoda hasWork() vraci, zda-li ma co napraci, tedy instrukce, ktere vykonavat
- metoda doWork() simuluje praci vlakna provedenim jedne instrukce z fronty
  pokud zpracovavana insrukce bude prazda, z fronty se odstrani
  
Popis třídy Instruction:
- cpuCycles, ioCycles, sleepCycles, callThread slouzi pro simulaci poctu iteraci
  zadane insrtukce
- threadID je ID vlakna, ktere ho vytvari
- blockID je ID bloku instrukci v konkretnim vlakne
- metoda hasWork() vraci, zda-li ma co napraci, tedy instrukce, ktere vykonavat     
- metoda doWork() simuluje praci instrukce

Co jsem nestihl:
- zpracovani instrukce pro vytvoreni noveho vlakna
- realnejsi simulaci za pomoci vlaken, nynejsi reseni je schopno vykonavat pouze
  jednu operaci - bud cpu, io nebo sleep. Nezbyl cas na implementaci. 