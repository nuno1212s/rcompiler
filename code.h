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
        I_LAB
    } type;

    char *finalValueName;

    union {

        struct Atom_ *atom;

        struct {

            struct Atom_ *atom1, *atom2;

            int operator;
        } binom;

    };

};

struct Atom_ {

    enum {
        A_VAR,
        A_NUMBER
    } type;

    union {
        int value;

        char *varName;
    };

};

typedef struct Instr_ Instr;

typedef struct Atom_ Atom;

LinkedList *compileExp(Expr *e, char *result);

Atom *compileNumber(int num);

Atom *compileVar(char *name);

Instr *compileAtom(char *name, Atom *atom);

Instr *compileExpr(char *name, Expr *);

#endif //COMPILADORES_CODE_H
