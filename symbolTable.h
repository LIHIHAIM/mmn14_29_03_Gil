/* [mmn 14 : symbolTable.h]:
in this file: prototypes of functions which are operating on the symbol-table or are related to it. 

author: Uri K.H,   Lihi Haim       Date: 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include "bool.h"

boolean pushExtern(char *, int *, int);
boolean pushEntry(char *, int*, int);
boolean checkEntry(char *, int *, int);
boolean addToSymTab(char *, char *, int);
boolean wasDefined(char *, int, boolean, int);
void encPlusIC();
void cleanSymAndExtTab();
void printSymTab();