/* 
author: Uri K.H,   Lihi Haim    Date: 21.3.2021 
ID: 215105321,     313544165    Tutor: Danny Calfon */

/* [mmn 14: compiler.c]:
This file contiains the main function in the compilation process of the assembly file.
The compile() function will compile the wnated assmebly file into three optput files: 
fileName.ob, fileName.ent, fileName.ext (if possible)

This file also conatins the sub functions nedded for the main 'compile()' fucntion.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ineterface.h"
#include "files.h"
#include "line.h"
#include "util.h"
#include "label.h"
#include "instructionHandeling.h"
#include "dataHandeling.h"
#include "symbolTable.h"
#include "statmentType.h"

#define FOREVER 1

void clean(boolean, FILE *);

/* compile(): The function compiles an Assembly file and make one to three files in addition:
 file.ob - a binary file, which includes the memory....
 file.ent - an "entry" file, which includes the names of all the variable which other files can see and
            their address in the RAM.
 file.ext - an "extern" file, which includes the names of all the external variables (from other files) 
            and their calling address in the RAM.
*/
boolean compile(char *fileName){
     /* files variables */
     char *tempFileName = malloc(strlen(fileName) + MAX_SUFIX_LEN + 1);
     FILE *fd = fopen(addSufix(tempFileName, ".as", fileName), "r");
     int lineCnt = 0, i;
     /* line variables */
     char line[LINE_LEN];            /* array for handling a line of assembly code */
     int *lInd = malloc(sizeof(int)); /* the Line-Index */
     char *tempLine = NULL, *optLabel = NULL;
     /* label and compilation variables and falgs */
     statType type;
     boolean compSuc = TRUE, labelExist, pushed;

     if (!isAlloc(lInd) || !isAlloc(tempFileName)){
          free(lInd);
          free(tempFileName);
          return ERROR;
     }
     printf("fd = %p\n", (void *)fd);
     if (!fd){
          printf("error : could not open file: \"%s.as\"\n",fileName);
          free(lInd);
          free(tempFileName);
          return FALSE;
     }
     rewind(fd);
     if(!fd)
          printf("fd is NULL\n ");
     free(tempFileName);
     clean(FALSE, NULL);

     while (FOREVER){
          labelExist = FALSE;
          lineCnt++;
          *lInd = 0;
          tempLine = readLine(fd, LINE_LEN); /* reading a line from the file */
          if (tempLine[0] == '\0') /* end of file */
               break;
          strcpy(line, tempLine);
          free(tempLine);
          printf("line : %s\n", line);
          for(i = 0; i < 100; i++)
               printf(" \r");
          if(isBlank(line, *lInd))
               continue;
          jumpSpaces(line, lInd);
          if (!(optLabel = readWord(line, lInd))){
               free(lInd);
               clean(TRUE, fd);
               return ERROR;
          }
          if(isLabel(optLabel)){
               labelExist = TRUE;
               if(!isValidLabel(optLabel, lineCnt, TRUE)) /* could not get a label */
                    compSuc = FALSE;
          }
          if(!labelExist){
               free(optLabel);
               *lInd = 0;
          }
          /*jumpSpaces(line, lInd);  for now */
          switch ((type = getStatType(line))){ /* finding the statment type (instruction/ directive/ balnk line/ comment) */
          case blank:
          case comment:
               break;
          case directive:
          {    
               int command = getDirCom(line, lInd, lineCnt);
               switch (command){
               case data:
               case string:
                    if (labelExist){
                         if (!(pushed = addToSymTab(optLabel, "data", lineCnt)))
                              compSuc = FALSE;
                         if(pushed == ERROR){
                              free(optLabel);
                              free(lInd);
                              clean(TRUE, fd);
                              return ERROR;
                         }
                         free(optLabel);
                    }
                    if(command == data){
                         if(!(pushed = pushData(line, lInd, "data", lineCnt)))
                              compSuc = FALSE;
                    }
                    else if(command == string){
                         if(!(pushed = pushData(line, lInd, "string", lineCnt)))
                              compSuc = FALSE;
                    }
                    if(pushed == ERROR){
                         free(lInd);
                         clean(TRUE, fd);
                         return ERROR;
                    }
                    break;
               case external:
                    if (labelExist){
                         isIlegalName(optLabel, lineCnt);
                         wasDefined(optLabel, lineCnt, TRUE, 1);
                         free(optLabel);
                         printf("warnning [line %d]: a label before the directive \".extern\" is meaningless\n", lineCnt);
                    }
                    if (!(pushed = pushExtern(line, lInd, lineCnt)))
                         compSuc = FALSE;
                    else if (pushed == ERROR){
                         free(lInd);
                         clean(TRUE, fd);
                         return ERROR;
                    }
                    break;
               case entry:
               {    
                    boolean check;
                    if(!(check = checkEntry(line, lInd, lineCnt)))
                         compSuc = FALSE;
                    if(check == ERROR){
                         free(optLabel);
                         free(lInd);
                         clean(TRUE, fd);
                         return ERROR;
                    }
                    break;
               }
               }
               break;
          }
          case order:
               if (labelExist){
                    if (!(pushed = addToSymTab(optLabel, "code", lineCnt)))
                         compSuc = FALSE;
                    if(pushed == ERROR){
                         free(optLabel);
                         free(lInd);
                         clean(TRUE, fd);
                         return ERROR;
                    }
                    free(optLabel);
               }
               if(!(pushed = pushInstFirst(line, lInd, lineCnt)))
                    compSuc = FALSE;
               else if(pushed == ERROR){
                    free(lInd);
                    clean(TRUE, fd);
                    return ERROR;
               }
               break;
          }
     }
     if (compSuc == FALSE){
          printf("IS FALSE\n");
          free(lInd);
          printf("OK\n");
          clean(TRUE, fd);
          printf("terminate :)\n");
          return FALSE;
     }
     encPlusIC();
     rewind(fd);
     lineCnt = 0;

     while (FOREVER){
          *lInd = 0;
          lineCnt++;
          labelExist = FALSE;
          tempLine = readLine(fd, LINE_LEN); /* reading a line from the file */
          if (tempLine[0] == '\0') /* end of file */
               break;
          strcpy(line, tempLine);
          free(tempLine);
          
          if(isBlank(line, *lInd))
               continue;
          jumpSpaces(line, lInd);
          if (!(optLabel = readWord(line, lInd))){
               free(lInd);
               free(tempFileName);
               clean(TRUE, fd);
               return ERROR;
          }
          if(isLabel(optLabel))
               labelExist = TRUE;
          if(!labelExist)
               *lInd = 0;

          switch ((type = getStatType(line))){ /* finding the statment type (instruction/ directive/ balnk line/ comment) */
          case comment:
          case blank:
               break;
          default:
          {
               boolean added;
               if(!(added = pushEntry(line, lInd, lineCnt))){
                    compSuc = FALSE;
                    break;
               }
               else if(added == ERROR){
                    free(lInd);
                    clean(TRUE, fd);
                    return ERROR;
               }
               else if(/*isBlank(line, *lInd) || ????? */*lInd == -1)
                    break;
               jumpSpaces(line, lInd);
               while(line[*lInd] != '\0'){
                    if(!(added = pushInstSecond(line, lInd, lineCnt))){
                         compSuc = FALSE;
                         break;
                    }
                    else if(added == ERROR){
                         free(lInd);
                         clean(TRUE, fd);
                         return ERROR;
                    }
                    isThereComma(line, lInd, lineCnt);
               }
               break;
          }
          }
     }
     if(compSuc == TRUE){
          if(!buildOutputFiles(fileName)){
               free(lInd);
               clean(TRUE, fd);
               return ERROR;
          }
     }
     free(lInd);
     clean(TRUE, fd);
     return compSuc;
}

/* clean(): the function get as a parameter the boolean flag that indicates
 whether to free all DATA and Instruction that are stored and if it's TRUE releases all DATA
 and sets DC and IC counters to thier original values*/
void clean(boolean terminate, FILE *fd){
     cleanDC();
     cleanIC();
     printf("IC_DC cleaned\n");
     cleanSymAndExtTab();
     printf("symTab cleaned\n");
     if(terminate){
          rewind(fd);
          printf("rewinded\n");
          fclose(fd);
          printf("closed\n");
     }
     return;
}