#include "abstree.h"
#include "parser.h"
#include <stdio.h>

char *oprStr = NULL;

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
        case E_OPERATION: {
            printExpr(expr->attr.op.left);

            printf(" %s ", getName(expr->attr.op.operator));

            printExpr(expr->attr.op.right);
            break;
        }
    }
}