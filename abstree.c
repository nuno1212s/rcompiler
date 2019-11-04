// AST constructor functions

#include <stdlib.h> // for malloc
#include "abstree.h" // AST header
#include <string.h>
#include <stdio.h>

Expr *ast_integer(int v) {
    Expr *node = (Expr *) malloc(sizeof(Expr));
    node->kind = E_INTEGER;
    node->attr.value = v;
    return node;
}

Expr *ast_name(char *name) {
    Expr *node = (Expr *) malloc(sizeof(Expr));

    node->kind = E_NAME;
    node->attr.name = name;

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

    cmd->command = EMPTY_CMD;

    return cmd;
}

Command *ast_if(Expr *expr, Command *cmd) {

    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = IF_CMD;

    nCmd->attr.ifCmd.expr = expr;
    nCmd->attr.ifCmd.cmd = cmd;

    return nCmd;
}

Command *ast_while (Expr *expr, Command *cmd) {
    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = WHILE_CMD;

    nCmd->attr.whileCmd.expr = expr;
    nCmd->attr.whileCmd.cmd = cmd;

    return nCmd;
}

Command *ast_if_then_else (Expr *expr, Command *ifThen, Command* elseThen) {

    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = IF_ELSE_CMD;

    nCmd->attr.ifElseCmd.expr = expr;
    nCmd->attr.ifElseCmd.cmd = ifThen;
    nCmd->attr.ifElseCmd.elsecmd = elseThen;

    return nCmd;
}

Command *ast_eq (char *name, Expr *expr) {
    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = ASSIGNMENT_CMD;

    nCmd->attr.assignment.varName = name;
    nCmd->attr.assignment.expr = expr;

    return nCmd;
}

Command *ast_var (char *name, Expr *expr) {

    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = VAR_CMD;

    nCmd->attr.varDef.varName = name;
    nCmd->attr.varDef.expr = expr;

    return nCmd;
}

Command *ast_compound(LinkedList *list) {
    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = COMPOUND_CMD;

    nCmd->attr.compound.commands = list;

    return nCmd;
}
