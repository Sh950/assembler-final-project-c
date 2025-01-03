/*
 * macro.h
 *
 *  Created on: Mar 18, 2024
 *      Author: user
 */

#ifndef MACRO_H_
#define MACRO_H_

#include "assembler.h"
#include "utils.h"

typedef struct Macro {
	char name[MAX_LINE];
	char text[MAX_MACRO];
	struct Macro *next;
}Macro ;

int macroSpread (FILE *source_file_p, FILE *spread_macros_p);

#endif /* MACRO_H_ */
