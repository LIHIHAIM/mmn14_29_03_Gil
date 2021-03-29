/* [mmn 14 : ]:
in this file: 

author: Uri K.H,   Lihi Haim       Date: 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <stdio.h>
#include "bool.h"

#define LINE_LEN 81
#define MAX_SUFIX_LEN 4

FILE *openf(char *, char *);
char *readLine(FILE *, int);
char *addSufix(char *, char *, char *);
boolean buildOutputFiles(char *);