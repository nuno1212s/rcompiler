
// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _Expr {
    enum {
        E_INTEGER,
        E_OPERATION,
        E_BOOL
    } kind;

    union {
        int value; // for integer values
        struct {
            int operator; // PLUS, MINUS, etc
            struct _Expr *left;
            struct _Expr *right;
        } op; // for binary expressions
    } attr;

};

struct _Function {

    char *name;

    char **args;

    int argc;

    Command *cmd;

};

struct _Command {

    enum {
        EMPTY,
        WHILE,
        IF,
        IFELSE,
        VAR,
        COMPOUND
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

        }, ifElseCmd;

        struct {
            char *varName;

            struct _Expr *expr;
        } varDef;

        struct {

            Command ** command;

            int size;

        } compound;

    } attr;
};

typedef struct _Expr Expr; // Convenience typedef

typedef struct _Command Command; //Convenience typedef

// Constructor functions (see implementation in ast.c)
Expr *ast_integer(int v);

Expr *ast_operation(int operator, Expr *left, Expr *right);

Expr *ast_binary(int operator, Expr *left, Expr* right)

Command *ast_empty();

Command *ast_binary(int cmd, Expr *expr, Command* cmd);

Command *ast_trie(int cmd, Expr *expr, Command* cmdIf, Command* cmdElse);

Command *ast_var(char *name, Expr *expr);

#endif
