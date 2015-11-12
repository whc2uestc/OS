#ifndef LINKLIST_H_
#define LINKLIST_H_
typedef struct Node{
	char val[255]; // the string of the instruction
	short addr;	   // the address of the instruction in the memory
	short size;    // the size of the instruction(2 bytes or 4 bytes)
	short label;   // the size of the label(label=0 if it's not exist) 
	struct Node *next; // the next node of the list
}Node;

void insert(Node **pHead,char *val,short addr,short size,short label);
short find(Node *pHead,char *str);
#endif