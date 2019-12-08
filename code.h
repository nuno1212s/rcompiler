//
// Created by nuno on 01/12/19.
//

#ifndef COMPILADORES_CODE_H
#define COMPILADORES_CODE_H

#include "linkedlist.h"
#include "abstree.h"

struct Instr_ {

    enum {
        I_ATOM,
        I_BINOM,
        I_GOTO,
        I_IF_ELSE,
        I_LAB,
        I_ARG
    } type;

    int finalValue;

    union {

        struct Atom_ *atom;

        char *labelName;

        struct {

            struct Atom_ *atom1, *atom2;

            int operator;
        } binom;

        struct {

            char *labelIfTrue, *labelIfFalse;

            int operator;

            struct Atom_ *atom1, *atom2;

        } if_else;

    };

};

struct Atom_ {

    enum {
        A_VAR,
        A_NUMBER,
        A_TEMP
    } type;

    union {
        int value;

        char *varName;
    };

};

typedef struct Instr_ Instr;

typedef struct Atom_ Atom;

LinkedList *compileExpr(Expr *e, int *);

Atom *compileNumber(int num);

Atom *compileVar(char *name);

Atom *compileTemp(int tmp);

Instr *initGoto(char *name);

Instr *initLabel(char *name);

Instr *compileAtom(Atom *atom, int *);

Instr *compileArg(Atom *, int *);

#endif //COMPILADORES_CODE_H
