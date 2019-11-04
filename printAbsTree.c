#include "parser.h"
#include <stdio.h>
#include "printAbsTree.h"

char *oprStr = NULL;

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

void print(char *string) {
    printf("%s, ", string);
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

    switch (expr->kind) {
        case E_INTEGER:
            printf("%d", expr->attr.value);
            break;
        case E_NAME:
            printf("%s", expr->attr.name);
            break;
        case E_OPERATION: {
            printExpr(expr->attr.op.left);

            printf(" %s ", getName(expr->attr.op.operator));

            printExpr(expr->attr.op.right);
            break;
        }
        case E_BOOL: {
            printExpr(expr->attr.op.left);

            printf(" %s ", getName(expr->attr.op.operator));

            printExpr(expr->attr.op.right);
            break;
        }
        case E_ASSIGNMENT: {
            printf("%s = ", expr->attr.assignment.name);

            printExpr(expr->attr.assignment.value);
            break;
        }
    }
}

void printCmd(Command *cmd) {

    switch (cmd->command) {
        case EMPTY_CMD:
            printf(";\n");
            break;
        case WHILE_CMD:
            printf("while (");
            printExpr(cmd->attr.whileCmd.expr);
            printf(")");
            printCmd(cmd->attr.whileCmd.cmd);
            break;
        case IF_CMD:
            printf("if (");
            printExpr(cmd->attr.ifCmd.expr);
            printf(")");
            printCmd(cmd->attr.ifCmd.cmd);
            break;
        case VAR_CMD:
            printf("var %s = ", cmd->attr.varDef.varName);
            printExpr(cmd->attr.varDef.expr);
            printf(";\n");
            break;
        case COMPOUND_CMD:
            printf("{\n");
            iterateList(cmd->attr.compound.commands, (void (*)(void *)) printCmd);
            printf("}\n");
            break;
        case EXPR_CMD:
            printExpr(cmd->attr.value);
            printf(";\n");
            break;
        default:
//            printf("%p", cmd);
            printf("Failed %s", getCmdName(cmd));
            break;
    }

}

void printFunc(Function *f) {

    printf("fn %s(", f->name);

    iterateList(f->args, (void (*)(void *)) print);

    printf(")");

    printCmd(f->command);
}
