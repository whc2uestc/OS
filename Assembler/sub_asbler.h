#ifndef SUB_ASBLER_H_
#define SUB_ASBLER_H_
#include <string.h>
#include "linklist.h"
#include <stdlib.h>
#include <stdio.h>
#include "cfg.h"
extern FILE *fw;
extern Node *pHead;
short judge_field(char *field);
void do_assembler(char *str);

#endif