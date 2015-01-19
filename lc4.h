#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


#define ENDIAN(x) (((x) << 8) | ((x) >> 8))

typedef struct state {
	unsigned short PC;//= 0;
	unsigned short NZP; //= 2;
	unsigned short PSR;//= 0;
	unsigned short last_PC;
} state; 


signed short reg[8]; 
unsigned short heap[0xFFFF];
unsigned short last_PC;

void initial_nyan(state *nyan);
void print(state *nyan);
int execute(state *nyan);










 