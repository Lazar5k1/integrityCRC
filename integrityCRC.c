/*
Assignment:
integrityCRC - CRC Algorithm Implementation
Author: Nicholas Gill
Language: C, C++, or Rust (only)
To Compile:
gcc -O2 -std=c11 -o integrityCRC integrityCRC.c
g++ -O2 -std=c++17 -o integrityCRC integrityCRC.cpp
rustc -O integrityCRC.rs -o integrityCRC
To Execute (on Eustis):
./integrityCRC <message_file> <crc_algorithm>
where:
<message_file> is the path to the input text file
<crc_algorithm> is 3, 4, or 8 (for CRC-3, CRC-4, or CRC-8)
Notes:
- Implements CRC-3, CRC-4, and CRC-8 algorithms
- Processes plain text messages and computes CRC values
- Outputs all intermediate steps and final CRC values
- Tested on Eustis.
Class: CIS3360 - Security in Computing - Fall 2025
Instructor: Dr. Jie Lin
Due Date: Friday, October 17, 2025 at 11:55 PM ET
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void processPlainText(FILE *plaintext, char **plainContent, char **message);
void toDecimal(char *message);
void toHex(char *message);
void printBinary(char *message);
void printPad(char *message, int crcArg);
void processCRC(char *message, char **binMessage, int crcArg, char **crcValue);

int main(int argc, char* argv []){
    if(argc != 3){
        printf("reenter command: incorrect number of arguments");
        return 1;
    }
    int crcArg = (int)((*argv[2]) - '0');
    if(crcArg != 3 && crcArg != 4 && crcArg != 8){
        printf("reenter command: crcArg value wrong");
        return 1;
    }
// changes crc argument into int and forcecs correct commands

    FILE *plaintext = fopen(argv[1], "r");
// opens text file

    char *message;
    char *plainContent;
    processPlainText(plaintext, &plainContent, &message);
// yoinks the text from the text file in argv[1], stores it into plainContent, then takes out all the characters that aren't A-Z or a-z and stores that into message


    printf("The original message:\n");
    for(size_t i = 0; i < strlen(plainContent); i++){
        printf("%c", plainContent[i]);
    }
    printf("\nThe preprocessed message (invisible characters removed):\n");
    for(size_t i = 0; i < strlen(message); i++){
        printf("%c", message[i]);
    }
    toDecimal(message);
    toHex(message);
    printf("\n\nThe binary representation of the preprocessed message:\n");
    printBinary(message);
    printPad(message, crcArg);
// prints output
    char *binMessage;
    char *crcValue;
    processCRC(message, &binMessage, crcArg, &crcValue);
// processes message into binary and grabs the remainder from the xor division to return the crc value
    printf("\n\nThe crc value for the chosen crc algorithm in binary:\n");
    for(int i = 0; i < crcArg; i++){
        printf("%c", crcValue[i]);
    }
    
    free(plainContent);
    free(message);
    free(binMessage);
    free(crcValue);
    fclose(plaintext);

    return 0;
}

void processPlainText(FILE *plainText, char **plainContent, char **message){
    fseek(plainText, 0, SEEK_END);
    long plainSize = ftell(plainText);
    rewind(plainText);
// moves cursor to end of file, reads how many bits are in file by reading position (at end of file) to fileSize, then puts cursor back to beginning of file

    *plainContent = malloc(plainSize + 1);
    fread(*plainContent, 1, plainSize, plainText);
    (*plainContent)[plainSize] = '\0';
// allocates file's amount of memory to fileContent for accurate storage, reads 1 byte at a time starting at bytefileSize of file and ending at byte 0 into fileContent
// then appending fileSize with \0 to make it a valid string
    
    *message = malloc(plainSize +1);
    int j = 0;
    for(int i = 0; i < (plainSize + 1); i++){
        if(isalpha((*plainContent)[i]) || isdigit((*plainContent)[i])){
            (*message)[j++] = (*plainContent)[i];
        }
    }
    (*message)[j] = '\0';
// deletes everything except for a-z, A-Z, and 0-9
}

void toDecimal(char *message){
    printf("\n\nThe decimal representation of the preprocessed message:\n");
    for(int i = 0; i < strlen(message); i++){
        printf("%d ", (int)message[i]);
    }
// prints out every character as its decimal ASCII value
}

void toHex(char *message){
    printf("\n\nThe hex representation of the preprocessed message:\n");
    for(int i = 0; i < strlen(message); i++){
        printf("%X ", (int)message[i]);
    }
// prints out every character as its hex ASCII value
}

void printBinary(char *message){
    for(int i = 0; i < strlen(message); i++){
        for(int j =7; j >= 0; j--){
            printf("%u", (message[i] >> j) & 1);
        }
        printf(" ");
    }
// prints out every character as its binary ASCII value
}

void printPad(char *message, int crcArg){
    if(crcArg == 3){
        printf("\n\nThe binary representation of the original message prepared for CRC computation (padded with 3 zeros):\n");
        printBinary(message);
        printf("000");
    }
    else if(crcArg == 4){
        printf("\n\nThe binary representation of the original message prepared for CRC computation (padded with 4 zeros):\n");
        printBinary(message);
        printf("0000");
    }
    else if(crcArg == 8){
        printf("\n\nThe binary representation of the original message prepared for CRC computation (padded with 8 zeros):\n");
        printBinary(message);
        printf("00000000");
    }
// prints out every cahracter as its binary ASCII value and appends the according crc padding
}

void processCRC(char *message, char **binMessage, int crcArg, char **crcValue){
    int binLength = strlen(message) * 8;
    *binMessage = malloc(binLength + crcArg + 1);
// assign memory for binary string
    int bit = 0;
    for(int i = 0; i < strlen(message); i++){
        for(int j =7; j >= 0; j--){
            (*binMessage)[bit++] = ((message[i] >> j) & 1) ? '1' : '0';
// each character gets turned into 8 bits, by shifting the character's byte j bits to the right and appending binMessage with whatever value is at that spot
        }
    }
// message is converted to binary
    for(int i = 0; i < crcArg; i++){
        (*binMessage)[i + (binLength)] = '0';
    }
    (*binMessage)[binLength + crcArg] = '\0';
    binLength = strlen(*binMessage);
// message is now converted to binary and padded with correct number of crc 0's, binLength is updated to new length

    char *polynomial;
    int polyLength;
    if(crcArg == 3){
        *crcValue = malloc(4);
        polynomial = malloc(5);
        strcpy(polynomial, "1101");
        polyLength = 4;
    }
    else if(crcArg == 4){
        *crcValue = malloc(5);
        polynomial = malloc(6);
        strcpy(polynomial, "10110");
        polyLength = 5;
    }
    else if(crcArg == 8){
        *crcValue = malloc(9);
        polynomial = malloc(10);
        strcpy(polynomial, "100110101");
        polyLength = 9;
    }
// assigns correct polynomial for crc argument and assigns right amount of memory to crcvalue
    
    char *tempBin = malloc(binLength + 1);
    strcpy(tempBin, *binMessage);

    for(int i = 0; i <= binLength - polyLength; i++){
// divides up to binLength - polyLength so there isn't overflow
        if(tempBin[i] == '1'){
            for(int j = 0; j < polyLength; j++){
                if(tempBin[i + j] == polynomial[j]){
                    tempBin[i + j] = '0';
                }
                else tempBin[i + j] = '1';
            }
        }
// if there is a 1 at current position compare following bits to polynomial, if they're the same change to zero if not change to 1
    }
// does xor division on temporary binary message

    for(int i = 0; i < crcArg; i++){
        (*crcValue)[i] = tempBin[(binLength - crcArg) + i];
    }
    (*crcValue)[crcArg] = '\0';
// crc value gets the remainder of the xor division
    free(polynomial);
    free(tempBin);
}