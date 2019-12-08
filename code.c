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

Atom *compileTemp(int num) {

    Atom *atom = malloc(sizeof(Atom));

    atom->type = A_TEMP;

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

    instr->finalValue = argCounter++;
    *(finalValue) = argCounter - 1;
    instr->type = I_ARG;

    instr->atom = atom;

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

Instr *initLabel(char *name) {
    Instr *instr = initGoto(name);

    instr->type = I_LAB;
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
            //TODO: ??
            break;
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

            Instr* instr = getLast(list);

            dropLast(list);

            char lab_name[64];

            sprintf(lab_name, "LAB%d", labCounter++);

            char *lab_name_dup = strdup(lab_name);

            Instr *lab = initLabel(lab_name_dup);

            LinkedList *cmdList = compileCmd(cmd->attr.ifCmd.cmd);

            sprintf(lab_name, "LAB%d", labCounter++);

            char *lab_name_false_dup = strdup(lab_name);

            Instr *falseLab = initLabel(lab_name_false_dup);

            Instr *if_instr = compileIf(instr->binom.atom1, instr->binom.atom2, instr->binom.operator, lab_name_dup, lab_name_false_dup);

            concatLast(list, if_instr);

            concatLast(list, lab);

            list = concatLists(list, cmdList);

            concatLast(list, falseLab);
            break;
        }
    }

    return list;
}