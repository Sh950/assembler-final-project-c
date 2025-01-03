/*
 * assembler.c
 *
 *  Created on: Mar 18, 2024
 *      Author: user
 */

#include "assembler.h"

int createEntFile(SymbolTable *table, char fileName[])
{
	SymbolTable *tablePtr = table;
	int fileCreated = 0;
	FILE *entFile = NULL;
	char filePath[MAX_LINE+4]; //adding 4 for the extension
	memset (filePath, '\0', MAX_LINE);
	strcpy(filePath, fileName);
	strcat(filePath, ".ent");
	while (tablePtr != NULL)
	{
		if (strcmp(tablePtr->type, ".entry") == 0 )//found Symbol of type entry
		{
			if(fileCreated == 0)
			{
				entFile = fopen(filePath, "w");
				if(entFile == NULL) //validate file
				{
					printf("ERROR!\nNo access to create .ent file\n");
					return 0;
				}
				fileCreated = 1;
			}
			fprintf (entFile, "%s\t%d\n", tablePtr->name, tablePtr->value);
		}
		tablePtr = tablePtr->next;
	}
	if(fileCreated == 1)
		fclose(entFile);
	return 1;
}

// Get an 14 bits binary string and replace it to 7 bits code with the symbols "*#%!".
int createObjFile(char machineCodeIns[][MAX_BITS+1], char fileName[], int inputIC, int inputDC)
{
	int temp;
	int i = 0; //index to read bits
	int j = 0; //index to write bit

	int counter = 0;
	FILE *objFile = NULL;
	char filePath[MAX_LINE+3]; //adding 3 for the extension
	memset (filePath, '\0', MAX_LINE);
	strcpy(filePath, fileName);
	strcat(filePath, ".ob");
	objFile = fopen(filePath, "w");
	if(objFile == NULL) //validate file
	{
		printf("ERROR!\nNo access to create .ob file\n");
		return 0;
	}

	//write title with number of code and number of data
	fprintf (objFile, "  %d %d\n", inputIC, inputDC);

	//loop on the total number of bytes
	while (counter < inputIC + inputDC)
	{
		i = 0;
		j = 0;
		//write line number at the beginning of the line
		if(counter + 100 >= 1000)
			fprintf (objFile, "%d ", counter+100);
		else
			fprintf (objFile, "0%d ", counter+100);
		// loop on the bits in the current byte
		while (j <= 6)
		{
			//calculate the decimal value of the current two bits to convert it to a symbol
			temp = (machineCodeIns[counter][i] -'0') * 2 + (machineCodeIns[counter][i+1] -'0');

			switch (temp)
			{
				case 0:
				   fprintf (objFile, "%c", '*');
				   break;
				case 1:
					fprintf (objFile, "%c", '#');
				   break;
				case 2:
					fprintf (objFile, "%c", '%');
				   break;
				case 3:
					fprintf (objFile, "%c", '!');
				   break;
			}

			i+=2;
			j+=1;
		}
		fprintf (objFile, "%s", "\n");


		//moving to the next byte
		counter++;
	}
	fclose(objFile);
	return 1;

}


int main (int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Error:\nNumber of arguments is invalid\n");
		return 1;
	}

	for(int i = 1; i < argc; i++)
	{

		FILE *source_file_p = NULL, *spread_macros_p = NULL;

		source_file_p = fopen(argv[i], "r"); //open the source file
		char file_path[MAX_LINE];
		memset (file_path, '\0', MAX_LINE);
		strcpy(file_path, argv[i]);
		char file_name[MAX_LINE];
		memset (file_name, '\0', MAX_LINE);
		strcpy(file_name, file_path);
		strcat(file_path, ".am");// add to the input's file path the ".am" extension
		spread_macros_p = fopen(file_path, "w"); // create and open the "am" file

		if(source_file_p == NULL) //validate file
		{
			printf("ERROR!\nNo access to a file\n");
				return 1;
		}

		if(spread_macros_p == NULL)
		{
			printf("ERROR!\nUnable to create .am file\n");
				return 1;
		}

		if (!macroSpread(source_file_p, spread_macros_p))
		{
			printf("Exit...");
			exit(1);
		}
		fclose(spread_macros_p);
		fclose(source_file_p);
		spread_macros_p = fopen(file_path, "r");


		char machineCodeIns[MAX_MACHINE_CODE_LINES][MAX_BITS+1];//the instruction array
		char machineCodeData[MAX_MACHINE_CODE_LINES][MAX_BITS+1];//the data array
		SymbolTable *table = NULL; //The symbol table
		int IC, DC;
		table = firstPass(spread_macros_p, machineCodeIns, machineCodeData, table, &IC, &DC);
		if (table == NULL)
		{
			printf("Errors detected at first pass. Exit ...");
			exit(1);
		}

		//to init it again from the beginning.
		fclose(spread_macros_p);
		spread_macros_p = fopen(file_path, "r");
		table = secondPass(spread_macros_p, machineCodeIns, table, file_name);
		if (table == NULL)
		{
			printf("Errors detected at second pass. Exit ...");
			exit(1);
		}

		fclose(spread_macros_p);

		if(createEntFile(table, file_name) == 0 || createObjFile(machineCodeIns, file_name, IC, DC) == 0)
		{
			printf("Errors while trying to create output files. Exit ...");
			exit(1);
		}

	}

	printf("Completed successfully...");

	return 0;
}
