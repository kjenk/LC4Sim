#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4.h"


#define ENDIAN(x) (((x) << 8) | ((x) >> 8))


void parse(char* obj){
 	unsigned short buffer;
	unsigned short endbuffer;
	unsigned short addr, n, inst, line, findex;
	unsigned short endaddr, endn, endinst, endline, endfindex;
	unsigned short instr_tracker;

	

	FILE *file = fopen(obj, "rb");
	while(fread(&buffer, 2, 1, file) != 0){
		endbuffer = ENDIAN(buffer);	

		if(endbuffer == 0xCADE){//code
			printf("code\n");
			fread(&addr, 2, 1, file);
			endaddr = ENDIAN(addr);
			fread(&n, 2, 1, file);
			endn = ENDIAN(n);
			unsigned short count =0;
			do{
				fread(&inst, 2, 1, file);
				endinst = ENDIAN(inst);
				
				heap[endaddr+count] = endinst;

			printf("\t%.4X\n", endinst);
				instr_tracker++;
				count++;
				endn--;	
			} while(endn>0);

		}
		else if(endbuffer == 0xDADA){//data
			printf("data\n");
			fread(&addr, 2, 1, file);
			endaddr = ENDIAN(addr);
			fread(&n, 2, 1, file);
			endn = ENDIAN(n);
			unsigned short count =0;
			do{
				fread(&inst, 2, 1, file);
				endinst = ENDIAN(inst);
				heap[endaddr+count] = endinst;
			printf("\t%.4X\n", endinst);
			endn--;	
			} while(endn>0);

		}
		else if(endbuffer == 0xC3B7){//symbol
			printf("symbol\n");
			fread(&addr, 2, 1, file);
			endaddr = ENDIAN(addr);
			fread(&n, 2, 1, file);
			endn = ENDIAN(n);
			do{
				fread(&inst, 1, 1, file);
				endinst = ENDIAN(inst);
			printf("\t%.4X\n", endinst);
			endn--;	
			} while(endn>0);

		}
		else if(endbuffer == 0xF17E){//filename
			//fread(&addr, 2, 1, file);
			//endaddr = ENDIAN(addr);
			printf("filename\n");
			fread(&n, 2, 1, file);
			endn = ENDIAN(n);
			do{
				fread(&inst, 1, 1, file);
				endinst = ENDIAN(inst);
			printf("\t%.4X\n", endinst);
			endn--;	
			} while(endn>0);

		}
		else if(endbuffer == 0x715E){//line number
			printf("line number\n");
			fread(&addr, 2, 1, file);
			endaddr = ENDIAN(addr);
			fread(&line, 2, 1, file);
			endline = ENDIAN(line);
			fread(&findex, 2, 1, file);
			endfindex = ENDIAN(findex);

		}
		else{
			printf("You lied saying you'll only test correctly formatted obj files");
		}		
	
	}

	fclose(file);

}


