/*

 * firstPass.h
 *
 *  Created on: Mar 18, 2024
 *      Author: user
*/

#ifndef FIRSTPASS_H_

#define FIRSTPASS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decoding.h"
#include "utils.h"
#include "symbolTable.h"


SymbolTable *firstPass (FILE *source_file_p, char machineCodeIns[][MAX_BITS+1], char machineCodeData[][MAX_BITS+1], SymbolTable *table, int* inputIC, int* inputDC);


#endif /* FIRSTPASS_H_ */
