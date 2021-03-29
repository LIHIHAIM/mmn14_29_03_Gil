/* [mmn 14 : util.h]:
in this file: prototypes of utility functions which are used in many of the other files.

author: Uri K.H,   Lihi Haim       Date: 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include "bool.h"

#define OS_MEM 100 /* operating system private memory */

/* util.c functions prototypes */
boolean isBlank(char *, int);
void jumpSpaces(char *, int *);
boolean isAlloc(void *);
boolean isThereComma(char *, int *, int);

boolean isIlegalName(char *, int); /* from instructionHandeling.c */
int getIC(); /* from instructionHandeling.c */
int getDC(); /* from dataHandeling.c */