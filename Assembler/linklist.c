#include <stdlib.h>
#include <string.h>
#include "linklist.h"

void insert(Node **pHead,char *val,short addr,short size,short label){
	Node *pNode = (Node*)malloc(sizeof(Node));
	strcpy(pNode->val,val);
	pNode->next = NULL;
	pNode->addr = addr;
	pNode->size = size;
	pNode->label = label;
	if(*pHead == NULL){
		*pHead = pNode;
		return ;
	}
	Node *pCurr = *pHead;
	while(pCurr->next){
		pCurr = pCurr->next;
	}
	pCurr->next = pNode;
}
short find(Node *pHead,char *str){
	while(pHead){
		if(pHead->label){
			char label[10] = {0};
			strncpy(label,pHead->val,pHead->label);
			if(strcmp(label,str) == 0)
				return pHead->addr;
		}
		pHead = pHead->next;
	}
	return -1;
}
