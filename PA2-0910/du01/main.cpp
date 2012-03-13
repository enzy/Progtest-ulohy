/* 
 * File:   Komprimace RLE+
 * Author: Matěj Šimek - www.matejsimek.cz
 *
 * Created on 20. březen 2010, 17:40
 *

   Zadani:

   Blok opakujících se bajtů je při komprimaci kódován jako 2 bajty:
           <pocet_opakovani | 0x80> <opakujici_se_bajt>

   Tedy například 10 (=0xA) po sobě jdoucích bajtů o hodnotě 32 (=0x20, mezera)
   bude na výstup uloženo jako dva bajty s hodnotami 0x8A 0x20. Nejvyšší bit
   tedy signalizuje, že dochází k opakování a nižších 7 bitů udává počet opakování.
   Takto lze zaznamenat max. 127 opakování, delší sekvence (např. 200 opakování)
   lze zapsat na více dvojic bajtů na výstupu (např. 0xff 0x20 0xc9 0x20).

   Blok neopakujících se n bajtů je při komprimaci kódován na výstupu jako n+1
   bajtů, kde první bajt udává délku kopírovaného úseku (počet bajtů). Tedy
   například 5 bajtů na vstupu o hodnotách 0x20 0x21 0x22 0x23 0x24 budou na
   výstupu zakódováno jako 6 bajtů 0x05 0x20 0x21 0x22 0x23 0x24. Hodnota 0x05
   signalizuje, že následujících 5 bajtů bylo kopírováno (nejvyšší bit je nulový).
   Opět, takto lze kódovat pouze do počtu 127 bajtů, delší úseky je potřeba rozdělit.

 */
#ifndef __PROGTEST__
    #include <stdlib.h>
    #include <iostream>
    #include <iomanip>
    #include <fstream>
#endif /* __PROGTEST__ */

#define INPUT_BUFFER 2048
#define OUTPUT_BUFFER 2048


bool debug = true;
bool local = true;

bool compressRLEPlus(const char * inFileName, const char * outFileName);
bool decompressRLEPlus(const char * inFileName, const char * outFileName);

void print_error(int ln);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

bool compressRLEPlus(const char * inFileName, const char * outFileName) {

    // otevreni vstupniho a vystupniho souboru pro cteni a pro zapis, binarne

    FILE * inputFile;
    FILE * outputFile;

    inputFile = fopen(inFileName, "rb");
    if (!inputFile) perror ("Error opening file");//print_error(__LINE__);

    outputFile = fopen(outFileName, "wb");
    if (!outputFile) perror ("Error opening file");//print_error(__LINE__);

    // spocitej velikost souboru v bajtech
    fseek(inputFile, 0, SEEK_END);
    unsigned long inputFileSize = ftell(inputFile);
    rewind(inputFile);

    // vstupni a vystupni buffer
    unsigned char inBuff[128];
    unsigned char outBuff[128];

    unsigned char n;
    unsigned char n1;
    unsigned char n2;

    // Nacteni casti vstupniho souboru do bufferu
    unsigned long readLenght = fread(inBuff, 3, 1, inputFile);
    if (!readLenght && !feof(inputFile)) perror ("Error reading file");//print_error(__LINE__);

    if(readLenght>=1) n = inBuff[0];
    if(readLenght>=2) n1 = inBuff[1];
    if(readLenght>=3) n2 = inBuff[2];

    // pruchody souborem
    for (unsigned long i = readLenght-1 /* 2 */; i < inputFileSize; i++) {


        // Pokud jsou za sebou 3 opakujici se znaky
        if(n==n1 && n1==n2){
            unsigned char repeatElement = n;
            unsigned char repeatCount = 3;

            // hledej dalsi
            n = n2;
            readLenght = fread(inBuff, 1, 1, inputFile);
            if (!readLenght && !feof(inputFile)) perror ("Error reading file");
            i++;
            n1 = inBuff[0];

            while(n==n1 && i < inputFileSize && repeatCount < 127){
                repeatCount++;
                n = n1;
                readLenght = fread(inBuff, 1, 1, inputFile);
                if (!readLenght && !feof(inputFile)) perror ("Error reading file");
                i++;
                n1 = inBuff[0];
            }

            outBuff[0] = 128 + repeatCount;
            outBuff[1] = repeatElement;

            unsigned long writedLenght = fwrite(outBuff, 1, 2, outputFile);
            if(ferror(outputFile)) perror("Error writing file");
            if (!writedLenght) print_error(__LINE__);
        }

        // Pokud opakujicise znaky nejsou
        else if(i < inputFileSize){
            
            unsigned char nonRepeatCount = 2;
            
            while((n!=n1 || n1!=n2) && i < inputFileSize && nonRepeatCount < 127){
                nonRepeatCount++;
                outBuff[nonRepeatCount-2] = n;
                outBuff[nonRepeatCount-1] = n1;
                outBuff[nonRepeatCount] = n2;
                n = n1;
                n1 = n2;
                readLenght = fread(inBuff, 1, 1, inputFile);
                if (!readLenght && !feof(inputFile)) perror ("Error reading file");
                i++;
                n2 = inBuff[0];                                
            }
            
            outBuff[0] = 0x7F & nonRepeatCount - 2;
            
            // Zapsani obsahu vystupniho bufferu do souboru
            unsigned long writedLenght = fwrite(outBuff, 1, nonRepeatCount, outputFile);
            if(ferror(outputFile)) perror("Error writing file");
            if (!writedLenght) print_error(__LINE__);


        }
                                                          
    }


    // Uzavreni otevrenych souboru
    fclose(inputFile);
    fclose(outputFile);

    return true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

bool decompressRLEPlus(const char * inFileName, const char * outFileName) {

    FILE * inputFile;
    FILE * outputFile;

    inputFile = fopen(inFileName, "rb");
    if (!inputFile) perror ("Error opening file");//print_error(__LINE__);

    outputFile = fopen(outFileName, "wb");
    if (!outputFile) perror ("Error opening file");//print_error(__LINE__);

    // spocitej velikost souboru v bajtech
    fseek(inputFile, 0, SEEK_END);
    unsigned long inputFileSize = ftell(inputFile);
    rewind(inputFile);

    // vstupni a vystupni buffer
    unsigned char inBuff[INPUT_BUFFER];
    unsigned char outBuff[OUTPUT_BUFFER];

    // pruchody souborem
    for (unsigned long i = 0; i < inputFileSize; i++) {

        // Načtete bajt X na vstupu.
        unsigned long readLenght = fread(inBuff, 1, 1, inputFile);
        if (!readLenght) perror ("Error reading file");//print_error(__LINE__);

        //printf("Test kontrolniho znaku: %X\n", inBuff[0]);

        if(feof(inputFile)) break;

        // Pokud je v X nejvyšší bit nastaven, pak načtete následující bajt Y a zopakujete jej na výstupu tolikrát, kolik udávají nižší bity X.
        if ( (inBuff[0] & 128) - 128 == 0) {
            unsigned char repeatCount = inBuff[0] & 127;
            //printf("%d opakujicich se\n",repeatCount);
            unsigned long readLenght = fread(inBuff, 1, 1, inputFile);
            if (!readLenght && !feof(inputFile)) perror ("Error reading file");//print_error(__LINE__);//break;

            for (int j = 0; j < repeatCount; j++) {
                outBuff[j] = inBuff[0];
            }

            // Zapsani obsahu vystupniho bufferu do souboru
            unsigned long writedLenght = fwrite(outBuff, 1, repeatCount, outputFile);
            if(ferror(outputFile)) perror("Error writing file");
            if (repeatCount && !writedLenght) {
                //printf("writedLenght %d, repeatCount %d\n",writedLenght, repeatCount);
                print_error(__LINE__);
            }

            i++;

        }// Pokud je v X nejvyšší bit nulový, pak na výstup zkopírujete následujících X bajtů.
        else {
            unsigned char repeatCount = inBuff[0]; // & 127;
            //printf("%d neopakujicich se\n", repeatCount);                        

            unsigned long readLenght = fread(inBuff, 1, repeatCount, inputFile);
            if (repeatCount && !readLenght && !feof(inputFile)) {
                perror("Error reading file");
                //printf("repeatCount %d, readLenght %d\n",repeatCount, readLenght);
                //print_error(__LINE__);
            }

            for (unsigned char j = 0; j < repeatCount && j < readLenght; j++) {
                outBuff[j] = inBuff[j];
            }

            // Zapsani obsahu vystupniho bufferu do souboru
            unsigned long writedLenght = fwrite(outBuff, 1, readLenght, outputFile);
            if (ferror(outputFile)) perror("Error writing file");
            if (readLenght && !writedLenght) print_error(__LINE__);

            i += repeatCount;
        }


    }

    // Uzavreni otevrenych souboru
    fclose(inputFile);
    fclose(outputFile);

    return true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef __PROGTEST__

int main(int argc, char** argv) {

    // Návod: stačí aby váš program bral:
    // co komprimovat: argv[1] kam komprimovat: argv[2]
    // co dekomprimovat: argv[3] kam dekomprimovat: argv[4]

    if (debug && !local) {
        compressRLEPlus(argv[1], argv[2]);
        decompressRLEPlus(argv[3], argv[4]);
    }

    if (debug && local) {
        printf("Komprese raw_1.bin ...\n");
        compressRLEPlus("raw_1.bin", "test_compressed_1.bin");
        printf("Dekomprese compressed_1.bin ...\n");
        decompressRLEPlus("compressed_1.bin", "test_raw_1.bin");


        printf("Komprese raw_2.bin ...\n");
        compressRLEPlus("raw_2.bin", "test_compressed_2.bin");
        printf("Dekomprese compressed_2.bin ...\n");
        decompressRLEPlus("compressed_2.bin", "test_raw_2.bin");

        printf("Komprese raw_3.bin ...\n");
        compressRLEPlus("raw_3.bin", "test_compressed_3.bin");
        printf("Dekomprese compressed_3.bin ...\n");
        decompressRLEPlus("compressed_3.bin", "test_raw_3.bin");

        printf("Komprese sample.mp3 ...\n");
        compressRLEPlus("sample.mp3", "compressed_sample.mp3");
        printf("Dekomprese compressed_sample.mp3 ...\n");
        decompressRLEPlus("compressed_sample.mp3", "test_sample.mp3");
    }

    return (EXIT_SUCCESS);
}
#endif /* __PROGTEST__ */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void print_error(int ln) {
    if (debug) printf("Nespravny vstup. [%d])\n", ln);
    else printf("Nespravny vstup.\n");

    exit(EXIT_FAILURE);
}
