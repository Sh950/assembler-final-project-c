/*
 * firstPass.c
 *
 *  Created on: Mar 18, 2024
 *      Author: user
*/

#include "firstPass.h"


//Update all data values in the symbol table with IC+100
void updateSymbolTableData(int IC, SymbolTable *table)
{
	while (table != NULL)
	{
		if (strcmp(table->type, ".data")== 0 )//found Symbol of type data
			table->value = table->value+100+IC;
		table = table->next;
	}
}

// First pass as defined in the instructions of the project.
SymbolTable *firstPass (FILE *source_file_p, char machineCodeIns[][MAX_BITS+1], char machineCodeData[][MAX_BITS+1], SymbolTable *table, int *inputIC, int *inputDC)
{
	int IC = 0, DC = 0; // counters for instruction and data
	int L = 0; // number of words that the machine code holds for an instruction

	char line[MAX_LINE]; //the instruction line
	memset (line, '\0', MAX_LINE);
	//!!! maybe change name to currStrWord
	char firstWord[MAX_LINE];


	SymbolTable *temp;

	int lineNumber = 0;
	int errors = 0;
	int warnings = 0;

	int labelFlag = 0;
	int currWord = 0;
	char labelName[MAX_LABEL];
	int updatedCtr;

	while (fgets (line, MAX_LINE, source_file_p) != NULL)// read line
	{
		memset (firstWord, '\0', MAX_LINE);
		currWord = 0; // words that has read from the line
		lineNumber ++;
		if (!labelFlag)
			memset(labelName, '\0', MAX_LABEL);

		if(!getNWord(firstWord, line, 0) || firstWord[0] == ';')//only white spaces or comment line
			continue;
		currWord ++;

		if (strcmp(firstWord, ".define")== 0) //is a define line
		{
			if (labelFlag)
			{
				printf("%s%d%s\n", "Error!\nIllegal label before define instruction in line: ", lineNumber, ".");
				errors ++;
				labelFlag = 0;
			}

			temp = addDefine(table, line, lineNumber); //create the define symbol for store in the symbol table
			if (temp == NULL)
					errors ++;
			else
				table = temp; //add the define symbol to the table

			continue;
		}

		if (isLabel(firstWord)==1) // the line begin with a label
		{
			if (labelFlag)//this line is in label
			{
				printf("%s%d%s\n", "Error!\nDouble labels in line: ", lineNumber, ".");
				errors ++;
			}

			labelFlag = 1;
			// need to be this way anyway
			if(lastChar(firstWord) == ':')
			{
				//remove the ':' from the name
				firstWord[length(firstWord)-1] = '\0';
			}
			strcpy (labelName, firstWord); //keep the label name
			memset (firstWord, '\0', MAX_LINE);
			if(!getNWord(firstWord, line, currWord))//Get the next word in the line to firstWord, if no more words in this line continue
				continue;
			currWord ++;
		}

		if (strcmp(firstWord, ".data") == 0 || strcmp(firstWord, ".string") == 0)// instruction to store data
		{
			if(labelFlag)
			{
				temp = addLabel(table, labelName, ".data", DC, lineNumber);
				labelFlag = 0;
				if (temp == NULL)
					errors ++;
				else
					table = temp;
			}

			if(strcmp(firstWord, ".data") == 0 )
			{
				if (!(updatedCtr = decodingData(table, DC, machineCodeData, line, lineNumber, currWord)))
					errors ++;
				else
					DC = updatedCtr;
				continue;
			}
			else // firstWord == ".string"
			{
				if (!(updatedCtr = decodingStr(DC, machineCodeData, line, lineNumber, currWord)))
					errors ++;
				else
					DC = updatedCtr;
				continue;
			}
		}
		if (strcmp(firstWord, ".extern") == 0 || strcmp(firstWord, ".entry") == 0)
		{
			if(labelFlag)
			{
				printf("%s%d%s\n", "Warning!\nIgnoring label before external/entry instruction in line: ", lineNumber, ".");
				warnings ++;
				labelFlag = 0;
			}
			if(strcmp(firstWord, ".entry") == 0)
				continue;
			table = decodingExtern(table, line, lineNumber, currWord, firstWord);
			if(table == NULL)
				errors++;
			continue;
		}
		//if we got here it's a .code line
		if (labelFlag)
		{
			temp = addLabel(table, labelName, ".code", IC+100, lineNumber);
			labelFlag = 0;
			if (temp == NULL)
				errors ++;
			else
				table = temp;
		}

		int tempIC = createBinaryForInstruction(table, line, lineNumber, firstWord, machineCodeIns, IC, currWord);
		if(tempIC == -1)
		{
			errors++;
		}
		else
		{
			IC = tempIC;
		}
		memset (line, '\0', MAX_LINE);
	}


	if (labelFlag)
	{
		printf("%s%d%s\n", "Error!\nIllegal instruction: label without instruction in line ", lineNumber, ".");
		errors ++;
	}

	updateSymbolTableData(IC, table);

	if (errors > 0 )
		return NULL;

	*inputIC = IC;
	*inputDC = DC;

	//copy data bytes to the end of ins bytes
	int localDC = 0;
	while (localDC < DC)
	{
		strcpy(machineCodeIns[IC+localDC], machineCodeData[localDC]);
		localDC++;
	}

	return table;
}

