/* [mmn 14 : ]:
in this file: 

author: Uri K.H,   Lihi Haim       Date: 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <stdio.h>
#include "bool.h"

/*symbolTable: like it's name it is the layout of the element in symbol table*/

typedef struct{
    char *symbol;
    int address;
    char *attribute1;
    char *attribute2;
} symbolTable;

void pushEntryToFile(FILE *);
void pushExternToFile(FILE *fd);
boolean addToExtTab(char *, int);
boolean entryExist();
boolean externalExist();
boolean wasDefined(char *, int, boolean, int);
symbolTable getFromSymTab(char *);

void printSymTab();
