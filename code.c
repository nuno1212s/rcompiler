//
// Created by nuno on 01/12/19.
//

#include "code.h"

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

Instr *compileAtom(char *finalValue, Atom *atom) {

    Instr *instr = malloc(sizeof(Instr));

    instr->finalValueName = finalValue;
    instr->type = I_ATOM;

    instr->atom = atom;

    return instr;

}

LinkedList *compileExpr(char *finalValue, Expr *expr) {

    LinkedList *list = mkEmptyList();

    Instr *instr = malloc(sizeof(Instr));

    instr->finalValueName = finalValue;

    switch (expr->kind) {
        case E_INTEGER:
            instr->type = I_ATOM;
            instr->atom = compileInt(finalValue, expr->attr.value);
            break;
        case E_NAME:
            instr->type = I_ATOM;
            instr->atom = compileVar(finalValue, expr->attr.name);
            break;
        case E_OPERATION:
            instr->type = I_BINOM;
            char *leftSideName = strdup(""),
                    *rightSideName = strdup("");

            instr->binom.atom1 = compileVar(leftSideName);
            instr->binom.atom2 = compileVar(rightSideName);

            instr->binom.operator = expr->attr.op.operator;

            LinkedList *left = compileExpr(leftSideName, expr->attr.op.left),
                    *right = compileExp(rightSideName, expr->attr.op.right);

            left = concatLists(left, right);

            list = concatLists(left, list);
            break;

    }

    concatLast(list, instr);

    return list;
}