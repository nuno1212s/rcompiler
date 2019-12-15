#include "parser.h"
#include <stdio.h>
#include "printAbsTree.h"

char *oprStr = NULL;
int globalSpaces = 0;

const char *getCmdName(Command *cmd) {

    switch (cmd->command) {
        case EMPTY_CMD:
            return "EMPTY_CMD";
        case WHILE_CMD:
            return "WHILE_CMD";
        case IF_CMD:
            return "IF_CMD";
        case IF_ELSE_CMD:
            return "IF_ELSE_CMD";
        case VAR_CMD:
            return "VAR_CMD";
        case COMPOUND_CMD:
            return "COMPOUND_CMD";
        default:
            printf("%d", cmd->command);
            return "";
    }

}

void printSpaces(int spaces) {
    for (int i = 0; i < spaces; i++)
        printf(" ");
}

void printName(char *name) {
    printSpaces(globalSpaces);
    printf("%s\n", name);
}

void printArgs(Expr *expr) {
    printExpr(expr);
    printf(",");
}

char *getName(int operator) {

    if (oprStr == NULL) {
        oprStr = malloc(sizeof(char) * 3);
    }

    switch (operator) {
        case MINUS:
            oprStr[0] = '-';
            oprStr[1] = '\0';
            break;
        case PLUS:
            oprStr[0] = '+';
            oprStr[1] = '\0';
            break;
        case MULT:
            oprStr[0] = '*';
            oprStr[1] = '\0';
            break;
        case DIV:
            oprStr[0] = '/';
            oprStr[1] = '\0';
            break;
        case REMAIN:
            oprStr[0] = '%';
            oprStr[1] = '\0';
            break;
        case GREATER:
            oprStr[0] = '>';
            oprStr[1] = '\0';
            break;
        case LESS:
            oprStr[0] = '<';
            oprStr[1] = '\0';
            break;
        case EQUAL:
            oprStr[0] = '=';
            oprStr[1] = '=';
            oprStr[2] = '\0';
            break;
        case NOTEQUAL:
            oprStr[0] = '!';
            oprStr[1] = '=';
            oprStr[2] = '\0';
            break;
        case GREATEREQ:
            oprStr[0] = '>';
            oprStr[1] = '=';
            oprStr[2] = '\0';
            break;
        case LESSEQ:
            oprStr[0] = '<';
            oprStr[1] = '=';
            oprStr[2] = '\0';
            break;
        case BAND:
            oprStr[0] = '&';
            oprStr[1] = '&';
            oprStr[2] = '\0';
            break;
        case BOR:
            oprStr[0] = '|';
            oprStr[1] = '|';
            oprStr[2] = '\0';
            break;
        default:
            break;
    }

    return oprStr;
}

void printExpr(Expr *expr) {
    printExprS(expr, globalSpaces);
}

void printExprS(Expr *expr, int spaces) {

    switch (expr->kind) {

        case E_INTEGER:
            printSpaces(spaces);
            printf("%d\n", expr->attr.value);
            break;
        case E_NAME:
            printSpaces(spaces);
            printf("%s\n", expr->attr.name);
            break;
        case E_NAME_ADDR:
            printSpaces(spaces);
            printf("& %s\n", expr->attr.name);
            break;
        case E_STRING:
            printSpaces(spaces);
            printf("\"%s\"\n", expr->attr.name);
            break;
        case E_BOOL_VALUE:
            printSpaces(spaces);
            printf("%s\n", expr->attr.value ? "TRUE" : "FALSE");
            break;
        case E_OPERATION:
        case E_BOOL: {
            printSpaces(spaces);

            printf("%s\n", getName(expr->attr.op.operator));

            printExprS(expr->attr.op.left, spaces + 1);
            printExprS(expr->attr.op.right, spaces + 1);
            break;
        }
        case E_ASSIGNMENT:
            printSpaces(spaces);
            printf("%s\n", expr->attr.assignment.name);
            printSpaces(spaces);
            printf("=\n");
            printExprS(expr->attr.assignment.value, spaces + 1);
            break;
        case E_FUNC_CALL: {
            printSpaces(spaces);
            printf("%s(\n", expr->attr.funcCall.functionName);
            int previousSpaces = globalSpaces;
            globalSpaces = spaces + 1;
            iterateList(expr->attr.funcCall.args, (void (*)(void *)) printExpr);
            globalSpaces = previousSpaces;
            printSpaces(spaces);
            printf(")\n");
            break;
        }

    }
}

//void printExpr(Expr *expr) {
//
//    switch (expr->kind) {
//        case E_INTEGER:
//            printf("%d", expr->attr.value);
//            break;
//        case E_NAME:
//            printf("%s", expr->attr.name);
//            break;
//        case E_NAME_ADDR:
//            printf("& %s", expr->attr.name);
//            break;
//        case E_STRING:
//            printf("\"%s\"", expr->attr.name);
//            break;
//        case E_BOOL_VALUE:
//            printf("%s", (expr->attr.value ? "TRUE" : "FALSE"));
//            break;
//        case E_OPERATION: {
//            printExpr(expr->attr.op.left);
//
//            printf(" %s ", getName(expr->attr.op.operator));
//
//            printExpr(expr->attr.op.right);
//            break;
//        }
//        case E_BOOL: {
//            printExpr(expr->attr.op.left);
//
//            printf(" %s ", getName(expr->attr.op.operator));
//
//            printExpr(expr->attr.op.right);
//            break;
//        }
//        case E_ASSIGNMENT: {
//            printf("%s = ", expr->attr.assignment.name);
//
//            printExpr(expr->attr.assignment.value);
//            break;
//        }
//        case E_FUNC_CALL: {
//            printf("%s(", expr->attr.funcCall.functionName);
//            iterateList(expr->attr.funcCall.args, (void (*)(void *)) printArgs);
//            printf(")");
//            break;
//        }
//    }
//}

void printCmdS(Command *cmd, int spaces) {

    switch (cmd->command) {
        case EMPTY_CMD:
            printSpaces(spaces);
            printf("\n");
            break;
        case WHILE_CMD:
            printSpaces(spaces);
            printf("WHILE(\n");
            printExprS(cmd->attr.whileCmd.expr, spaces + 1);
            printSpaces(spaces);
            printf(")\n");
            printCmdS(cmd->attr.whileCmd.cmd, spaces);
            break;
        case IF_CMD:
            printSpaces(spaces);
            printf("IF\n");
            printExprS(cmd->attr.ifCmd.expr, spaces + 1);
            printCmdS(cmd->attr.ifCmd.cmd, spaces);
            break;
        case IF_ELSE_CMD:
            printSpaces(spaces);
            printf("IF\n");
            printExprS(cmd->attr.ifElseCmd.expr, spaces + 1);
            printCmdS(cmd->attr.ifElseCmd.cmd, spaces + 4);
            printSpaces(spaces);
            printf("ELSE\n");
            printCmdS(cmd->attr.ifElseCmd.elsecmd, spaces + 4);
            break;
        case VAR_CMD:
            printSpaces(spaces);

            printf("VAR \n");

            int previous = globalSpaces;

            globalSpaces = spaces + 1;

            iterateList(cmd->attr.varDef.expr, (void (*)(void *)) printExpr);

            globalSpaces = previous;

            break;
        case COMPOUND_CMD: {
            int previousSpaces = globalSpaces;
            globalSpaces = spaces;

            iterateList(cmd->attr.compound.commands, (void (*)(void *)) printCmd);
            globalSpaces = previousSpaces;
            break;
        }
        case EXPR_CMD:
            printExprS(cmd->attr.value, spaces);
            break;
    }

}

void printCmd(Command *cmd) {
    printCmdS(cmd, globalSpaces);
}
//
//void printCmd(Command *cmd) {
//
//    switch (cmd->command) {
//        case EMPTY_CMD:
//            printf(";\n");
//            break;
//        case WHILE_CMD:
//            printf("while (");
//            printExpr(cmd->attr.whileCmd.expr);
//            printf(")");
//            printCmd(cmd->attr.whileCmd.cmd);
//            break;
//        case IF_CMD:
//            printf("if (");
//            printExpr(cmd->attr.ifCmd.expr);
//            printf(")");
//            printCmd(cmd->attr.ifCmd.cmd);
//            break;
//        case VAR_CMD:
//            printf("var %s = ", cmd->attr.varDef.varName);
//            printExpr(cmd->attr.varDef.expr);
//            printf(";\n");
//            break;
//        case COMPOUND_CMD:
//            printf("{\n");
//            iterateList(cmd->attr.compound.commands, (void (*)(void *)) printCmd);
//            printf("}\n");
//            break;
//        case EXPR_CMD:
//            printExpr(cmd->attr.value);
//            printf(";\n");
//            break;
//        default:
////            printf("%p", cmd);
//            printf("Failed %s", getCmdName(cmd));
//            break;
//    }
//
//}

void printFuncS(Function *f, int spaces) {
    printSpaces(spaces);

    printf("FN %s\n", f->name);

    int prevSpaces = globalSpaces;

    globalSpaces = spaces + 1;

    iterateList(f->args, (void (*)(void *)) printName);

    globalSpaces = prevSpaces;

    printCmdS(f->command, spaces + 4);
}

void printFunc(Function *f) {
    printFuncS(f, globalSpaces);
}


void printAtom(Atom *atom) {
    switch (atom->type) {
        case A_VAR:
            printf("%s", atom->varName);
            break;
        case A_NUMBER:
            printf("%d", atom->value);
            break;
        case A_TEMP:
            printf("$t%d", atom->value);
            break;
        case A_ARG:
            printf("$a%d", atom->value);
            break;
        case A_VAR_ADDR:
            printf("$&%s", atom->varName);
            break;
        case A_STRING:
            printf("\"%s\"", atom->stringValue);
            break;
        case A_FUNCTION_TYPE:
            printf("$v%d", atom->value);
            break;
    }
}

void printInstr(Instr *instr) {
    switch (instr->type) {
        case I_ATOM: {
            printf("$t%d := ", instr->finalValue);
            printAtom(instr->atom);
            printf("\n");
            break;
        }
        case I_BINOM: {
            printf("$t%d := ", instr->finalValue);
            printAtom(instr->binom.atom1);

            printf("%s", getName(instr->binom.operator));

            printAtom(instr->binom.atom2);

            printf("\n");
            break;
        }
        case I_GOTO: {
            printf("GOTO %s\n", instr->labelName);
            break;
        }

        case I_LAB: {
            printf("LABEL %s\n", instr->labelName);
            break;
        }

        case I_IF_ELSE: {
            printf("IF ");
            printAtom(instr->if_else.atom1);
            printf(" %s ", getName(instr->if_else.operator));
            printAtom(instr->if_else.atom2);
            printf(" THEN %s ELSE %s\n", instr->if_else.labelIfTrue, instr->if_else.labelIfFalse);
            break;
        }
        case I_ATRIB: {
            printAtom(instr->atrib.atom1);

            printf(" := ");

            printAtom(instr->atrib.atom2);

            printf("\n");

            break;
        }
        case I_READ: {
            printf("READ ");

            printAtom(instr->atom);

            printf("\n");

            break;
        }
        case I_PRINT: {
            printf("PRINT ");

            printAtom(instr->print.toPrint);

            printf("\n");

            break;
        }
        case I_FUNCTION: {

            printf("FUNC %s\n", instr->labelName);

            break;
        }
    }

}

void printInstrs(LinkedList *list) {

    iterateList(list, (void (*)(void *)) printInstr);

}

HTable *printing;

void printKey(char *);

void printMIPSInstr(MIPSInstr *);

void printMIPS(MIPSFunction *function) {

    printf(".data\n");

    printing = function->data;

    iterateKeys(function->data, printKey);

    printing = NULL;

    printf("\n\n.text\n");

    Node *node = function->text->first;

    while (node != NULL) {

        printMIPSInstr((MIPSInstr*)node->value);

        node = node->next;
    }

}

void printKey(char *key) {

    if (get(printing, key) != NULL) {

        printf("%s: .asciiz \"%s\"\n", key, (char*) get(printing, key));

    } else {
        printf("%s: .space 4\n", key);
    }

}

void printMIPSInstr(MIPSInstr *instr) {


    switch (instr->type) {

        case M_FUNCTION: {
            printf("%s:\n", instr->value.functionName);
            break;
        }
        case M_LOAD_INTO_VAR_CONST: {

            printf("sw ");

            printAtom(instr->value.var.to);

            printf(", ");

            printAtom(instr->value.var.from);

            printf("\n");
            break;
        }

        case M_LOAD_INTO_VAR_OTHER: {

            printf("sw ");

            printAtom(instr->value.var.to);

            printf(", ");

            printAtom(instr->value.var.from);

            printf("\n");

            break;
        }

        case M_LOAD_INTO_REG_CONST: {

            printf("li ");

            printAtom(instr->value.var.to);

            printf(", ");

            printAtom(instr->value.var.from);

            printf("\n");

            break;
        }

        case M_LOAD_INTO_REG_VAR: {

            printf("lw ");

            printAtom(instr->value.var.to);

            printf(", ");

            printAtom(instr->value.var.from);

            printf("\n");

            break;
        }

        case M_LOAD_ADRESS_INTO_REG: {

            printf("la ");

            printAtom(instr->value.var.to);

            printf(", ");

            printAtom(instr->value.var.from);

            printf("\n");

            break;
        }

        case M_MOVE: {

            printf("move ");

            printAtom(instr->value.var.to);

            printf(", ");

            printAtom(instr->value.var.from);

            printf("\n");

            break;
        }

        case M_ADD: {

            printf("add ");

            printAtom(instr->value.operation.to);

            printf(", ");

            printAtom(instr->value.operation.part1);

            printf(", ");

            printAtom(instr->value.operation.part2);

            printf("\n");

            break;
        }
        case M_SUB: {
            printf("sub ");

            printAtom(instr->value.operation.to);

            printf(", ");

            printAtom(instr->value.operation.part1);

            printf(", ");

            printAtom(instr->value.operation.part2);

            printf("\n");
            break;
        }

        case M_DIV: {
            printf("div ");

            printAtom(instr->value.operation.to);

            printf(", ");

            printAtom(instr->value.operation.part1);

            printf(", ");

            printAtom(instr->value.operation.part2);

            printf("\n");
            break;
        }

        case M_MULT: {
            printf("div ");

            printAtom(instr->value.operation.to);

            printf(", ");

            printAtom(instr->value.operation.part1);

            printf(", ");

            printAtom(instr->value.operation.part2);

            printf("\n");
            break;
        }
        case M_REMAINDER: {
            printf("rem ");

            printAtom(instr->value.operation.to);

            printf(", ");

            printAtom(instr->value.operation.part1);

            printf(", ");

            printAtom(instr->value.operation.part2);

            printf("\n");
            break;
        }

        case M_AND: {
            printf("and ");

            printAtom(instr->value.operation.to);

            printf(", ");

            printAtom(instr->value.operation.part1);

            printf(", ");

            printAtom(instr->value.operation.part2);

            printf("\n");
            break;
        }

        case M_OR: {
            printf("or ");

            printAtom(instr->value.operation.to);

            printf(", ");

            printAtom(instr->value.operation.part1);

            printf(", ");

            printAtom(instr->value.operation.part2);

            printf("\n");
            break;
        }

        case M_IF_ELSE: {

            switch (instr->value.ifS.operator) {

                case EQUAL:
                    printf("beq ");
                    break;
                case NOTEQUAL:
                    printf("bne ");
                    break;
                case GREATER:
                    printf("bgt ");
                    break;
                case GREATEREQ:
                    printf("bge ");
                    break;
                case LESS:
                    printf("blt ");
                    break;
                case LESSEQ:
                    printf("ble ");
                    break;

            }

            printAtom(instr->value.ifS.a1);

            printf(", ");

            printAtom(instr->value.ifS.a2);

            printf(", %s\n", instr->value.ifS.labelIfTrue);

            break;
        }

        case M_LABEL: {

            printf("LABEL %s\n", instr->value.gotoName);

            break;
        }

        case M_GOTO: {
            printf("GOTO %s\n", instr->value.gotoName);
            break;
        }

        case M_SYSCALL: {
            printf("syscall\n");
            break;
        }

    }

}