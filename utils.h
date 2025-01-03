/*
 * utils.h
 *
 *  Created on: Mar 19, 2024
 *      Author: user
 */

#ifndef UTILS_H_
#define UTILS_H_

#define NUM_INSTRUCTIONS 16
#define MAX_LINE 82
#define MAX_MACRO 500
#define MAX_LABEL 32
#define MAX_MACHINE_CODE_LINES 100
#define MAX_BITS 14
#define MAX_SYMBOL_TYPE 10

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

int isAlphabet(char c);
int isLabel(char word[]);
char lastChar (char str[]);
int isNum(char c);
int isStrNum(char word[]);
int isAlphabetOrNum (char word[], int checkLastChar);
int length(char word[]);
int indxNWord(char *line, int n);
int getNWord (char *word, char *line, int n);
int getNWordStr (char *word, char *line, int n);
void intToBinaryStr(char word[], int value);
int findIndex(char  instructions[][4], int numInstructions, char word[]);




#endif /* UTILS_H_ */
