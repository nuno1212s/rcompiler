#ifndef COMPILADORES_PRINTABSTREE_H
#define COMPILADORES_PRINTABSTREE_H

#include "abstree.h"
#include "code.h"
#include "mips.h"

void printExpr(Expr *);

void printExprS(Expr *, int);

void printCmd(Command *);

void printCmdS(Command *, int);

void printFunc(Function *);

void printFuncS(Function *, int);

void printInstr(Instr *);

void printInstrs(LinkedList *);

void printMIPS(MIPSFunction *);

#endif //COMPILADORES_PRINTABSTREE_H
