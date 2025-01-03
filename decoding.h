/*
 * decoding.h
 *
 *  Created on: Mar 19, 2024
 *      Author: user
 */

#ifndef DECODING_H_
#define DECODING_H_
#define MAX_DATA_ARGS 40
#define MAX_EXTERN_ARGS 40
#define MAX_INC_ARGS 2
#define NUM_OF_OPS 16
#include "utils.h"
#include "symbolTable.h"

int decodingData(SymbolTable *table, int DC, char machineCodeData[][MAX_BITS+1], char line[], int lineNumber, int currWord);
int decodingStr( int DC, char machineCodeData[][MAX_BITS+1], char line[], int lineNumber, int currWord);
SymbolTable *decodingExtern(SymbolTable *table, char line[], int lineNumber, int currWord, char firstWord[]);
int createBinaryForInstruction(SymbolTable *table, char line[], int lineNumber, char firstWord[], char machineCodeIns[][MAX_BITS+1], int IC, int currWord);

#endif /* DECODING_H_ */
