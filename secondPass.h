/*
 * secondPass.h
 *
 *  Created on: Apr 23, 2024
 *      Author: user
 */

#ifndef SECONDPASS_H_
#define SECONDPASS_H_

#include "utils.h"
#include "symbolTable.h"


SymbolTable *secondPass(FILE *source_file_p, char machineCodeIns[][MAX_BITS+1], SymbolTable *table, char extFileName[]);

#endif /* SECONDPASS_H_ */
