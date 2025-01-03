/*
 * SymbolTable.h
 *
 *  Created on: Apr 14, 2024
 *      Author: user
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <stdio.h>
#include <string.h>
#include "utils.h"
//#define MAX_LINE 82

typedef struct SymbolTable{
	char name[MAX_LINE];
	int value;
	char type[MAX_SYMBOL_TYPE];
	struct SymbolTable* next;
}SymbolTable;


SymbolTable *addDefine(SymbolTable *table, char *line, int lineNumber);
SymbolTable *addSymbol (SymbolTable *table, char name[], int value, char type[]);
SymbolTable *isSymbol (char *name, char *type, SymbolTable *table);
SymbolTable *addLabel(SymbolTable *table, char *name, char *type, int value, int lineNumber);
SymbolTable *symbolExists (SymbolTable *table, char *name);

#endif /* SYMBOLTABLE_H_ */
