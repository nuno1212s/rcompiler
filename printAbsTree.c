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
        case E_BOOL:{
            printSpaces(spaces);

            printf("%s\n", getName(expr->attr.op.operator));

            printExprS(expr->attr.op.left, spaces + 1);
            printExprS(expr->attr.op.right, spaces + 1);
            break;
        }
        case E_ASSIGNMENT:
            printSpaces(spaces);
            printf("=\n");
            printSpaces(spaces + 1);
            printf("%s\n", expr->attr.assignment.name);
            printExprS(expr->attr.assignment.value, spaces + 1);
            break;
        case E_FUNC_CALL: {
            printSpaces(spaces);
            printf("%s(\n", expr->attr.funcCall.functionName);
            int previousSpaces = globalSpaces;
            globalSpaces = spaces + 1;
            iterateList(expr->attr.funcCall.args, (void (*)(void *))  printExpr);
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
