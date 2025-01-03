/*
 * assembler.h
 *
 *  Created on: Mar 18, 2024
 *      Author: user
 */

#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "firstPass.h"
#include "macro.h"
#include "secondPass.h"
#include "symbolTable.h"

int createEntFile(SymbolTable *table, char fileName[]);
int createExtFile(SymbolTable *table, char fileName[]);
int createObjFile(char machineCodeIns[][MAX_BITS+1], char file_name[], int inputIC, int inputDC);
int main (int argc, char *argv[]);



#endif /* ASSEMBLER_H_ */
