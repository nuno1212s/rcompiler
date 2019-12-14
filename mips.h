//
// Created by nuno on 14/12/19.
//

#ifndef RCOMPILER_MASTER_MIPS_H
#define RCOMPILER_MASTER_MIPS_H

#include "linkedlist.h"
#include "hashtable.h"

struct Function_ {

    HTable *data;

    LinkedList *text;

};

struct MIPSInstr_ {

    enum {

        M_DECLARE_VAR,
        M_DECLARE_STRING,
        M_LOAD_INTO_VAR,
        M_LOAD_INTO_REG,
        M_ADD

    } type;

    union {

        char *varName;

        struct {

            Atom *from, *to;

        } var;

        struct {

            Atom *to, *part1, *part2;

        } add;

    } value;

};

typedef struct MIPSInstr_ MIPSInstr;

typedef struct Function_ MIPSFunction;

MIPSInstr *initMipsInstr();

MIPSFunction* translateThreeRegisters(LinkedList *commands);

#endif //RCOMPILER_MASTER_MIPS_H
