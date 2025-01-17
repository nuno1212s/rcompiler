
// AST definitions
#ifndef __ast_h__
#define __ast_h__

#include "linkedlist.h"

// AST for expressions
struct _Expr {
    enum {
        E_INTEGER,
        E_NAME, //as
        E_NAME_ADDR, // & x
        E_STRING, // "teste"
        E_OPERATION, //a + bteste
        E_BOOL, // a < b
        E_BOOL_VALUE, // true
        E_ASSIGNMENT, // b = a
        E_FUNC_CALL // ola()
    } kind;

    union {
        int value; // for integer values
        char *name; // for name value

        struct {
            int operator; // PLUS, MINUS, etc
            struct _Expr *left;
            struct _Expr *right;
        } op; // for binary expressions

        struct {
            char *name;

            struct _Expr *value;
        } assignment;

        struct {

            char *functionName;

            LinkedList *args;

        } funcCall;
    } attr;

};

struct _Command {

    enum {
        EMPTY_CMD = 0, //;
        EXPR_CMD, //ola();
        WHILE_CMD,
        IF_CMD,
        IF_ELSE_CMD,
        VAR_CMD, // var expr
        COMPOUND_CMD // { }
    } command;

    union {
        struct _Expr *value;

        struct {

            struct _Expr *expr;

            struct _Command *cmd;

        } ifCmd, whileCmd;

        struct {

            struct _Expr *expr;

            struct _Command *cmd;

            struct _Command *elsecmd;

        } ifElseCmd;

        struct {

            LinkedList *expr;

        } varDef;

        struct {

            LinkedList *commands;

        } compound;

    } attr;
};

struct _Function {

    char *name;

    LinkedList *args;

    struct _Command *command;

};

typedef struct _Expr Expr; // Convenience typedef

typedef struct _Command Command; //Convenience typedef

typedef struct _Function Function;

// Constructor functions (see implementation in ast.c)
Expr *ast_integer(int v);

Expr *ast_name(char *name);

Expr *ast_name_addr(char *name);

Expr *ast_string(char *string);

Expr *ast_bool(int val);

Expr *ast_operation(int operator, Expr *left, Expr *right);

Expr *ast_binary(int operator, Expr *left, Expr* right);

Expr *ast_assignment(char *name, Expr *value);

Expr *ast_parenthesis(Expr *value);

Expr *ast_funcCall(char *name, LinkedList *);

Command *ast_empty();

Command *ast_if(Expr *expr, Command* cmd);

Command *ast_while(Expr *expr, Command *cmd);

Command *ast_if_then_else(Expr *expr, Command* cmdIf, Command* cmdElse);

Command *ast_expr(Expr *expr);

Command *ast_var(LinkedList *expr);

Command *ast_compound(LinkedList *);

Function *ast_function(char *name, LinkedList *, Command *);

#endif
