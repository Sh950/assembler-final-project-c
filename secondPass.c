
#include "secondPass.h"

SymbolTable *secondPass(FILE *source_file_p, char machineCodeIns[][MAX_BITS+1], SymbolTable *table, char extFileName[])
{
	int IC = 0;

	char line[MAX_LINE]; //the instruction line
	memset (line, '\0', MAX_LINE);
	char currStrWord[MAX_LINE];


	int lineNumber = 0;
	int errors = 0;
	int currWord = 0;
	char labelName[MAX_LABEL];
	SymbolTable* symbol;

	int fileCreated = 0;
	FILE *extFile = NULL;
	char filePath[MAX_LINE+4]; //adding 4 for the extension
	memset (filePath, '\0', MAX_LINE);
	strcpy(filePath, extFileName);
	strcat(filePath, ".ext");

	while (fgets (line, MAX_LINE, source_file_p) != NULL)// read line
	{
		memset (currStrWord, '\0', MAX_LINE);
		currWord = 0; // words that has read from the line
		lineNumber ++;

		memset(labelName, '\0', MAX_LABEL);

		if(!getNWord(currStrWord, line, currWord) || currStrWord[0] == ';')//only white spaces or comment line
			continue;
		currWord ++;

		if (isLabel(currStrWord)==1) // the line begin with a label
		{
			memset (currStrWord, '\0', MAX_LINE);
			if(!getNWord(currStrWord, line, currWord))//Get the next word in the line, if no more words in this line continue
				continue;
			currWord ++;
		}
		if (strcmp(currStrWord, ".data") == 0 || strcmp(currStrWord, ".string") == 0
				|| strcmp(currStrWord, ".extern") == 0 || strcmp(currStrWord, ".define") == 0)
		{
			continue;
		}
		if(strcmp(currStrWord, ".entry") == 0)
		{
			//get all entry definitions in line and add to symbol table
			memset (currStrWord, '\0', MAX_LINE);
			while(getNWord(currStrWord, line, currWord))
			{
				currWord++;
				if((symbol = symbolExists(table, currStrWord)) == NULL)
				{
					printf("%s %d %s\n", "Error!\n Symbol used by entry does not exist. In line: ", lineNumber, ".");
					errors++;
				}
				table = addSymbol(table, currStrWord, symbol->value, ".entry");
			}
			continue;
		}
		//update IC by 1 for the first byte for this line.
		IC++;
		//moving on to the next word, as if we got here the first word is an instruction
		memset (currStrWord, '\0', MAX_LINE);
		int firtsArgReg = 0;
		while(getNWord(currStrWord, line, currWord))
		{
			currWord++;
			if(currStrWord[0] == '#')
			{
				IC++;
				continue;
			}
			if(currStrWord[0] == 'r')
			{
				if(firtsArgReg == 0)
				{
					IC++;
					firtsArgReg = 1;
				}
				continue;
			}
			int addressingType = 1;
			//last option is only addressing type 1 or 2
			if(isAlphabet(currStrWord[0]))
			{
				int len = length(currStrWord);
				int j =0;
				for (; j < len && currStrWord[j] != '['; j++);
				if (j != len) // we stopped because we found '[', addressingType is 2
				{
					//remove the index part
					currStrWord[j] = '\0';
					addressingType = 2;
				}
				//otherwise, addressing type is 1 and the value of 'currStrWord' is the actual word
			}

			//no need to check validation of commas because we have done it in the first pass, if there is a comma, just remove it
			if (lastChar(currStrWord) == ',')
				currStrWord[length(currStrWord)-1] = '\0'; //remove the comma

			if((symbol = symbolExists(table, currStrWord)) == NULL)
			{
				printf("%s %d %s\n", "Error!\n Symbol used in code does not exist. In line: ", lineNumber, ".");
				errors++;
			}
			//move the the 12 left bits to keep 2 right most bits for ARE
			int value = (symbol->value)*4;
			intToBinaryStr(machineCodeIns[IC], value);
			if(strcmp(symbol->type, ".external") == 0)
			{
				machineCodeIns[IC][12] = '0';
				machineCodeIns[IC][13] = '1';

				if(fileCreated == 0)
				{
					extFile = fopen(filePath, "w");
					if(extFile == NULL) //validate file
					{
						printf("ERROR!\nNo access to create .ext file\n");
						errors++;
					}
					fileCreated = 1;
				}
				int line = IC+100;
				if(line>=1000)
					fprintf (extFile, "%s\t%d\n", symbol->name, line);
				else
					fprintf (extFile, "%s\t0%d\n", symbol->name, line);

			}
			else//it's an internal label
			{
				machineCodeIns[IC][12] = '1';
				machineCodeIns[IC][13] = '0';
			}
			IC++;
			if(addressingType == 2)
				 IC++; //another byte required for the actual index
			memset (currStrWord, '\0', MAX_LINE);
		}
	}
	if (errors > 0)
		return NULL;

	if(fileCreated == 1)
		fclose(extFile);
	return table;

}

