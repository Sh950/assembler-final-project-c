/*
 * symbolTable.c
 *
 *  Created on: Apr 14, 2024
 *      Author: user
 */


#include "symbolTable.h"

//validate "define" definition and call to addSymbol to add a symbol to the the table point by table argument, return point to the new symbol in the table
SymbolTable *addDefine(SymbolTable *table, char *line, int lineNumber)
{
	char temp[MAX_LINE];
	memset (temp, '\0', MAX_LINE);
	char name[MAX_LINE];
	memset (name, '\0', MAX_LINE);
	int value;

	int getWordReturn = getNWord(name, line, 1);
	if (!getWordReturn) //no name for define
	{
		printf("%s%d%s\n", "Error; illegal define in line: ", lineNumber, ".\nNo name of define.");
		return NULL;
	}

	if(isSymbol(name, "mdefine", table) != NULL)// the define name already exists
	{
		printf("%s%d%s\n", "Error; illegal define in line: ", lineNumber, " .\nThe name of define is exist.");
		return NULL;
	}

	if (!getNWord(temp, line, 2) || strcmp(temp, "=") != 0) //no value for the operand in the define statement
	{
		printf("%s%d%s\n", "Error; illegal define in line: ", lineNumber, ".\nNo '=' operand in the define statement.");
		return NULL;
	}

	memset (temp, '\0', MAX_LINE);
	getNWord(temp, line, 3); //get the value part of the define
	if(isStrNum(temp))
		value = atoi(temp); //convert to int
	else
	{
		printf("%s %d %s\n", "Error; illegal define in line:", lineNumber, ".\nilligal value.");
		return NULL;
	}

	return addSymbol(table, name, value,  "mdefine"); // add the symbol to the table
}

//add a symbol to the the table point by table argument, return point to the new symbol in the table
SymbolTable *addSymbol (SymbolTable *table, char name[], int value, char type[])
{

	SymbolTable *ptr = (SymbolTable *)malloc(sizeof(SymbolTable)); //new symbol in the table
	memset (ptr->name, '\0', MAX_LINE);
	strcpy (ptr->name, name);
	ptr->value = value;
	memset (ptr->type, '\0', MAX_SYMBOL_TYPE);
	strcpy (ptr->type, type);
	ptr->next = table;
	return ptr;
}

//Verify is symbol exists in table and return it, otherwise return NULL
SymbolTable *isSymbol (char *name, char *type, SymbolTable *table)
{
	SymbolTable *tablePtr = table;

	while (tablePtr != NULL)
	{
		if (strcmp(tablePtr->name, name)== 0 && strcmp(tablePtr->type, type)== 0 )//found Symbol with the same name and type
			return tablePtr;
		tablePtr = tablePtr->next;
	}

	return NULL;
}

// add a label to the symbol table, if the label exists already return NULL.
SymbolTable *addLabel(SymbolTable *table, char *name, char *type, int value, int lineNumber)
{
	if (isSymbol (name, "code", table) != NULL || isSymbol (name, "data", table) != NULL) //found label with the same name
	{
		printf("%s %d %s\n", "Error!\nThe label name in the line: ", lineNumber, " already exist.");
		return NULL;
	}
	return addSymbol(table, name, value, type); // add the label to the table
}



//return the symbol from the table if it exists, otherwise retun NULL
SymbolTable *symbolExists (SymbolTable *table, char *name)
{
	SymbolTable *tablePtr = table;

	while (tablePtr != NULL)
	{
		if (strcmp(tablePtr->name, name)== 0 )//found Symbol with the same name
			return tablePtr;
		tablePtr = tablePtr->next;
	}
	return NULL;
}

