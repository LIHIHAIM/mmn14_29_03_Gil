/* [mmn 14: buildWord.c]:
in this file: functions for RAM memory cells building.

author: Uri K.H,   Lihi Haim    Date: 21.3.2021 
ID: 215105321,     313544165    Tutor: Danny Calfon */

#include <stdlib.h>
#include "util.h"
#include "buildWord.h"

#define FUNCT_SIZE 4 /* the number of bits the funct field is using */
#define TARGET_SIZE 2 /* the number of bits the target field is using */
#define SOURCE_SIZE 2 /* the number of bits the source field is using */

/* buildMainWord(): the function get as parameters the opcode, funct (for representing an instruction word)
and the target and source operands addressing methods. the function returns the word which represent
such an instruction word with such operand types. */
word buildMainWord(int opcode, int funct, int source, int target){
    word w;
    w.wrd = target+ (source << TARGET_SIZE) + 
            (funct << (TARGET_SIZE + SOURCE_SIZE)) + 
            (opcode << (FUNCT_SIZE + TARGET_SIZE + SOURCE_SIZE));
    w.ARE = 'A';
    return w;
}

/* buildRegWord(): the function get as parameters the name of the register (r0, r1, ... r7)
and returns the word which represent it. 
(example: for register "r2" the returnd word will be: 000000000100 (in binary) and value 'A')*/
word buildRegWord(char *reg){
    word w ;
    w.wrd = 1 << atoi(reg+1);
    w.ARE = 'A';
    return w;
}