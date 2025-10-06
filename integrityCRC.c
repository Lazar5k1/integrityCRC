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
void toDecimal(char *message, int **decMessage);

int main(int argc, char* argv []){
    if(argc != 3){
        printf("reenter command");
        return 1;
    }

    FILE *plaintext = fopen(argv[1], "r");

    char *message;
    char *plainContent;
    processPlainText(plaintext, &plainContent, &message);
// yoinks the text from the text file in argv[1], stores it into plainContent, then takes out all the characters that aren't A-Z or a-z and stores that into message

    int *decMessage;
    toDecimal(message, &decMessage);

// start printing
    printf("The original message:\n");
    for(size_t i = 0; i < strlen(plainContent); i++){
        printf("%c", plainContent[i]);
    }
    printf("\nThe preprocessed message (invisible characters removed):\n");
    for(size_t i = 0; i < strlen(message); i++){
        printf("%c", message[i]);
    }
    printf("\n\nThe decimal representation of the preprocessed message:\n");
    for(size_t i = 0; i < strlen(message) - 1; i++){
        printf("%d ", decMessage[i]);
    }
    printf("\n\nThe hex representation of the preprocessed message:\n");
    for(size_t i = 0; i < strlen(message) - 1; i++){
        printf("%d ", decMessage[i]);
    }

    
    free(plainContent);
    free(message);
    free(decMessage);
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
// originally I returned plainContent to main as well but it was unnecessary for the output in the end. But I'm keeping it like this because it works and I don't feel like changing it
    
    int j = 0;
    for(int i = 0; i < (plainSize + 1); i++){
        if((isalpha((*plainContent)[i]))){
            (*message)[j++] = (*plainContent)[i];
        }
    }
    (*message)[j] = '\0';
// capitalizes all a-z characters and deletes everything else
}

void toDecimal(char *message, int **decMessage){
    *decMessage = malloc((strlen(message) - 1) * sizeof(int));
    for(size_t i = 0; i < (strlen(message) - 1); i++){
        (*decMessage)[i] = (int)message[i];
    }
}