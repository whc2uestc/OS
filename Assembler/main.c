#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sub_asbler.h"
#include "linklist.h"
FILE *fw;
Node *pHead = NULL;
short check_is_reg(char *str){
	if(str == NULL)
		return 1;
	if(strcmp(str,"A1")==0 || strcmp(str,"A2")==0 || strcmp(str,"R1")==0 || strcmp(str,"R2")==0)
		return 1;
	else 
		return 0;
}


short cal_size(char *str,short *label){
	char instr[255] = {0};
	int i;
	strcpy(instr,str);
	for(i=0;i<8;i++){
		if(instr[i]==' ')
			break;
	}
	*label = i;
	short size = 0;
	char *result = strtok(&instr[8]," ");
	if(result != NULL){
		if(strcmp(result,"DATA") == 0){
			size += 2;
			return size;
		}
		else{
			size += 2;
		}
		result = strtok(NULL," ");
		if(result != NULL){
			char *result1 = strtok(result,",");
			char *result2 = strtok(NULL,",");
			if(check_is_reg(result1) && check_is_reg(result2))
				return size;
			else{
				return size+2;
			}
		}
	}
	return size;
}

void traverse(Node *pHead){
	while(pHead){
		do_assembler(pHead->val);
		pHead = pHead->next;
	}
}

int main(){
	fw = stdout;
	char str[255] = {0};
	int i = 0;
	
	short addr = 0,label = 0,size = 0;
	while(fgets(str,255,stdin)){
		for(i=0;i<255&&str[i]!='#'&&str[i]!='\n';i++)
			;
		if(str[i] == '#' || str[i]=='\n')
			str[i] = 0;
		if(i > 0){
			addr += size; 
			size = cal_size(str,&label);
			insert(&pHead,str,addr,size,label);
		}
	}
	traverse(pHead);
	return 0;
}
