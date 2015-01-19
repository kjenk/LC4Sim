#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lc4.h"


/* notes to self

F = 15 = 1111
7 = 0111
3 = 0011

*/
#define INSN_OP(I) ((I) >> 12)
#define INSN_11_9(I) (((I) >> 9) & 0x7)//d
#define INSN_8_6(I) (((I) >> 6) & 0x7)//s
#define INSN_2_0(I) ((I) & 0x7)//t
#define INSN_5_3(I) (((I) >> 3) & 0x7)//arit + logic
#define INSN_8_7(I) (((I) >> 7) & 0x3)//cmp
#define INSN_11(I) (((I) >> 11) & 1)//jsr jmp
#define INSN_5_4(I) (((I) >> 4) & 0x3)
#define INSN_IMM9(I) ((I) & 0x01FF)
#define MSB9(I) (((I) >> 8) & 1)//msb imm9
#define SEXT9(I) ((I) | 0xFE00)//use with imm9 only

#define IMM5(I) ((I) & 0x001F)
#define IMM7(I) ((I) & 0x007F)
#define IMM11(I) ((I) & 0x07FF)
#define IMM6(I) ((I) & 0x003F)

#define MSB5(I) (((I) >> 4) & 1)//msb imm9
#define SEXT5(I) ((I) | 0xFFE0)//use with imm9 only
#define MSB6(I) (((I) >> 5) & 1)//msb imm9
#define SEXT6(I) ((I) | 0xFFC0)//use with imm9 only
#define MSB7(I) (((I) >> 6) & 1)//msb imm9
#define SEXT7(I) ((I) | 0xFF80)//use with imm9 only
#define MSB11(I) (((I) >> 10) & 1)//msb imm9
#define SEXT11(I) ((I) | 0xF800)//use with imm9 only







#define ENDIAN(x) (((x) << 8) | ((x) >> 8))

#define USER_CODE_S 0x0000
#define USER_DATA_S 0x2000
#define OS_CODE_S 0x8000
#define OS_DATA_S 0xA000

#define USER_CODE_E 0x1FFF//8191
#define USER_DATA_E 0x7FFF
#define OS_CODE_E 0x9FFF
#define OS_DATA_E 0xFFFF


void initial_nyan(state *nyan){
	nyan->PC  = 0;
	nyan->NZP = 2;
	nyan->PSR = 0;
}

void print(state *nyan){
		printf("PC: %X\n", nyan->PC);
		printf("NZP: %d\n", nyan->NZP);
		printf("PSR: %d\n", nyan->PSR);

}



void update_nzp(state * nyan, signed short value)
{
	if (value < 0) {
		nyan->NZP = 4;
	}
	else if (value == 0) {
		nyan->NZP = 2;
	}
	else {
		nyan->NZP = 1;
	}
}

signed short sex9(signed short imm9){
		if (MSB9(imm9) == 1) {
			signed short sign_extend = SEXT9(imm9);
			return sign_extend;
		}
		else{
			return imm9;			
		}

}

signed short sex6(signed short imm6){
		if (MSB6(imm6) == 1) {
			signed short sign_extend = SEXT6(imm6);
			return sign_extend;
		}
		else{
			return imm6;			
		}

}
signed short sex5(signed short imm5){
		if (MSB5(imm5) == 1) {
			signed short sign_extend = SEXT5(imm5);
			return sign_extend;
		}
		else{
			return imm5;			
		}

}
signed short sex7(signed short imm7){
		if (MSB7(imm7) == 1) {
			signed short sign_extend = SEXT7(imm7);
			return sign_extend;
		}
		else{
			return imm7;			
		}

}
signed short sex11(signed short imm11){
		if (MSB11(imm11) == 1) {
			signed short sign_extend = SEXT11(imm11);
			return sign_extend;
		}
		else{
			return imm11;			
		}

}



void nop(state * nyan, unsigned short instruction){
	nyan->PC = (nyan->PC) + 1; 

}

void brn(state * nyan, unsigned short instruction){
	signed short imm9 = INSN_IMM9(instruction);
	signed short sign_extend = sex9(imm9);
	if(nyan->NZP == 4){
		nyan->PC = nyan->PC + 1 + sign_extend;
	}
	else{
		nyan->PC = nyan->PC + 1;
	}
	
}
void brnz(state * nyan, unsigned short instruction){
	signed short imm9 = INSN_IMM9(instruction);
	signed short sign_extend = sex9(imm9);
	if((nyan->NZP == 4) || (nyan->NZP == 2)||(nyan->NZP == 6)){
		nyan->PC = nyan->PC + 1 + sign_extend;
	}
	else{
		nyan->PC = nyan->PC + 1;
	}	
	
}
void brnp(state * nyan, unsigned short instruction){
	signed short imm9 = INSN_IMM9(instruction);
	signed short sign_extend = sex9(imm9);
	if((nyan->NZP == 4) || (nyan->NZP == 1) || (nyan->NZP == 5)){
		nyan->PC = nyan->PC + 1 + sign_extend;
	}
	else{
		nyan->PC = nyan->PC + 1;
	}	
}
void brz(state * nyan, unsigned short instruction){
	signed short imm9 = INSN_IMM9(instruction);
	signed short sign_extend = sex9(imm9);
	if(nyan->NZP == 2){
		nyan->PC = nyan->PC + 1 + sign_extend;
	}
	else{
		nyan->PC = nyan->PC + 1;
	}	
}
void brzp(state * nyan, unsigned short instruction){
	signed short imm9 = INSN_IMM9(instruction);
	signed short sign_extend = sex9(imm9);
	if((nyan->NZP == 2) || (nyan->NZP == 1)||(nyan->NZP == 3)){
		nyan->PC = nyan->PC + 1 + sign_extend;
	}
	else{
		nyan->PC = nyan->PC + 1;
	}	
}
void brp(state * nyan, unsigned short instruction){
	signed short imm9 = INSN_IMM9(instruction);
	signed short sign_extend = sex9(imm9);
	if(nyan->NZP == 1){
		nyan->PC = nyan->PC + 1 + sign_extend;
	}
	else{
		nyan->PC = nyan->PC + 1;
	}	
}
void brnzp(state * nyan, unsigned short instruction){
	signed short imm9 = INSN_IMM9(instruction);
	signed short sign_extend = sex9(imm9);
	if((nyan->NZP > 0) && (nyan->NZP < 8)){
		nyan->PC = nyan->PC + 1 + sign_extend;
	}
	else{
		nyan->PC = nyan->PC + 1;
	}	
}


/* math */
void add(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	unsigned short rt = INSN_2_0(instruction);
	reg[rd] = reg[rs] + reg[rt];
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);

	
}
void mul(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	unsigned short rt = INSN_2_0(instruction);
	reg[rd] = reg[rs] * reg[rt];
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);
	
}
void sub(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	unsigned short rt = INSN_2_0(instruction);
	reg[rd] = reg[rs] - reg[rt];
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);	
	
}
void divide(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	unsigned short rt = INSN_2_0(instruction);
	reg[rd] = ((short)reg[rs] / (short)reg[rt]);
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);	
}
void add_imm(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	signed short imm5 = IMM5(instruction);
	reg[rd] = reg[rs] + sex5(imm5);
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);
}

/* cmp */
void cmp(state * nyan, unsigned short instruction){
	unsigned short rs = INSN_11_9(instruction);
	unsigned short rt = INSN_2_0(instruction);
	update_nzp(nyan, ((signed short)(reg[rs]-reg[rt])));
	nyan->PC = nyan->PC + 1;
	
}
void cmpu(state * nyan, unsigned short instruction){
	unsigned short rs = INSN_11_9(instruction);
	unsigned short rt = INSN_2_0(instruction);
	update_nzp(nyan, ((unsigned short)(reg[rs]-reg[rt])));
	nyan->PC = nyan->PC + 1;
}
void cmpi(state * nyan, unsigned short instruction){
	unsigned short rs = INSN_11_9(instruction);
	signed short imm7 = IMM7(instruction);
 	update_nzp(nyan, ((signed short)(reg[rs]-sex7(imm7))));
	nyan->PC = nyan->PC + 1;
}
void cmpiu(state * nyan, unsigned short instruction){
	unsigned short rs = INSN_11_9(instruction);
	unsigned short uimm7 = (instruction & 0x007F);

 	update_nzp(nyan, ((unsigned short)(reg[rs]-uimm7)));
	nyan->PC = nyan->PC + 1;
}

/* jsr */
void jsr(state * nyan, unsigned short instruction){
	signed short imm11 = IMM11(instruction);
	reg[7]= nyan->PC + 1;
	update_nzp(nyan, (unsigned short)(reg[7]));
	 nyan->PC = ( nyan->PC & 0x8000) | (imm11 <<4);

}
void jsrr(state * nyan, unsigned short instruction){
	unsigned short rs = INSN_8_6(instruction);
	reg[7]= nyan->PC + 1;
	unsigned short foo = (unsigned short)(reg[rs]);
	update_nzp(nyan, (unsigned short)(reg[7]));
	 nyan->PC = foo;



}

/* logic */
void and(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	unsigned short rt = INSN_2_0(instruction);
	reg[rd] = reg[rs] & reg[rt];
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);
	
}
void not(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	reg[rd] = ~(reg[rs]);
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);	
}
void or(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	unsigned short rt = INSN_2_0(instruction);
	reg[rd] = reg[rs] | reg[rt];
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);	
	
}
void xor(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	unsigned short rt = INSN_2_0(instruction);
	reg[rd] = reg[rs] ^ reg[rt];
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);	
	
}
void and_imm(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	signed short imm5 = IMM5(instruction);
	reg[rd] = reg[rs] & sex5(imm5);
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);
}

/* Load store */
void ldr(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	signed short imm6 = IMM6(instruction);
	reg[rd] = heap[reg[rs] + sex6(imm6)];
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);
}
void str(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	signed short imm6 = IMM6(instruction);
	heap[reg[rs]+sex6(imm6)] = reg[rd];
	nyan->PC = nyan->PC + 1; 
	update_nzp(nyan, reg[rd]);
}

/* RTI */
void rti(state * nyan, unsigned short instruction){
	nyan->PC = (unsigned short)reg[7];
	nyan->PSR = 0; 
	
}

void CONST(state * nyan, unsigned short instruction){
		unsigned short rd = INSN_11_9(instruction);
		signed short imm9 = INSN_IMM9(instruction);
		reg[rd] = sex9(imm9);
		//printf("#####%d#####", imm9);
		//exit(1);
		nyan->PC = nyan->PC + 1; 
		update_nzp(nyan, reg[rd]);

			
}
void sll(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);

	unsigned short uimm4 = (instruction & 0x000F);
	reg[rd] = reg[rs] << uimm4;
	update_nzp(nyan, reg[rd]);
	nyan->PC = nyan->PC + 1; 
}
void sra(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);

	unsigned short uimm4 = (instruction & 0x000F);
	reg[rd] = (signed short)(reg[rs]) >> uimm4;///signed???
	update_nzp(nyan, reg[rd]);
	nyan->PC = nyan->PC + 1; 
}
void srl(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);

	unsigned short uimm4 = (instruction & 0x000F);
	reg[rd] = (unsigned short)(reg[rs]) >> uimm4;
	update_nzp(nyan, reg[rd]);
	nyan->PC = nyan->PC + 1; 
}
void mod(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short rs = INSN_8_6(instruction);
	unsigned short rt = INSN_2_0(instruction);
	reg[rd] = (signed short)(reg[rs])%(signed short)(reg[rt]);
	update_nzp(nyan, reg[rd]);
	nyan->PC = nyan->PC + 1; 

}
void jmpr(state * nyan, unsigned short instruction){
	unsigned short rs = INSN_8_6(instruction);
	nyan->PC = (unsigned short)reg[rs]; 
}
void jmp(state * nyan, unsigned short instruction){

	signed short imm11 = IMM11(instruction);

	nyan->PC = ((nyan->PC) + 1 + (sex11(imm11))); 

}
void hiconst(state * nyan, unsigned short instruction){
	unsigned short rd = INSN_11_9(instruction);
	unsigned short uimm8 = (instruction & 0x00FF);
		reg[rd] = (((signed short)reg[rd]) & 0x00FF) | (uimm8 << 8);
	nyan->PC = nyan->PC + 1;
	update_nzp(nyan, reg[rd]);

	
}
void trap(state * nyan, unsigned short instruction){
	reg[7]= nyan->PC + 1;
	update_nzp(nyan, reg[7]);
	unsigned short uimm8 = (instruction & 0x00FF);
	nyan->PC = 0x8000 | uimm8;
	nyan->PSR = 1;
}


int execute(state *nyan){

	unsigned short PC = nyan->PC;//current PC
	unsigned short instruction = heap[PC];//16bit instruction from memory
	// 	if (nyan->PSR == 0) {
	// 		if ((PC >= OS_CODE_S && PC <= OS_CODE_E)&&(PC >= USER_CODE_S && PC <= USER_CODE_E)) {
	// 			return PC;
	// 		}

	// 		else if((PC >= OS_DATA_S && PC <= OS_DATA_E)&&(PC >= USER_DATA_S && PC <= USER_DATA_E)) {
	// 			return PC;
	// 		}
	// 		else if(nyan->PSR == 1) {
	// 			return PC;
	// 		}
		
	// 	else {
	// 		printf("%X\t is a no no , go stand in the corner", PC);
	// 		exit(1);
	// 	}
	// }
	unsigned short opt_code = INSN_OP(instruction);
	printf("%.X\n", instruction);
	switch(opt_code){
		case 0://branch
			switch(INSN_11_9(instruction)){
				case 0:
					nop(nyan, instruction);
					break;
				case 1:
					brp(nyan, instruction);				
					break;
				case 2:
					brz(nyan, instruction);				
					break;					
				case 3:
					brzp(nyan, instruction);				
					break;
				case 4:
					brn(nyan, instruction);				
					break;	
				case 5:
					brnp(nyan, instruction);				
					break;						
				case 6:
					brnz(nyan, instruction);				
					break;																			
				default:
					brnzp(nyan, instruction);				
					break;
			}
			break;
		case 1://math
			switch(INSN_5_3(instruction)){
				case 0:
					add(nyan, instruction);
					break;
				case 1:
					mul(nyan, instruction);				
					break;
				case 2:
					sub(nyan, instruction);				
					break;					
				case 3:
					divide(nyan, instruction);				
					break;				
				default:
					add_imm(nyan, instruction);				
					break;
			}
			break;
		case 2://CMP
			switch(INSN_8_7(instruction)){
				case 0:
					cmp(nyan, instruction);
					break;
				case 1:
					cmpu(nyan, instruction);				
					break;
				case 2:
					cmpi(nyan, instruction);				
					break;					
				case 3:
					cmpiu(nyan, instruction);				
					break;				
			}		
			break;
		case 4://JSR
			switch(INSN_11(instruction)){
				case 0:
					jsrr(nyan, instruction);
					break;
				case 1:
					jsr(nyan, instruction);				
					break;
			}		
			break;
		case 5://logic
			switch(INSN_5_3(instruction)){
				case 0:
					and(nyan, instruction);
					break;
				case 1:
					not(nyan, instruction);				
					break;
				case 2:
					or(nyan, instruction);				
					break;					
				case 3:
					xor(nyan, instruction);				
					break;				
				default:
					and_imm(nyan, instruction);				
					break;
			}		
			break;
		case 6://LDR
			ldr(nyan, instruction);
			break;
		case 7://STR
			str(nyan, instruction);
			break;
		case 8://RTI
			rti(nyan, instruction);
			break;
		case 9://CONST
			CONST(nyan, instruction);
			break;
		case 10://SLL stuff
			switch(INSN_5_4(instruction)){
				case 0:
					sll(nyan, instruction);
					break;
				case 1:
					sra(nyan, instruction);				
					break;
				case 2:
					srl(nyan, instruction);				
					break;					
				case 3:
					mod(nyan, instruction);				
					break;				
			}		
			break;
		case 12://JMP
			switch(INSN_11(instruction)){
				case 0:
					jmpr(nyan, instruction);
					break;
				case 1:
					jmp(nyan, instruction);				
					break;	
			}	
			break;
		case 13://HICONST
			hiconst(nyan, instruction);
			break;
		case 15://TRAP
			trap(nyan, instruction);
			break;
		default:
			printf("I wonder why hmmm");
	}

}


