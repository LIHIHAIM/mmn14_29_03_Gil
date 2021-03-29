/* [mmn 14 : ]:
in this file: 

author: Uri K.H,   Lihi Haim       Date: 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "word.h"
#include "label.h"
#include "privateDataFuncs.h"

#define MAX_DEC_LEN 5

static int DC;
static word *dataImage[1];

static word *scanDataParams(char *, int *, char *, int);

/* pushData(): the function get as parameters the current line we're reading,
the pointer to the current character, the line's length and the type of the current word.
and pushes the relevent words to memory image */
boolean pushData(char *line, int *lInd, char *type, int lineCnt){
    boolean error = FALSE;
    int i = 0;
    word *scaned = malloc(sizeof(word));
    
    if(!isAlloc(scaned))
        return ERROR;
    
    jumpSpaces(line, lInd);
    while(line[*lInd] != '\0'){
        if(!(scaned = scanDataParams(line, lInd, type, lineCnt))) /* scanning data from the line */{
            printf("NULL RETURNED\n");
            /*error =*/return ERROR;
        }
        else if(scaned->ARE == ERROR_ARE)
            return FALSE;
        if(!isThereComma(line, lInd, lineCnt)){
            free(scaned);
            return FALSE;
        }
        i = 0;
        while(scaned[i].ARE != BLANK_ARE){
            /*printf("DC = %d\n", DC);*/
            if(DC == 0){ /* pushing the data to the data image */
                *dataImage = malloc(sizeof(word));
                DC++;
            }
            else
                *dataImage = realloc(*dataImage, ++DC * sizeof(word));
            if(!isAlloc(*dataImage)){
                free(scaned);
                return ERROR;
            }
            (*dataImage)[DC-1] = scaned[i];
            i++;
        }
        if(error == TRUE)
            continue;
    }
    free(scaned);
    return TRUE;
}
    
/* scanDataParams(): the function get as parameters the current line we're reading,
the pointer to the current word, the line's length and the type of the current word.
and reads the parameters from the line and returns an array of words with the parameters */
static word *scanDataParams(char *line, int *lInd, char *type, int lineCnt)
{
    int size = 1, decimal, temp;
    char curr, currentWrd[MAX_DEC_LEN];
    word *dataForPush = calloc(size, sizeof(word));
    word *error = malloc(sizeof(word));
    error->ARE = ERROR_ARE;

    if (!isAlloc(dataForPush) || !isAlloc(error))
        return NULL;
        
    /*There must be parameters after .data or .stirng in Directive*/
    if (line[*lInd] == '\0'){
        printf("error [line %d]: There must be parameters after the directives \".data\" or \".stirng\"\n", lineCnt);
        free(dataForPush);
        return error;
    }
    /*if it's a string case:*/
    if (strcmp(type, "string") == 0){
        if (line[*lInd] != '\"'){
            printf("error [line %d]: the operand of the directive \".string\" must start with a \"\n", lineCnt);
            free(dataForPush);
            return error;
        }
        (*lInd)++; /* the beginning of the string */
        size = 2;
        while ((curr = line[*lInd]) != '\0' && curr != '\"'){
            if (curr == '\\' && line[*lInd + 1] == '\"'){
                dataForPush = realloc(dataForPush, ++size * sizeof(word));
                if (!isAlloc(dataForPush))
                    return NULL;
                dataForPush[size - 3].wrd = '\"';
                (*lInd) += 2;
                continue;
            }
            dataForPush = realloc(dataForPush, ++size * sizeof(word));
            if (!isAlloc(dataForPush))
                return NULL;
            dataForPush[size - 3].wrd = curr;
            dataForPush[size - 3].ARE = 'A';
            (*lInd)++;
        }
        if (curr == '\0' || curr != '\"'){ /* string ended unaturaly at the end of the array and not by: " */
            printf("error [line %d]: There must be \" at the end of a string\n", lineCnt);
            free(dataForPush);
            return error;
        }
        (*lInd)++;
        if (!isBlank(line, *lInd)){
            free(dataForPush);
            return error;
        }
        dataForPush[size - 2].wrd = '\0';
        dataForPush[size - 2].ARE = 'A';
        dataForPush[size - 1].ARE = BLANK_ARE;
        return dataForPush;
    }

    if (strcmp(type, "data") != 0){
        free(dataForPush);
        return error;
    }
    if (((curr = line[*lInd]) != '+' && curr != '-' && !isdigit(curr))){
        printf("error [line %d]: the decimal number of \".data\" operand must start with a sign (+/-) or a digit\n", lineCnt);
        free(dataForPush);
        return error;
    }
    temp = *lInd;
    while ((curr = line[*lInd]) != '\0' && !isspace(curr) && curr != ',' && *lInd - temp < MAX_DEC_LEN){
        if (isdigit(curr) || ((curr == '-' || curr == '+') && temp == *lInd))
            currentWrd[*lInd - temp] = curr;
        else {
            printf("error [line %d]: the decimal number of the \".data\" directive operand must include only digits (and an optioanl sign: +/- at its start)\n", lineCnt);
            free(dataForPush);
            return error;
        }
        (*lInd)++;
    }
    currentWrd[*lInd - temp] = '\0';
    if (isdigit(line[*lInd]) || (decimal = atoi(currentWrd)) < MIN_WORD_VAL || decimal > MAX_WORD_VAL)
    {
        printf("error [line %d]: the decimal number of the \".data\" directive operand is too %s\n", lineCnt, (decimal > MAX_WORD_VAL || isdigit(line[*lInd + 1]) ? "big" : "small"));
        free(dataForPush);
        return error;
    }
    dataForPush = realloc(dataForPush, ++size * sizeof(word));
    if (!isAlloc(dataForPush))
        return NULL;
    dataForPush[size - 2].wrd = decimal;
    dataForPush[size - 2].ARE = 'A';
    dataForPush[size - 1].ARE = BLANK_ARE;
    return dataForPush;
}

/* cleanDC(): the function get as a parameter the boolean flag that indicates
 whether to free all DATA from datacounter and if it's TRUE releases all DATA
 and sets DC counter to ZERO */
void cleanDC(){
    /*int i = 0;
    while(1){
        if(i < DC)
            free(dataImage[i]);
        i++;
    }*/
    DC = 0;
    *dataImage = NULL;
    return;
}

/* getDC(): the fuction fetches the value of the DATA counter */
int getDC(){
    return DC;
}

/* pushDataToFile(): the function get as a parameter the file we want to write to
and writes everything from DATA's  array */
void pushDataToFile(FILE *fd){
    int i;
    for(i = 0; i < DC; i++)
        fprintf(fd, "%04d %03X %c\n", getIC() + i, ((*dataImage)[i].wrd & 0xFFF), (*dataImage)[i].ARE);
    return;
}

void printDataImage(){
    int i;
    for(i = 0; i < DC; i++)
        printf("%04d %03x %c\n", getIC() + i, ((*dataImage)[i].wrd & 0xFFF), (*dataImage)[i].ARE);
    return;
}