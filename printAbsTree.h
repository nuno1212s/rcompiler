#ifndef COMPILADORES_PRINTABSTREE_H
#define COMPILADORES_PRINTABSTREE_H

#include "abstree.h"

void printExpr(Expr *);

void printExprS(Expr *, int);

void printCmd(Command *);

void printCmdS(Command *, int);

void printFunc(Function *);

void printFuncS(Function *, int);

#endif //COMPILADORES_PRINTABSTREE_H
