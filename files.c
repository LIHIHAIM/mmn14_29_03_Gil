/* [mmn 14 : ]:
in this file: 

author: Uri K.H,   Lihi Haim       Date: 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "util.h"
#include "privateDataFuncs.h"
#include "privateInstFuncs.h"
#include "privateSymTabFuncs.h"

#define RAM_SIZE 4096

/* openf(): Open a given file in a given status and returns the pointer to the file. */
FILE *openf(char *file, char *status){
     FILE *fd = fopen(file, status);
     if(!fd){
          printf("error : could not open file: \"%s\"\n",file);
          return NULL;
     }
     return fd;
}

/* readLine(): Reads a line (length 80 characters) from a given file pointer - fd, 
 to a dinamic array and returns the array. */
char *readLine(FILE *fd, int len){
     char *line = calloc(len, sizeof(char));
     fgets(line,len,fd);
     return line;
}

/* buildOutputFiles(): the function get as a parameter the name we want for a new file
creats the file and pushes the data into the file */
boolean buildOutputFiles(char *fileName){
     FILE *fd;
     char *tempFileName = malloc(strlen(fileName) + MAX_SUFIX_LEN + 1);
     if(getIC() + getDC() >= RAM_SIZE){
          printf("error: file is too long\n");
          free(tempFileName);
          return FALSE;
     }
     fd = openf(addSufix(tempFileName, ".ob", fileName), "w");
     if(!fd)
          return FALSE;
     fprintf(fd, "\t%d %d\n", getIC()-OS_MEM, getDC());
     pushInstsToFile(fd);
     pushDataToFile(fd);
     fclose(fd);
     
     if(entryExist()){
          fd = openf((addSufix(tempFileName, ".ent", fileName)), "w");
          if(!fd)
               return FALSE;
          pushEntryToFile(fd);
          fclose(fd);
     }
     if(externalExist()){
          fd = openf((addSufix(tempFileName, ".ext", fileName)), "w");
          if(!fd)
               return FALSE;
          pushExternToFile(fd);
          fclose(fd);
     }
     return TRUE;
}

/* buildOutputFiles(): the function get as a parameter the destination global array,
 an extension to be added to name and combines them and pushes them to the destination array
 without harming 'name' itself and returns the full string's address*/

char *addSufix(char *dest, char *suffix, char *name){
     if(!dest || !suffix || !name)
          return NULL;
     strcpy(dest, name);
     strcat(dest, suffix);
     return dest;
}