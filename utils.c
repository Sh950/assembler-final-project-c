/*
 * utils.c
 *
 *  Created on: Mar 19, 2024
 *      Author: user
 */

#include "utils.h"


//return 1 if the char is from alphabet, otherwise 0
int isAlphabet(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    		return 1;
    return 0;
}

int isLabel(char word[])
{
	if (isAlphabet(word[0]) && isAlphabetOrNum(word, 0) &&  lastChar(word) == ':' && length(word) <= MAX_LABEL)
		return 1;
	return 0;
}

//return the last char that is not '\0' or '\n' in the str, if no other characters return '\0', assume that their is one '\0' char at least
char lastChar (char str[])
{
	int i = 0;
	for(; str[i] != '\0' && str[i] != '\n'; i++) ;
	if (i == 0 || (i == 1 && str[i] == '\n'))
		return '\0';
	if (str[i-1] == '\n')
		return str[i-2];
	return str[i-1];
}

//return 1 if the char is a digit, otherwise 0
int isNum(char c)
{
    if ((c >= '0' && c <= '9'))
    	return 1;
    return 0;
}

//return 1 if the entire string are all digits and represents a number
int isStrNum(char word[])
{
	for (int i=0; i<length(word); i++)
	{
		if(!isNum(word[i]))
		{
			return 0;
		}
	}
	return 1;
}

// check if the word (the characters until the first '\0') contain only alphabet or numbers until the last character not include the last one
// if string is empty will return 1
// set checkLastChar to a non zero value to validate last char too
int isAlphabetOrNum(char word[], int checkLastChar)
{
	for (int i = 0; word[i] != '\0'; i++)
		if (!(isAlphabet(word[i]) || (isNum(word[i]))))
			if (!checkLastChar && word[i+1] != '\0')
				return 0;
	return 1;
}

//return the length of the string in "word" till the '\0' char, assume that their is one '\0' char at list
int length(char word[])
{
	int i = 0;
	for(; word[i] != '\0'; i++);
	return i;
}


//return the index of the n word in line, return -1 if there aren't n words in the line
int indxNWord(char *line, int n)
{
	int j = 0;
	int k = 0;

	int eqFound = 0;

	for(int i = 0; i<=n ; i++) //Skipping the n-1 first words
	{
		if (line[j] == '\0') // no more words in the line
			return -1;
		for(; isspace(line[j]); j++); // Skipping the next white spaces
		k = j;
		if(line[j] == '=')
		{
			j++;
			continue;
		}

		for(; !isspace(line[j]) && line[j] != '=' && line[j] !='\0'; j++) // Skipping the characters of the current word
		{
			if (line[j] == ',')
			{
				j++;
				break;
			}
		}
	}

	if (line[k] == '\0') // no more words in the line
		return -1;

	return k;
}


// copy the n word in line to word
int getNWord (char *word, char *line, int n)
{
	int i = 0,j = 0;
	if ((i = indxNWord(line, n)) == -1) //There aren't n words in the line
		return 0;
	j = i + 1;
	if(line[i] != '=')
		for(; !isspace(line[j]) && line[j] != '=' && line[j] != ',' && line[j] !='\0'; j++); // find the end of the word

	if(line[j] == ',')
		j++;
	strncpy(word, &(line[i]), j-i); // copy the word

	return 1;
}

// copy the n word in line to word only when the word in quotation mark
int getNWordStr (char *word, char *line, int n)
{
	int i = 0;
	int j = 0;

	if ((i = indxNWord(line, n)) == -1) //There aren't n words in the line
		return 0;

	if (line[i] != '"')
		return 0;

	i++;
	j = i;

	for(; line[j] != '"'; j++); // Skipping the characters of the current word
		if (line[j] =='\0')
			return 0;

	if (j == i)//no chars in the string
		return 1;

	strncpy(word, &(line[i]), j-i); // copy the word

	return 1;
}


//write the binary representation of value to word
//negative values are represented in two's complement method
//assume that the length of word is MAX_BITS +1 and its big enough to the binary representation of value + the null sign in the end
void intToBinaryStr(char word[], int value)
{
	memset(word, '0', MAX_BITS+1);
	word[MAX_BITS] = '\0'; // put in the last char NULL sign

	int temp;
	int i;

	//if the value is negative create it's value in twos-completed
	if (value < 0)
	{
		value = pow(2, MAX_BITS) + value;
	}

	while(value != 0)
	{
		temp = 1;
		i = 0;

		for (;temp * 2 <= value; i++)// Check what is the higher power of 2 that is not more then value
			temp *= 2;

		word[MAX_BITS-1-i] = '1'; // put 1 in i bit
		value -= temp;
	}
}

//get index of instruction
int findIndex(char  instructions[][4], int numInstructions, char word[])
{
    for (int i = 0; i < numInstructions; ++i) {
        if (strcmp(instructions[i], word) == 0) {
            return i; // return found index
        }
    }
    return -1; // return -1 if index not found
}

