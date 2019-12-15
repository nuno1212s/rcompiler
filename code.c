#include "code.h"
#include "abstree.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int globalCounter = 0;
int argCounter = 0;
int labCounter = 0;
int returnCounter = 0;

typedef enum {

    L_WHILE,
    L_LAB,
    L_IF,
    L_ELSE

} LABEL_TYPE;

Atom *compileInt(int value) {

    Atom *atom = malloc(sizeof(Atom));

    atom->type = A_NUMBER;

    atom->value = value;

    return atom;
}

Atom *compileVar(char *varName) {

    Atom *atom = malloc(sizeof(Atom));

    atom->type = A_VAR;

    atom->varName = varName;

    return atom;
}

Atom *compileVarAddr(char *varName) {
    Atom *atom = malloc(sizeof(Atom));

    atom->type = A_VAR_ADDR;

    atom->varName = varName;

    return atom;
}

Atom *compileTemp(int num) {

    Atom *atom = malloc(sizeof(Atom));

    atom->type = A_TEMP;

    atom->value = num;

    return atom;
}

Atom *compileArgAtom(int num) {
    Atom *atom = malloc(sizeof(Atom));

    atom->type = A_ARG;

    atom->value = num;

    return atom;
}

Atom *compileString(char *string) {

    Atom *atom = malloc(sizeof(Atom));

    atom->type = A_STRING;

    atom->stringValue = string;

    return atom;
}

Atom *compileFuncType(int func) {

    Atom *atom = malloc(sizeof(Atom));

    atom->type = A_FUNCTION_TYPE;

    atom->value = func;

    return atom;
}

Instr *compileAtom(Atom *atom, int *finalValue) {

    Instr *instr = malloc(sizeof(Instr));

    instr->finalValue = globalCounter++;
    *(finalValue) = globalCounter - 1;
    instr->type = I_ATOM;

    instr->atom = atom;

    return instr;

}

Instr *compileArg(Atom *atom, int *finalValue) {
    Instr *instr = malloc(sizeof(Instr));

    instr->type = I_ATRIB;

    instr->atrib.atom1 = compileArgAtom(argCounter++);
    instr->atrib.atom2 = atom;

    *(finalValue) = argCounter - 1;

    return instr;
}

Instr *initIf(Atom *a1, Atom *a2, int oper, char *labelTrue, char *labelFalse) {

    Instr *instr = malloc(sizeof(Instr));

    instr->type = I_IF_ELSE;
    instr->if_else.operator = oper;
    instr->if_else.atom1 = a1;
    instr->if_else.atom2 = a2;
    instr->if_else.labelIfTrue = labelTrue;
    instr->if_else.labelIfFalse = labelFalse;

    return instr;
}

Instr *initGoto(char *name) {

    Instr *instr = malloc(sizeof(Instr));

    instr->finalValue = -1;
    instr->type = I_GOTO;

    instr->labelName = name;

    return instr;
}

char *getLabelName(LABEL_TYPE t) {

    char lab_name[64];

    switch (t) {

        case L_LAB: {
            sprintf(lab_name, "LAB%d", labCounter++);
            break;
        }

        case L_WHILE: {
            sprintf(lab_name, "WHILE%d", labCounter++);
            break;
        }

        case L_IF: {
            sprintf(lab_name, "IF%d", labCounter++);
            break;
        }
        case L_ELSE: {
            sprintf(lab_name, "ELSE%d", labCounter++);
            break;
        }

    }

    char *lab_name_dup = strdup(lab_name);

    return lab_name_dup;
}

Instr *initLabel(char *name) {
    Instr *instr = initGoto(name);

    instr->type = I_LAB;

    return instr;
}

Instr *initFunction(char *name) {

    Instr *instr = malloc(sizeof(Function));

    instr->type = I_FUNCTION;
    instr->labelName = name;

    return instr;
}

LinkedList *compilePrint(Expr *print) {

    LinkedList *list = mkEmptyList();

    Instr *instr = malloc(sizeof(Instr));

    instr->type = I_PRINT;

    int result = 0;

    LinkedList *cmdList = compileExpr(print->attr.funcCall.args->first->value, &result);

    instr->print.toPrint = compileTemp(result);

    //Remove the used temp
//    globalCounter--;

    list = concatLists(cmdList, list);

    concatLast(list, instr);

    return list;
}

LinkedList *compileRead(Expr *print) {
    LinkedList *list = mkEmptyList();

    Instr *instr = malloc(sizeof(Instr));

    instr->type = I_READ;

    int result = 0;

    LinkedList *cmdList = compileExpr(print->attr.funcCall.args->first->value, &result);

    instr->atom = compileTemp(result);

    //Remove the used temp
//    globalCounter--;

    list = concatLists(cmdList, list);

    concatLast(list, instr);

    return list;
}

LinkedList *compileExpr(Expr *expr, int *finalValue) {

    LinkedList *list = mkEmptyList();

    Instr *instr = malloc(sizeof(Instr));

    int usedTemps = 0;

    switch (expr->kind) {
        case E_INTEGER:
        case E_BOOL_VALUE:
            instr->type = I_ATOM;
            instr->atom = compileInt(expr->attr.value);
            break;
        case E_NAME:
            instr->type = I_ATOM;
            instr->atom = compileVar(expr->attr.name);
            break;
        case E_NAME_ADDR:
            instr->type = I_ATOM;
            instr->atom = compileVarAddr(expr->attr.name);
            break;
        case E_STRING:
            instr->type = I_ATOM;
            instr->atom = compileString(expr->attr.name);
            break;
        case E_OPERATION:
        case E_BOOL:
            instr->type = I_BINOM;

            int leftSideValue = 0, rightSideValue = 0;

            LinkedList *left = compileExpr(expr->attr.op.left, &leftSideValue);

            LinkedList *right = compileExpr(expr->attr.op.right, &rightSideValue);

            usedTemps += 2;

            instr->binom.atom1 = compileTemp(leftSideValue);
            instr->binom.atom2 = compileTemp(rightSideValue);

            instr->binom.operator = expr->attr.op.operator;

            left = concatLists(left, right);

            list = concatLists(left, list);
            break;
        case E_ASSIGNMENT:

            instr->type = I_ATRIB;

            int result = 0;

            if (expr->attr.assignment.value->kind == E_STRING) {

                instr->atrib.atom1 = compileVar(expr->attr.assignment.name);

                instr->atrib.atom2 = compileString(expr->attr.assignment.value->attr.name);

            } else {

                LinkedList *cmdList = compileExpr(expr->attr.assignment.value, &result);

                usedTemps++;

                instr->atrib.atom1 = compileVar(expr->attr.assignment.name);

                instr->atrib.atom2 = compileTemp(result);

                list = concatLists(cmdList, list);
            }

            break;
        case E_FUNC_CALL:

            if (strcmp(expr->attr.funcCall.functionName, "println!") == 0) {

                free(instr);

                return compilePrint(expr);
            } else if (strcmp(expr->attr.funcCall.functionName, "read_line") == 0) {

                free(instr);

                return compileRead(expr);
            }

            instr->type = I_GOTO;
            instr->labelName = expr->attr.funcCall.functionName;

            //Compile all the arguments
            LinkedList *args = expr->attr.funcCall.args;

            Node *first = args->first;

            LinkedList *argList = mkEmptyList();

            while (first != NULL) {

                Expr *arg = first->value;

                int final = 0, argNum = 0;

                LinkedList *expr_list = compileExpr(arg, &final);

                Atom *a = compileTemp(final);

                Instr *instr = compileArg(a, &argNum);

                concatLast(argList, instr);

                list = concatLists(list, expr_list);

                first = first->next;
            }

            argCounter = 0;

            list = concatLists(list, argList);

            //TODO: Handle passing arguments?
            break;
    }

    globalCounter -= usedTemps;

    instr->finalValue = globalCounter++;

    *finalValue = globalCounter - 1;

    concatLast(list, instr);

    return list;
}

LinkedList *compileCmd(Command *cmd) {

    LinkedList *list = mkEmptyList();

    int usedTemps = 0;

    switch (cmd->command) {
        case EXPR_CMD: {
            int result = 0;
            LinkedList *exprList = compileExpr(cmd->attr.value, &result);

            usedTemps++;
            list = concatLists(exprList, list);

            break;
        }
        case IF_CMD: {

            int result = 0;

            LinkedList *exprList = compileExpr(cmd->attr.ifCmd.expr, &result);

            list = concatLists(exprList, list);

            Instr *instr = getLast(list);

//            dropLast(list);

//            globalCounter--;
            usedTemps++;

            char *labName = getLabelName(L_IF), *labNameFalse = getLabelName(L_ELSE);

            Instr *lab = initLabel(labName);

            LinkedList *cmdList = compileCmd(cmd->attr.ifCmd.cmd);

            Instr *falseLab = initLabel(labNameFalse);

            Instr *if_instr = initIf(compileTemp(instr->finalValue), compileInt(1), EQUAL, labName,
                                     labNameFalse);

            concatLast(list, if_instr);

            concatLast(list, lab);

            list = concatLists(list, cmdList);

            concatLast(list, falseLab);
            break;
        }
        case IF_ELSE_CMD: {

            int result = 0;

            LinkedList *exprList = compileExpr(cmd->attr.ifCmd.expr, &result);

            list = concatLists(exprList, list);

            Instr *last_instr = getLast(list);

//            dropLast(list);

//            globalCounter--;
            usedTemps++;

            char *labName = getLabelName(L_IF), *labNameFalse = getLabelName(L_ELSE);

            Instr *lab = initLabel(labName);

            LinkedList *cmdList = compileCmd(cmd->attr.ifElseCmd.cmd),
                    *cmdFalse = compileCmd(cmd->attr.ifElseCmd.elsecmd);

            Instr *falseLab = initLabel(labNameFalse);

            Instr *if_instr = initIf(compileTemp(last_instr->finalValue), compileInt(1), EQUAL, labName,
                                     labNameFalse);

            concatLast(list, if_instr);

            concatLast(list, lab);

            list = concatLists(list, cmdList);

            concatLast(list, falseLab);

            list = concatLists(list, cmdFalse);

            break;
        }
        case WHILE_CMD: {

            char *whileLabName = getLabelName(L_WHILE), *startOfWhile = getLabelName(L_IF), *endOfWhile = getLabelName(
                    L_ELSE);

            Instr *whileLabel = initLabel(whileLabName);

            int result = 0;

            LinkedList *exprList = compileExpr(cmd->attr.whileCmd.expr, &result);

            Instr *finalInstr = getLast(exprList);

//            dropLast(exprList);

//            globalCounter--;
            usedTemps++;

            Instr *if_instr = initIf(compileTemp(finalInstr->finalValue), compileInt(1), EQUAL, startOfWhile,
                                     endOfWhile);

            Instr *startOfWhileInstr = initLabel(startOfWhile);

            Instr *endOfWhileInstr = initLabel(endOfWhile);

            LinkedList *cmd_list = compileCmd(cmd->attr.whileCmd.cmd);

            Instr *go_to = initGoto(whileLabName);

            list = concatLast(list, whileLabel);

            list = concatLists(list, exprList);

            concatLast(list, if_instr);

            concatLast(list, startOfWhileInstr);

            list = concatLists(list, cmd_list);

            concatLast(list, go_to);

            concatLast(list, endOfWhileInstr);

            break;
        }
        case COMPOUND_CMD: {

            LinkedList *cmdList = cmd->attr.compound.commands;

            Node *first = cmdList->first;

            while (first != NULL) {

                LinkedList *compiledCmd = compileCmd(first->value);

                list = concatLists(list, compiledCmd);

                first = first->next;
            }

            break;
        }
        case VAR_CMD: {

            LinkedList *cmdList = cmd->attr.varDef.expr;

            Node *first = cmdList->first;

            while (first != NULL) {

                int result = 0;

                LinkedList *compiledExpr = compileExpr(first->value, &result);

                list = concatLists(list, compiledExpr);

                usedTemps++;

                first = first->next;
            }

            break;
        }
    }

    globalCounter -= usedTemps;

    return list;
}

LinkedList *compileFunction(Function *func) {

    LinkedList *list = mkEmptyList();

    Instr *instrs = initFunction(func->name);

    concatLast(list, instrs);

    Node *first = func->args->first;

    while (first != NULL) {

        Instr *instr = malloc(sizeof(Instr));

        instr->type = I_ATRIB;

        instr->atrib.atom1 = compileVar(first->value);

        instr->atrib.atom2 = compileArgAtom(argCounter++);

        concatLast(list, instr);

        first = first->next;
    }

    list = concatLists(list, compileCmd(func->command));

    char *gotoReturn = strdup("$ra");

    Instr *instr = initGoto(gotoReturn);

    concatLast(list, instr);

    argCounter = 0;
    globalCounter = 0;
    labCounter = 0;
    returnCounter = 0;

    return list;
}