#include <stdio.h>
#include "parser.h"
#include "printAbsTree.c"

int eval(Expr *expr) {
    int result = 0;
    if (expr == 0) {
        yyerror("Null expression!!");
    } else if (expr->kind == E_INTEGER) {
        result = expr->attr.value;
    } else if (expr->kind == E_OPERATION) {
        int vLeft = eval(expr->attr.op.left);
        int vRight = eval(expr->attr.op.right);

        switch (expr->attr.op.operator) {
            case PLUS:
                result = vLeft + vRight;
                break;
            case MULT:
                result = vLeft * vRight;
                break;
            case DIV:
                result = vLeft / vRight;
                break;
            case MINUS:
                result = vLeft - vRight;
                break;
            case REMAIN:
                result = vLeft % vRight;
                break;
            case GREATER:
                result = vLeft > vRight;
                break;
            case LESS:
                result = vLeft < vRight;
                break;
            case EQUAL:
                result = vLeft == vRight;
                break;
            case NOTEQUAL:
                result = vLeft != vRight;
                break;
            case GREATEREQ:
                result = vLeft >= vRight;
                break;
            case LESSEQ:
                result = vLeft <= vRight;
                break;
            default:
                yyerror("Unknown operator!");
        }

    }
    return result;
}

int main(int argc, char **argv) {
    --argc;
    ++argv;
    if (argc != 0) {
        yyin = fopen(*argv, "r");
        if (!yyin) {
            printf("'%s': could not open file\n", *argv);
            return 1;
        }
    } //  yyin = stdin

    if (yyparse() == 0) {
        printExpr(root);
    }
    return 0;


}
