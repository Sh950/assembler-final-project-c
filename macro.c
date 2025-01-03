/*
 * macro.c
 *
 *  Created on: Mar 18, 2024
 *      Author: user
 */

#include "macro.h"


//
Macro *addMacro (char *line, Macro *macroTable)
{
	Macro *ptr = (Macro *)malloc(sizeof(Macro));

	if (!getNWord(ptr -> name, line, 1))
	{
		printf("Error!\nmcr expression without a name.\n");
		return NULL;
	}
	ptr -> next = macroTable;
	return ptr;
}




//Locking for a macro with the name %word%, if found copy the content to the file and return 1, otherwise return 0.
int isMacro(char *word, Macro *macroTable, FILE *spread_macros_p)
{
	Macro *tablePtr = macroTable;

	while (tablePtr != NULL)
	{
		if (strcmp(tablePtr->name, word)== 0)//found macro with the name %word%
		{
			fprintf (spread_macros_p, "%s", tablePtr->text); //add the macro content to the file
			return 1;
		}

		tablePtr = tablePtr->next;
	}


	return 0;
}

int macroSpread (FILE *source_file_p, FILE *spread_macros_p)
{
	char line[MAX_LINE];
	int macroFlag = 0;
	Macro *macroTable = NULL;
	char firstWord [MAX_LINE];

	while (fgets(line, MAX_LINE, source_file_p))
	{
		memset (firstWord, '\0', MAX_LINE);
		if(!getNWord(firstWord, line, 0))//only white spaces
			continue;

		if (strcmp(firstWord, "mcr") == 0) //define new macro
		{
			macroFlag = 1;
			macroTable = addMacro(line, macroTable);
			if (macroTable == NULL)
				return 0;
			continue;
		}

		if (strcmp(firstWord, "endmcr")== 0)// finish definition of macro
		{
			if(macroFlag)
			{
				macroFlag = 0;
				continue;
			}
			else
			{
				printf("Error!\nendmcr without mcr.\n");
				return 0;
			}

		}

		if(isMacro(firstWord, macroTable, spread_macros_p)) //this line is a macro call
			continue;

		if(macroFlag) //this line belong to the macro that point by macroTable
			strcat(macroTable->text, line);
		else
			fprintf (spread_macros_p, "%s", line); //add the line to the file
	}

	return 1;
}

