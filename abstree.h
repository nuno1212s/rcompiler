
// AST definitions
#ifndef __ast_h__
#define __ast_h__

#include "linkedlist.h"

// AST for expressions
struct _Expr {
    enum {
        E_INTEGER,
        E_NAME,
        E_OPERATION,
        E_BOOL
    } kind;

    union {
        int value; // for integer values
        char *name; // for name values

        struct {
            int operator; // PLUS, MINUS, etc
            struct _Expr *left;
            struct _Expr *right;
        } op; // for binary expressions
    } attr;

};

struct _Command {

    enum {
        EMPTY_CMD = 0,
        WHILE_CMD,
        IF_CMD,
        IF_ELSE_CMD,
        VAR_CMD,
        COMPOUND_CMD,
        ASSIGNMENT_CMD
    } command;

    union {
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

            char *varName;

            struct _Expr *expr;

        } varDef, assignment;

        struct {

            LinkedList *commands;

        } compound;

    } attr;
};

typedef struct _Expr Expr; // Convenience typedef

typedef struct _Command Command; //Convenience typedef

// Constructor functions (see implementation in ast.c)
Expr *ast_integer(int v);

Expr *ast_name(char *name);

Expr *ast_operation(int operator, Expr *left, Expr *right);

Expr *ast_binary(int operator, Expr *left, Expr* right);

Command *ast_empty();

Command *ast_if(Expr *expr, Command* cmd);

Command *ast_while(Expr *expr, Command *cmd);

Command *ast_if_then_else(Expr *expr, Command* cmdIf, Command* cmdElse);

Command *ast_eq(char *name, Expr *expr);

Command *ast_var(char *name, Expr *expr);

Command *ast_compound(LinkedList *);

#endif
