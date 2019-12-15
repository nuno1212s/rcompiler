#include <stdio.h>
#include "parser.h"
#include "printAbsTree.h"
#include "code.h"
#include "mips.h"

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
        printFunc(root);
    }

    LinkedList *functionList = compileFunction(root);

    printInstrs(functionList);

    printf("\n\n\nMIPS::::\n\n\n");

    MIPSFunction *func = translateThreeRegisters(functionList);

    printMIPS(func);

//
//
//    char *name = "x";
//    Expr *e = ast_binary(GREATER, ast_name(name), ast_operation(MULT, ast_integer(2), ast_integer(5)));
//
//    LinkedList *args = mkEmptyList();
//
//    concatLast(args, e);
//    concatLast(args, e);
//
//    Expr *e2 = ast_funcCall("test", args);
//
//    LinkedList *atribs = mkEmptyList();
//
//    Expr *assignment = ast_assignment("x", ast_binary(PLUS, ast_integer(10), ast_integer(5)));
//
//    concatLast(atribs, assignment);
//
//    Command *cmd = ast_var(atribs),
//            *cmd2 = ast_while(e, cmd);
//
//    printCmd(cmd2);
//
//    int result = 0;
//
//    LinkedList *l = compileCmd(cmd2);
//
//    printf("Printing the Instrs\n");
//    printf("%d\n", l->size);
//
//    printInstrs(l);

    return 0;

}
