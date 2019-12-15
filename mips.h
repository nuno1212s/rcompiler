#ifndef RCOMPILER_MASTER_MIPS_H
#define RCOMPILER_MASTER_MIPS_H

#include "linkedlist.h"
#include "hashtable.h"
#include "code.h"

struct Function_ {

    HTable *data;

    LinkedList *text;

};

struct MIPSInstr_ {

    enum {

        M_DECLARE_VAR,
        M_DECLARE_STRING,
        M_LOAD_INTO_VAR_CONST,
        M_LOAD_INTO_VAR_OTHER,
        M_LOAD_INTO_REG_CONST,
        M_LOAD_INTO_REG_VAR,
        M_LOAD_ADRESS_INTO_REG,
        M_MOVE,
        M_ADD,
        M_SUB,
        M_DIV,
        M_MULT,
        M_REMAINDER,
        M_EQ,
        M_NOT_EQ,
        M_LESS,
        M_GREATER,
        M_LESSEQ,
        M_GREATEREQ,
        M_AND,
        M_OR,
        M_SYSCALL,
        M_GOTO,
        M_LABEL,
        M_IF_ELSE,
        M_FUNCTION

    } type;

    union {

        char *varName;

        char *functionName;

        struct {

            Atom *from, *to;

        } var;

        struct {

            Atom *to, *part1, *part2;

        } operation;

        struct {

            Atom *a1, *a2;

            int operator;

            char *labelIfTrue;

        } ifS;

        char *gotoName;

    } value;

};

typedef struct MIPSInstr_ MIPSInstr;

typedef struct Function_ MIPSFunction;

MIPSInstr *initMipsInstr();

MIPSFunction* translateThreeRegisters(LinkedList *commands);

#endif //RCOMPILER_MASTER_MIPS_H
