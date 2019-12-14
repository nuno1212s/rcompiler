//
// Created by nuno on 14/12/19.
//

#include "mips.h"
#include "code.h"
#include <stdlib.h>
#include <stdio.h>

MIPSInstr *initMipsInstr() {

    MIPSInstr *instr = malloc(sizeof(MIPSInstr));

    return instr;
}

MIPSFunction *initMipsFunction(LinkedList *data, LinkedList *text) {

    MIPSFunction *func = malloc(sizeof(MIPSFunction));



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

                    insert(data, a1->varName, NULL);

                } else if (a1->type == A_TEMP) {

                    MIPSInstr *mipsInstr = initMipsInstr();

                    mipsInstr->type = LOAD_INTO_REGISTER;

                    if (instr->atrib.atom2->type == A_VAR) {
                        if (!containsKey(data, instr->atrib.atom2->varName)) {
                            fprinf(stderr, "Failed to translate ATRIB instr, var %s is not declared previously.\n", instr->atrib.atom2->varName);
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



                break;
            }

        }

        node = node->next;
    }

}