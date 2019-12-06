//
// Created by nuno on 01/12/19.
//

#include "code.h"
#include "abstree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

Instr *compileAtom(int finalValue, Atom *atom) {

    Instr *instr = malloc(sizeof(Instr));

    instr->finalValue = finalValue;
    instr->type = I_ATOM;

    instr->atom = atom;

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
    Instr * instr = initGoto(name);

    instr->type = I_LAB;
}

int globalCounter = 1;

LinkedList *compileExpr(int finalValue, Expr *expr) {

    LinkedList *list = mkEmptyList();

    Instr *instr = malloc(sizeof(Instr));

    switch (expr->kind) {
        case E_INTEGER:
            instr->type = I_ATOM;
            instr->atom = compileInt(expr->attr.value);
            break;
        case E_NAME:
            instr->type = I_ATOM;
            instr->atom = compileVar(expr->attr.name);
            break;
        case E_OPERATION:
            instr->type = I_BINOM;

            int leftSideValue, rightSideValue;

            LinkedList *left = compileExpr(0, expr->attr.op.left),
                    *right = compileExpr(0, expr->attr.op.right);

            leftSideValue = ((Instr *) getLast(left))->finalValue;
            rightSideValue = ((Instr *) getLast(right))->finalValue;

            instr->binom.atom1 = compileTemp(leftSideValue);
            instr->binom.atom2 = compileTemp(rightSideValue);

            instr->binom.operator = expr->attr.op.operator;

            left = concatLists(left, right);

            list = concatLists(left, list);
            break;

    }

    instr->finalValue = globalCounter++;

    concatLast(list, instr);

    return list;
}