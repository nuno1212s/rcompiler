//
// Created by nuno on 14/12/19.
//

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

                } else if (a1->type == A_TEMP) {

                    //I don't think this is necessary, because temps are noted as a I_ATOM
                    MIPSInstr *mipsInstr = initMipsInstr();

                    mipsInstr->type = LOAD_INTO_REGISTER;

                    if (instr->atrib.atom2->type == A_VAR) {
                        if (!containsKey(data, instr->atrib.atom2->varName)) {
                            fprinf(stderr, "Failed to translate ATRIB instr, var %s is not declared previously.\n",
                                   instr->atrib.atom2->varName);
                            return NULL;
                        }
                    }

                    mipsInstr->value.var.from = instr->atrib.atom2;

                    mipsInstr->value.var.to = instr->atrib.atom1;

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
                } else if (instr->atom->type == A_VAR || instr->atom->type == A_VAR_ADRR) {
                    mipsInstr->type = M_LOAD_INTO_REG_VAR;
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

        }

        node = node->next;
    }

    return initMipsFunction(data, text);
}