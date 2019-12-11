//
// Created by nuno on 01/12/19.
//

#include "code.h"
#include "abstree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int globalCounter = 1;
int argCounter = 1;
int labCounter = 1;
int returnCounter = 1;

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

char *getLabelName() {

    char lab_name[64];

    sprintf(lab_name, "LAB%d", labCounter++);

    char *lab_name_dup = strdup(lab_name);

    return lab_name_dup;
}

Instr *initLabel(char *name) {
    Instr *instr = initGoto(name);

    instr->type = I_LAB;

    return instr;
}

LinkedList *compileExpr(Expr *expr, int *finalValue) {

    LinkedList *list = mkEmptyList();

    Instr *instr = malloc(sizeof(Instr));

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
        case E_OPERATION:
        case E_BOOL:
            instr->type = I_BINOM;

            int leftSideValue = 0, rightSideValue = 0;

            LinkedList *left = compileExpr(expr->attr.op.left, &leftSideValue);

            LinkedList *right = compileExpr(expr->attr.op.right, &rightSideValue);

            instr->binom.atom1 = compileTemp(leftSideValue);
            instr->binom.atom2 = compileTemp(rightSideValue);

            instr->binom.operator = expr->attr.op.operator;

            left = concatLists(left, right);

            list = concatLists(left, list);
            break;
        case E_STRING:
            instr->type = I_ATOM;
            instr->atom = compileVar(expr->attr.name);
            break;
        case E_ASSIGNMENT:

            instr->type = I_ATRIB;

            instr->atrib.atom1 = compileVar(expr->attr.assignment.name);

            int result = 0;

            LinkedList *cmdList = compileExpr(expr->attr.assignment.value, &result);

            instr->atrib.atom2 = compileTemp(result);

            list = concatLists(cmdList, list);

            list = concatLast(list, instr);

            instr->finalValue = -1;

            return list;
        case E_FUNC_CALL:
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

            argCounter = 1;

            list = concatLists(list, argList);

            //TODO: Handle passing arguments?
            break;
    }

    instr->finalValue = globalCounter++;

    *finalValue = globalCounter - 1;

    concatLast(list, instr);

    return list;
}

LinkedList *compileCmd(Command *cmd) {

    LinkedList *list = mkEmptyList();

    switch (cmd->command) {
        case EXPR_CMD: {
            int result = 0;
            LinkedList *exprList = compileExpr(cmd->attr.value, &result);

            list = concatLists(exprList, list);

            break;
        }
        case IF_CMD: {

            int result = 0;

            LinkedList *exprList = compileExpr(cmd->attr.ifCmd.expr, &result);

            list = concatLists(exprList, list);

            Instr *instr = getLast(list);

            dropLast(list);

            char *labName = getLabelName(), *labNameFalse = getLabelName();

            Instr *lab = initLabel(labName);

            LinkedList *cmdList = compileCmd(cmd->attr.ifCmd.cmd);

            Instr *falseLab = initLabel(labNameFalse);

            Instr *if_instr = initIf(instr->binom.atom1, instr->binom.atom2, instr->binom.operator, labName,
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

            dropLast(list);

            char *labName = getLabelName(), *labNameFalse = getLabelName();

            Instr *lab = initLabel(labName);

            LinkedList *cmdList = compileCmd(cmd->attr.ifElseCmd.cmd),
                    *cmdFalse = compileCmd(cmd->attr.ifElseCmd.elsecmd);

            Instr *falseLab = initLabel(labNameFalse);

            Instr *if_instr = initIf(last_instr->binom.atom1, last_instr->binom.atom2, last_instr->binom.operator, labName,
                                     labNameFalse);

            concatLast(list, if_instr);

            concatLast(list, lab);

            list = concatLists(list, cmdList);

            concatLast(list, falseLab);

            list = concatLists(list, cmdFalse);

            break;
        }
        case WHILE_CMD: {

            char *whileLabName = getLabelName(), *startOfWhile = getLabelName(), *endOfWhile = getLabelName();

            Instr *whileLabel = initLabel(whileLabName);

            int result = 0;

            LinkedList *exprList = compileExpr(cmd->attr.whileCmd.expr, &result);

            Instr *finalInstr = getLast(exprList);

            dropLast(exprList);

            Instr *if_instr = initIf(finalInstr->binom.atom1, finalInstr->binom.atom2, finalInstr->binom.operator, startOfWhile, endOfWhile);

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

                first = first->next;
            }

            break;
        }
    }

    return list;
}

LinkedList *compileFunction(Function *func) {

    LinkedList *list = mkEmptyList();

    Instr *instrs = initLabel(func->name);

    concatLast(list, instrs);

    Node *first = func->args->first;

    while (first != NULL) {

        Instr* instr = malloc(sizeof(Instr));

        instr->type = I_ATRIB;

        instr->atrib.atom1 = compileVar(first->value);

        instr->atrib.atom2 = compileArgAtom(argCounter++);

        concatLast(list, instr);

        first = first->next;
    }

    argCounter = 1;

    list = concatLists(list, compileCmd(func->command));

    return list;
}