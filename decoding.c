/*
 * decoding.c
 *
 *  Created on: Mar 19, 2024
 *      Author: user
 */
#include "decoding.h"

// decoding ".data" line
int decodingData(SymbolTable *table, int DC, char machineCodeData[][MAX_BITS+1], char line[], int lineNumber, int currWord)
{
	int commaExepected = 0;
	char word[MAX_LINE];
	int temp;
	int i = 0;
	SymbolTable *define;


	for (; i <=  MAX_DATA_ARGS; i++, currWord ++)
	{
		memset(word, '\0', MAX_LINE);

		if(!getNWord(word, line, currWord)) //no more arguments
		{
			if(!commaExepected)
			{
				printf("%s%d%s\n", "Error!\nIllegal comma in line: ", lineNumber, ".");
				return 0;
			}
			else
				break;
		}

		if(commaExepected)
		{
			if (strcmp(word, ",") == 0)
			{
				commaExepected = 0;
				continue;
			}
			else
			{
				printf("%s%d%s\n", "Error!\nIllegal arguments syntax in line: ", lineNumber, ".");
				return 0;
			}
		}

		else //should be argument
		{
			if (strcmp(word, ",") == 0)
			{
				printf("%s%d%s\n", "Error!\nIllegal comma in line: ", lineNumber, ".");
				return 0;
			}
			else //is argument
			{
				if (lastChar(word) == ',')
					word[length(word)-1] = '\0'; //remove the comma
				else // next word should be a comma
					commaExepected = 1;


				temp = atoi(word); //convert the str to it's int value

				if(temp == 0)//Check if atoi failed to convert the str to int or that the str represent the int value 0
				{
					word[0]++;
					if (atoi(word) == 0)//the atoi field to convert the str to int
					{
						word[0]--;
						if((define = isSymbol(word, "mdefine", table)) != NULL)//check if "word" is a define value
						{
							intToBinaryStr (machineCodeData[DC], define->value);
							DC++;
							continue;
						}
						else // word is not a define value and is not a legal int value
						{
							printf("%s%d%s\n", "Error!\nIlegal arguments in line: ", lineNumber, ".");
							return 0;
						}
					}
					else  //the str represent the int value 0
					{
						intToBinaryStr (machineCodeData[DC], 0);
						DC++;
						continue;
					}
				}
				else
				{
					intToBinaryStr (machineCodeData[DC], temp);
					DC++;
					continue;
				}
			}
		}

		if (i == 0)//no arguments was received
		{
			printf("%s%d%s\n", "Error!\nNo arguments received for the .data instruction in line: ", lineNumber, ".");
			return 0;
		}

	}
	return DC;
}

//decoding the string that is the currWord in the "" sign, to the machineCodeData in binary representation begin from DC and update the DC according to the use
int decodingStr( int DC, char machineCodeData[][MAX_BITS+1], char line[], int lineNumber, int currWord)
{
	char word[MAX_LINE];
	memset (word, '\0', MAX_LINE);

	if(!getNWordStr(word, line, currWord)) //no more arguments
	{
		printf("%s%d%s\n", "Error!\nNo arguments received for the .string instruction in line: ", lineNumber, ".");
		return 0;
	}

	char temp[MAX_LINE];
	if(getNWord(temp, line, currWord + 1))
	{
		//.string can only have one argument, if more than 1 provided, error
		printf("%s%d%s\n", "Error!\nExtra characters after the argument in .string instruction in line: ", lineNumber, ".");
		return 0;
	}

	for (int i = 0; word[i] != '\0'; i++)
	{
		intToBinaryStr (machineCodeData[DC], (int)word[i]);
		DC++;
	}

	intToBinaryStr (machineCodeData[DC], 0);
	DC++;
	return DC;
}

//return 1 if the definition of the label is legal, otherwise return 0
int isLegalLable(char word[])
{
	if (isAlphabet(word[0]) && isAlphabetOrNum(word, 1) && length(word) <= MAX_LABEL)
		return 1;
	return 0;
}

//Decode an external definition and add to the symbol table
SymbolTable *decodingExtern(SymbolTable *table, char line[], int lineNumber, int currWord, char firstWord[])
{
	int commaExepected = 0;
	char word[MAX_LINE];
	int i = 0;
	SymbolTable *temp_table = table;


	for (; i <=  MAX_EXTERN_ARGS; i++, currWord ++)
	{
		memset(word, '\0', MAX_LINE);

		if(!getNWord (word, line, currWord)) //no more arguments
		{
			if(!commaExepected)
			{
				printf("%s%d%s\n", "Error!\nIllegal comma in line: ", lineNumber, ".");
				return NULL;
			}
			else
				break;
		}

		if(commaExepected)
		{
			if (strcmp(word, ",") == 0)
			{
				commaExepected = 0;
				continue;
			}
			else
			{
				printf("%s%d%s\n", "Error!\nIllegal arguments syntax in line: ", lineNumber, ".");
				return NULL;
			}
		}

		else //should be argument
		{
			if (strcmp(word, ",") == 0)
			{
				printf("%s%d%s\n", "Error!\nIllegal comma in line: ", lineNumber, ".");
				return NULL;
			}
			else //is argument
			{
				if (lastChar(word) == ',')
					word[length(word)-1] = '\0'; //remove the comma
				else // next word should be a comma
					commaExepected = 1;

				//validate doesn't exist already
				if(isSymbol(firstWord, ".extern", temp_table))
				{
					printf("%s %s %s%d%s\n", "Error!\nSymbol", firstWord, "already defined in table. Line number: ", lineNumber, ".");
					return NULL;
				}

				if (strcmp(firstWord, ".extern") == 0)
				{
					temp_table = addSymbol(temp_table, word, 0, ".external");
				}
			}
		}
	}


	if (i == 0)//no arguments received
	{
		printf("%s%d%s\n", "Error!\nNo arguments was received for the .extern/.entry instruction in line: ", lineNumber, ".");
		return NULL;
	}

	return temp_table;

}

//Create the byte for values after '#'
int createImmidiateByte(SymbolTable *table, char word[], int lineNumber, char machineCodeIns[][MAX_BITS+1], int IC, int first_byte_IC,  int opLocation)
{
	if(length(word) < 2) //the word is only '#' and not actual operand
	{
		printf("%s%d%s\n", "Error!\nMissing actual argument after '#' in line: ", lineNumber, ".");
		return -1;
	}
	// source location is 8,9 and destination is location 10,11
	machineCodeIns[first_byte_IC][opLocation] = '0';
	machineCodeIns[first_byte_IC][opLocation+1] = '0';
	//remove first char from 'word'
	int len = length(word);
	memmove(word, word + 1, len);
	len--;

	//Adding another byte for the value
	//IC++;
	//if is alphabet that means it needs to be in the symbol table
	if (isAlphabet(word[0]) == 1)
	{
		SymbolTable *symbol = isSymbol(word, "mdefine", table);
		if(symbol == NULL)
		{
			printf("%s %s %s%d%s\n", "Error!\nSymbol", word, " does not exists in symbol table, line number: ", lineNumber, ".");
			return -1;
		}
		//copy the value from the symbol table to the next byte after moving 2 bits to the left
		int value = (symbol->value)*4;
		intToBinaryStr(machineCodeIns[IC], value);
	}
	else //expect a number
	{
		for (int i = 0; i<len; i++)
		{
			//cover the case of negative number
			if(i==0 && word[i] == '-')
				continue;
			if(!isNum(word[i]))
			{
				printf("%s%d%s\n", "Error!\nIlligal argument in line: ", lineNumber, " (expected number).");
				return -1;
			}
		}
		//if we got here it means we can convert the word to int
		int value = atoi(word);
		//to push the value 2 bits to the left (since the value is only in 12 left bits and 2 right bits are for ARE
		value = value*4;
		//create the binary of the value
		intToBinaryStr(machineCodeIns[IC], value);
	}
	IC++;
	return IC;
}

//Create machine code for addressing type 2 ex: LIST[sz]
int createConstIndexByte(SymbolTable *table, char word[], int lineNumber, char machineCodeIns[][MAX_BITS+1], int IC, int first_byte_IC, int opLocation, int j, int len)
{
	machineCodeIns[first_byte_IC][opLocation] = '1';
	machineCodeIns[first_byte_IC][opLocation +1] = '0';

	//make sure we have a corresponding ']'
	if(word[len-1] != ']' || j == (len-1))
	{
		printf("%s%d%s\n", "Error!\nIllegal argument with '[' and no ']' in line: ", lineNumber, ".");
		return -1;
	}
	//if we got here first is [ and last is ]
	//label might not exist yet, therefore just add ?
	memset(machineCodeIns[IC], '?', MAX_BITS+1);
	machineCodeIns[IC][14] = '\0';
	IC++;

	//extract the index only (remove [ and ])
	memmove(word, word+j+1, len);
	len = length(word);
	word[len-1] = '\0';

	if(isStrNum(word))
	{
		//add number in 12 left bits and '00' for ARE
		int temp = atoi(word);
		//to push the value 2 bits to the left (since the value is only in 12 left bits and 2 right bits are for ARE
		temp = temp*4;
		//create the binary of the value
		intToBinaryStr(machineCodeIns[IC], temp);
		IC++;
		return IC;
	}
	else
	{
		SymbolTable *symbol = isSymbol(word, "mdefine", table);
		if(symbol == NULL)
		{
			printf("%s%d%s\n", "Error!\nIlligal index in line: ", lineNumber, " (expected to be in symbol table).");
			return -1;
		}

		int value = symbol->value;
		//to push the value 2 bits to the left (since the value is only in 12 left bits and 2 right bits are for ARE
		value = value*4;
		//create the binary of the value
		intToBinaryStr(machineCodeIns[IC], value);
		IC++;
		return IC;
	}
	printf("%s%d%s\n", "Error!\nIlligal index in line: ", lineNumber, " (expected number).");
	return -1;
}


//Create the binary machine code for an instruction line
int createBinaryForInstruction(SymbolTable *table, char line[], int lineNumber, char firstWord[], char machineCodeIns[][MAX_BITS + 1], int IC, int currWord)
{
	char instructions[NUM_INSTRUCTIONS][4];
	strcpy(instructions[0], "mov");
	strcpy(instructions[1], "cmp");
	strcpy(instructions[2], "add");
	strcpy(instructions[3], "sub");
	strcpy(instructions[4], "not");
	strcpy(instructions[5], "clr");
	strcpy(instructions[6], "lea");
	strcpy(instructions[7], "inc");
	strcpy(instructions[8], "des");
	strcpy(instructions[9], "jmp");
	strcpy(instructions[10], "bne");
	strcpy(instructions[11], "red");
	strcpy(instructions[12], "prn");
	strcpy(instructions[13], "jsr");
	strcpy(instructions[14], "rts");
	strcpy(instructions[15], "hlt");


	int first_byte_IC = IC;

	//memset for the current word
	memset(machineCodeIns[IC], '0', MAX_BITS+1);
	machineCodeIns[IC][MAX_BITS] = '\0';

	int opNum = findIndex(instructions, NUM_INSTRUCTIONS, firstWord);
	if(opNum == -1)
	{
		printf("%s%d%s\n", "Error!\nInstruction does not exist, in line: ", lineNumber, ".");
		return 0;
	}

	char binary_op[MAX_BITS+1];
	intToBinaryStr(binary_op, opNum);
	//copy 4 bits of op number into bits 6-9
	for(int i=4; i<8; i++)
	{
		//starting from the left most bit of value (in bit #10 from binary_op)
		machineCodeIns[first_byte_IC][i] = binary_op[i+6];
	}

	//update the IC to the next available byte
	IC++;

	//Walking through arguments
	int commaExepected = 0;
	char word[MAX_LINE];
	int temp;
	int i = 0;
	int numArgs = 0;

	if(opNum <=3 || opNum == 6)
		numArgs = 2;
	if (opNum == 4 || opNum == 5 || (opNum >=7 && opNum <= 13))
		numArgs = 1;


	int addressingType = 0;
	//flag to note that the source arg was a register type
	int source_reg = 0;
	for (; i <  numArgs; i++, currWord ++)
	{
		memset(word, '\0', MAX_LINE);

		if(!getNWord (word, line, currWord)) //no more arguments
		{
			printf("%s%d%s\n", "Error!\nMissing arguments in line: ", lineNumber, ".");
			return -1;
		}

		if(commaExepected)
		{
			if (strcmp(word, ",") == 0)
			{
				commaExepected = 0;
				continue;
			}
			else
			{
				printf("%s%d%s\n", "Error!\nIllegal arguments syntax in line: ", lineNumber, ".");
				return -1;
			}
		}

		else //should be argument
		{
			if (strcmp(word, ",") == 0)
			{
				printf("%s%d%s\n", "Error!\nIllegal comma in line: ", lineNumber, ".");
				return -1;
			}
			else //is argument
			{
				if (lastChar(word) == ',')
					word[length(word)-1] = '\0'; //remove the comma
				else // next word should be a comma
					commaExepected = 1;
			}
		}

		//opLocation holds the bit location to set the operands at. Either 8,9 (source) or 10,11 (destination)
		//setting it based on i, to cover the case of 2 arguments
		int opLocation = 8 + (2*i);
		if (numArgs == 1)
		{
			//when there is only one operand, its only in the destination which is bits 10,11
			opLocation = 10;
		}

		//need to extract all this to method since it's now running on all operands and matches really only first
		if(word[0] == '#')
		{
			addressingType = 0;
			temp = createImmidiateByte(table, word, lineNumber, machineCodeIns, IC, first_byte_IC, opLocation);
			if(temp == -1)
				return -1;
			IC = temp;
			continue;
		}
		if(word[0] == 'r' && isNum(word[1]) &&  length(word) == 2)
		{
			addressingType = 3;
			machineCodeIns[first_byte_IC][opLocation] = '1';
			machineCodeIns[first_byte_IC][opLocation+1] = '1';
			int temp = word[1] - '0';
			if(i == 0) //source register
			{
				source_reg = 1;
				memset(machineCodeIns[IC], '0', MAX_BITS+1);
				machineCodeIns[IC][MAX_BITS] = '\0';
				//to push the value 5 bits to the left (to be in bit 5-7 from the right)
				temp= temp*32;
				intToBinaryStr(machineCodeIns[IC], temp);
				IC++;
			}
			else // dest register
			{
				if(source_reg)//first arg was also of type reg
				{
					char temp_word[MAX_BITS+1];
					intToBinaryStr(temp_word, temp);
					//right most bit is index 13
					int temp_index = MAX_BITS -1;
					//Start copying from the right of temp word to bit 2-4 in machineCodeIns
					//do not create a new byte, because source register was also of type reg
					machineCodeIns[IC-1][temp_index-2] = temp_word[temp_index];
					machineCodeIns[IC-1][temp_index-3] = temp_word[temp_index-1];
					machineCodeIns[IC-1][temp_index-4] = temp_word[temp_index-2];
				}
				else
				{
					memset(machineCodeIns[IC], '0', MAX_BITS+1);
					machineCodeIns[IC][MAX_BITS] = '\0';
					//to push the value 2 bits to the left (to be in bit 2-4 from the right)
					temp= temp*4;
					intToBinaryStr(machineCodeIns[IC], temp);
					IC++;
				}
			}
			continue;
		}
		//last option is only addressing type 1 or 2
		if(isAlphabet(word[0]))
		{
			int len = length(word);
			int j =0;
			for (; j < len && word[j] != '['; j++);
			if (j != len) // we stopped because we found '['
			{
				//addressingType is 2
				addressingType = 2;
				temp = createConstIndexByte(table, word, lineNumber, machineCodeIns, IC, first_byte_IC, opLocation, j, len);
				if(temp == -1)
					return -1;
				IC = temp;
				continue;
			}
			else
			//addressingType is 1
			{
				machineCodeIns[first_byte_IC][opLocation] = '0';
				machineCodeIns[first_byte_IC][opLocation+1] = '1';
				addressingType = 1;
				//since the label might not be defined yet, adding only empty byte and will update in second pass
				memset(machineCodeIns[IC], '?', MAX_BITS+1);
				machineCodeIns[IC][MAX_BITS] = '\0';
				IC++;
				continue;
			}
		}
		//if we got here the argument does not match any option
		printf("%s %d%s\n", "Error!\nIllegal argument in line: ", lineNumber, ".");
		return -1;

	}

	if(getNWord (word, line, currWord)) //more arguments
	{
		printf("%s%d%s\n", "Error!\nExtra arguments in line: ", lineNumber, ".");
		return -1;
	}

	return IC;

}
