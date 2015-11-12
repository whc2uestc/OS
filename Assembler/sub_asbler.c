#include "sub_asbler.h"
#include <ctype.h>

short judge_field(char *field){
	if(field == NULL)
		return 0;
	if(strcmp(field,"A1") == 0)
		return A1;
	else if(strcmp(field,"A2") == 0)
		return A2;
	else if(strcmp(field,"R1") == 0)
		return R1;
	else if(strcmp(field,"R2") == 0)
		return R2;
	else{
		return 4;
	}
}

void do_assembler(char *str){
	int len = strlen(str);
	char instruction;
	int i;
	short reg1=0,reg2=0;
	short data1,data2;
	char *result = NULL;
	result = strtok(&str[8]," ");
	if(result != NULL){
		if(strcmp(result,"DATA") == 0){
			result = strtok(NULL," ");
			short num;
			if(isalpha(result[0])){
				if((num=find(pHead,result)) == -1)
					num = atoi(result);
			}
			else
				num = atoi(result);
			fwrite(&num,1,2,fw);
		}else {
			if(strcmp(result,"LOAD") == 0)
				instruction = LOAD;		
			else if(strcmp(result,"LOADI") == 0)
				instruction = LOADI;
			else if(strcmp(result,"STORE") == 0)
				instruction = STORE;
			else if(strcmp(result,"STOREI") == 0)
				instruction = STOREI;
			else if(strcmp(result,"JMP") == 0)
				instruction = JMP;
			else if(strcmp(result,"JMPO") == 0)
				instruction = JMPO;
			else if(strcmp(result,"JMPZ") == 0)
				instruction = JMPZ;
			else if(strcmp(result,"JMPN") == 0)
				instruction = JMPN;
			else if(strcmp(result,"ADD") == 0)
				instruction = ADD;
			else if(strcmp(result,"ADDR") == 0)
				instruction = ADDR;
			else if(strcmp(result,"SUB") == 0)
				instruction = SUB;
			else if(strcmp(result,"SUBR") == 0)
				instruction = SUBR;
			else if(strcmp(result,"OUTI") == 0)
				instruction = OUTI;
			else if(strcmp(result,"OUTR") == 0)
				instruction = OUTR;
			else if(strcmp(result,"OUTIC") == 0)
				instruction = OUTIC;
			else if(strcmp(result,"OUTRC") == 0)
				instruction = OUTRC;
			else if(strcmp(result,"OUT") == 0)
				instruction = OUT;
			else if(strcmp(result,"OUTC") == 0)
				instruction = OUTC;
			else if(strcmp(result,"HALT") == 0)
				instruction = HALT;
			fwrite(&instruction,1,1,fw);
			result = strtok(NULL," ");
			if(result == NULL){
				char bin_reg = 0;
				fwrite(&bin_reg,1,1,fw);
				return ;
			}
			char *result1 = strtok(result,",");
			if(result1 != NULL){
				char *result2 = strtok(NULL,",");
				reg1 = judge_field(result1);
				reg2 = judge_field(result2);
				if(reg1 < 4 && reg2 < 4){
					char bin_reg = (reg2<<6) + reg1;
					fwrite(&bin_reg,1,1,fw);
				}else if(reg1 < 4){
					char bin_reg = reg1;
					fwrite(&bin_reg,1,1,fw);
					short num;
					if((num=find(pHead,result2)) == -1)
						num = atoi(result2);
					fwrite(&num,1,2,fw);
				}else if(reg2 < 4){
					char bin_reg = reg2<<6;
					fwrite(&bin_reg,1,1,fw);
					short num;
					if((num=find(pHead,result1)) == -1)
						num = atoi(result1);
					fwrite(&num,1,2,fw);
				}	
			}
		}	
	}
}

