/* [mmn 14 : ]:
in this file: 

author: Uri K.H,   Lihi Haim       Date: 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

typedef enum {blank = 1, comment, directive, order} statType; /* the different statment types */

statType getStatType(char *);