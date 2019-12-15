#include "mips.h"
#include "code.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

MIPSInstr *initMipsInstr() {

    MIPSInstr *instr = malloc(sizeof(MIPSInstr));

    return instr;
}

MIPSFunction *initMipsFunction(HTable *data, LinkedList *text) {

    MIPSFunction *func = malloc(sizeof(MIPSFunction));

    func->data = data;
    func->text = text;

    return func;
}

MIPSFunction *translateThreeRegisters(LinkedList *commands) {

    LinkedList *text = mkEmptyList();

    HTable *data = createHashTable();

    if (commands->size == 0) {
        return initMipsFunction(data, text);
    }

    Node *node = commands->first;

    while (node != NULL) {

        Instr *instr = (Instr *) node->value;

        switch (instr->type) {

            case I_ATRIB: {

                Atom *a1 = instr->atrib.atom1, *a2 = instr->atrib.atom2;

                if (a1->type == A_VAR) {

                    if (a2->type == A_STRING) {
                        insert(data, a1->varName, a2->stringValue);
                    } else {
                        insert(data, a1->varName, NULL);
                    }

                    MIPSInstr *mipsInstr = initMipsInstr();

                    if (a2->type == A_NUMBER) {
                        mipsInstr->type = M_LOAD_INTO_VAR_CONST;
                    } else {
                        mipsInstr->type = M_LOAD_INTO_VAR_OTHER;
                    }

                    mipsInstr->value.var.to = a1;
                    mipsInstr->value.var.from = a2;

                    concatLast(text, mipsInstr);

                } else if (a1->type == A_TEMP) {

                    //I don't think this is necessary, because temps are noted as a I_ATOM
                    MIPSInstr *mipsInstr = initMipsInstr();

                    mipsInstr->type = M_LOAD_INTO_REG_VAR;

                    if (instr->atrib.atom2->type == A_VAR) {
                        if (!containsKey(data, instr->atrib.atom2->varName)) {
                            fprintf(stderr, "Failed to translate ATRIB instr, var %s is not declared previously.\n",
                                    instr->atrib.atom2->varName);
                            return NULL;
                        }
                    }

                    mipsInstr->value.var.from = instr->atrib.atom2;

                    mipsInstr->value.var.to = instr->atrib.atom1;

                    concatLast(text, mipsInstr);
                } else if (a1->type == A_ARG) {

                    MIPSInstr *mipsInstr = initMipsInstr();

                    if (a2->type == A_VAR || a2->type == A_TEMP) {
                        mipsInstr->type = M_LOAD_INTO_REG_VAR;
                    } else if (a2->type == A_VAR_ADDR) {
                        mipsInstr->type = M_LOAD_ADRESS_INTO_REG;
                    } else {
                        mipsInstr->type = M_LOAD_INTO_REG_CONST;
                    }

                    mipsInstr->value.var.to = a1;
                    mipsInstr->value.var.from = a2;

                    concatLast(text, mipsInstr);
                } else {
                    fprintf(stderr, "Failed to translate ATRIB instruction.\n");

                    return NULL;
                }

                break;
            }

            case I_ATOM: {

                MIPSInstr *mipsInstr = initMipsInstr();

                mipsInstr->value.var.to = compileTemp(instr->finalValue);

                if (instr->atom->type == A_NUMBER) {
                    mipsInstr->type = M_LOAD_INTO_REG_CONST;
                } else if (instr->atom->type == A_VAR) {
                    mipsInstr->type = M_LOAD_INTO_REG_VAR;
                } else if (instr->atom->type == A_VAR_ADDR) {
                    mipsInstr->type = M_LOAD_ADRESS_INTO_REG;
                }

                //TODO: Deal with string values.
                mipsInstr->value.var.from = instr->atom;

                concatLast(text, mipsInstr);

                break;
            }

            case I_BINOM: {

                MIPSInstr *mipsInstr = initMipsInstr();

                switch (instr->binom.operator) {

                    case PLUS:
                        mipsInstr->type = M_ADD;
                        break;
                    case MINUS:
                        mipsInstr->type = M_SUB;
                        break;
                    case DIV:
                        mipsInstr->type = M_DIV;
                        break;
                    case MULT:
                        mipsInstr->type = M_MULT;
                        break;
                    case REMAIN:
                        mipsInstr->type = M_REMAINDER;
                        break;
                    case LESS:
                        mipsInstr->type = M_LESS;
                        break;
                    case GREATER:
                        mipsInstr->type = M_GREATER;
                        break;
                    case LESSEQ:
                        mipsInstr->type = M_LESSEQ;
                        break;
                    case GREATEREQ:
                        mipsInstr->type = M_GREATEREQ;
                        break;
                    case EQUAL:
                        mipsInstr->type = M_EQ;
                        break;
                    case NOTEQUAL:
                        mipsInstr->type = M_NOT_EQ;
                        break;
                    case BAND:
                        mipsInstr->type = M_AND;
                        break;
                    case BOR:
                        mipsInstr->type = M_OR;
                        break;

                }

                mipsInstr->value.operation.to = compileTemp(instr->finalValue);
                mipsInstr->value.operation.part1 = instr->binom.atom1;
                mipsInstr->value.operation.part2 = instr->binom.atom2;

                concatLast(text, mipsInstr);

                break;
            }
            case I_GOTO: {

                MIPSInstr *mipsInstr = initMipsInstr();

                mipsInstr->type = M_GOTO;

                mipsInstr->value.gotoName = instr->labelName;

                concatLast(text, mipsInstr);

                break;
            }

            case I_LAB: {

                MIPSInstr *mipsInstr = initMipsInstr();

                mipsInstr->type = M_LABEL;

                mipsInstr->value.gotoName = instr->labelName;

                concatLast(text, mipsInstr);

                break;
            }
            case I_IF_ELSE: {

                MIPSInstr *mipsInstr = initMipsInstr(), *elseInstr = initMipsInstr();

                mipsInstr->type = M_IF_ELSE;

                elseInstr->type = M_GOTO;

                elseInstr->value.gotoName = instr->if_else.labelIfFalse;

                mipsInstr->value.ifS.operator = instr->if_else.operator;

                mipsInstr->value.ifS.a1 = instr->if_else.atom1;

                mipsInstr->value.ifS.a2 = instr->if_else.atom2;

                mipsInstr->value.ifS.labelIfTrue = instr->if_else.labelIfTrue;

                concatLast(text, mipsInstr);

                concatLast(text, elseInstr);

                break;
            }
            case I_FUNCTION: {

                MIPSInstr *mipsInstr = initMipsInstr();

                mipsInstr->type = M_FUNCTION;

                mipsInstr->value.functionName = instr->labelName;

                concatLast(text, mipsInstr);

                break;
            }
            case I_READ: {

                MIPSInstr *setV = initMipsInstr(), *syscall = initMipsInstr(), *move = initMipsInstr();

                setV->type = M_LOAD_INTO_REG_CONST;

                syscall->type = M_SYSCALL;

                setV->value.var.to = compileFuncType(0);

                setV->value.var.from = compileInt(5);

                move->type = M_MOVE;

                move->value.var.to = instr->atom;

                move->value.var.from = compileArgAtom(0);

                concatLast(text, setV);

                concatLast(text, syscall);

                concatLast(text, move);

                break;
            }

            case I_PRINT: {

                MIPSInstr *setV = initMipsInstr(), *setA = initMipsInstr(), *syscall = initMipsInstr();

                setV->type = M_LOAD_INTO_REG_CONST;

                setA->type = M_LOAD_INTO_REG_VAR;

                setV->value.var.to = compileFuncType(0);

                if (instr->print.toPrint->type == A_STRING) {
                    setV->value.var.from = compileInt(4);
                } else {
                    setV->value.var.from = compileInt(1);
                }

                setA->value.var.to = compileArgAtom(0);

                setA->value.var.from = instr->print.toPrint;

                syscall->type = M_SYSCALL;

                concatLast(text, setV);

                concatLast(text, setA);

                concatLast(text, syscall);

                break;
            }

        }

        node = node->next;
    }

    return initMipsFunction(data, text);
}