#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lc4.h"

 int main(int argc, char** argv){
 	struct state *nyan = malloc (sizeof(state)); 
 	initial_nyan(nyan);
 	unsigned short print_PC;
 	unsigned short print_addr;
 	unsigned short last_PC ;

	if (argc <= 3) {
		fprintf(stderr, "more arguements!!!!!!!\n");
		return -1;
	}
	for (int i = 3 ; i < argc; i++) {
 		parse(argv[i]);
 	}
	sscanf(argv[2], "%u", &last_PC);

	FILE * output = fopen(argv[1], "w");

	while((nyan->PC) <= last_PC){
			print_PC  = (nyan->PC);
			print_addr = heap[(nyan->PC)];
			execute(nyan);
			printf("%.4X\t%.4X\n", print_PC, print_addr);
			fwrite(&print_PC, sizeof(print_PC), 1, output);
			fwrite(&print_addr, sizeof(print_addr), 1, output);

	}

	fclose(output);
	free(nyan);
	free(heap);
	free(reg);
	


}





 