// AST constructor functions

#include <stdlib.h> // for malloc
#include "abstree.h" // AST header

Expr *ast_integer(int v) {
    Expr *node = (Expr *) malloc(sizeof(Expr));
    node->kind = E_INTEGER;
    node->attr.value = v;
    return node;
}

Expr *ast_operation
        (int operator, Expr *left, Expr *right) {
    Expr *node = (Expr *) malloc(sizeof(Expr));
    node->kind = E_OPERATION;
    node->attr.op.operator = operator;
    node->attr.op.left = left;
    node->attr.op.right = right;
    return node;
}

Expr *ast_binary(int operator, Expr *left, Expr* right) {

    Expr *node = (Expr *) malloc(sizeof(Expr));

    node->kind = E_BOOL;
    node->attr.op.operator = operator;
    node->attr.op.left = left;
    node->attr.op.right = right;

    return node;
}

Command *ast_empty() {
    Command *cmd = malloc(sizeof(Command));

    cmd->command = EMPTY;

    return cmd;
}

Command *ast_binary(int cmd, Expr *expr, Command *cmd) {

    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = IF;

    nCmd->attr.ifCmd = {expr, cmd};

    return nCmd;
}

Command *ast_compound() {
    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = COMPOUND;

}
