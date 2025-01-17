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

Expr* ast_name_addr(char *name) {
    Expr *node = (Expr*) malloc(sizeof(Expr));

    node->kind = E_NAME_ADDR;
    node->attr.name = name;

    return node;
}

Expr *ast_string(char *string) {
    Expr *node = (Expr *) malloc(sizeof(Expr));

    node->kind = E_STRING;

    int strLen = strlen(string);

    char *finalName = malloc(sizeof(char) * (strLen - 2));
    //Remove the " from the string

    memcpy(finalName, (string + 1), strLen - 2);

    node->attr.name = finalName;

    free(string);

    return node;
}

Expr *ast_bool(int value) {
    Expr * expr = malloc(sizeof(Expr));

    expr->kind = E_BOOL_VALUE;

    expr->attr.value = value;

    return expr;
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

Expr *ast_binary(int operator, Expr *left, Expr *right) {

    Expr *node = (Expr *) malloc(sizeof(Expr));

    node->kind = E_BOOL;
    node->attr.op.operator = operator;
    node->attr.op.left = left;
    node->attr.op.right = right;

    return node;
}

Expr *ast_assignment(char *name, Expr *value) {
    Expr *node = (Expr *) malloc(sizeof(Expr));

    node->kind = E_ASSIGNMENT;
    node->attr.assignment.name = name;
    node->attr.assignment.value = value;

    return node;
}

Expr *ast_funcCall(char *name, LinkedList *list) {

    Expr *nCmd = malloc(sizeof(Expr));

    nCmd->kind = E_FUNC_CALL;

    nCmd->attr.funcCall.functionName = name;
    nCmd->attr.funcCall.args = list;

    return nCmd;
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

Command *ast_while(Expr *expr, Command *cmd) {
    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = WHILE_CMD;

    nCmd->attr.whileCmd.expr = expr;
    nCmd->attr.whileCmd.cmd = cmd;

    return nCmd;
}

Command *ast_if_then_else(Expr *expr, Command *ifThen, Command *elseThen) {

    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = IF_ELSE_CMD;

    nCmd->attr.ifElseCmd.expr = expr;
    nCmd->attr.ifElseCmd.cmd = ifThen;
    nCmd->attr.ifElseCmd.elsecmd = elseThen;

    return nCmd;
}

Command *ast_expr(Expr *expr) {
    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = EXPR_CMD;

    nCmd->attr.value = expr;

    return nCmd;
}

Command *ast_var(LinkedList *expr) {

    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = VAR_CMD;

    nCmd->attr.varDef.expr = expr;

    return nCmd;
}

Command *ast_compound(LinkedList *list) {
    Command *nCmd = malloc(sizeof(Command));

    nCmd->command = COMPOUND_CMD;

    nCmd->attr.compound.commands = list;

    return nCmd;
}


Function *ast_function(char *name, LinkedList *args, Command *command) {

    Function *func = malloc(sizeof(Function));

    func->name = name;
    func->args = args;
    func->command = command;

    return func;
}
