// Tokens
%token
  INT
  NAME
  MINUS
  PLUS
  MULT
  DIV
  REMAIN
  GREATER
  LESS
  EQUAL
  NOTEQUAL
  GREATEREQ
  LESSEQ
  ASSIGNMENT
  SMCL
  OPENPARENTHESIS
  CLOSEPARENTHESIS
  OPENBRACKETS
  CLOSEBRACKETS
  VAR
  IF
  WHILE
  COMMA
  FN

// Operator associativity & precedence
%left PLUS
%left MINUS
%left MULT
%left DIV
%left REMAIN
%left GREATER
%left LESS
%left EQUAL
%left NOTEQUAL
%left GREATEREQ
%left LESSEQ
%left ASSIGNMENT

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  char *nameValue;
  Expr* exprValue;
  Command* cmdValue;
  LinkedList* cmdList, *nameList;
  Function* funcValue;
}

%type <intValue> INT
%type <nameValue> NAME
%type <exprValue> expr
%type <cmdValue> cmd
%type <cmdList> cmd_list
%type <nameList> name_list
%type <funcValue> func

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "abstree.h"
#include "linkedlist.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);

Function* root;
}

%%
program: func { root = $1; }

func:
  FN NAME OPENPARENTHESIS name_list CLOSEPARENTHESIS cmd {
    $$ = ast_function($2, $4, $6);
  }
  ;
name_list:
  NAME COMMA name_list {
    $$ = concatStart($1, $3);
  }
  |
  NAME {
    $$ = mkList($1);
  }
  |
  {
    $$ = mkEmptyList();
  }
  ;

cmd_list:
  cmd cmd_list {
    //printf("Command with list\n");
    $$ = concatStart($1, $2);
  }
  |
  cmd {
    //printf("Command\n");
    $$ = mkList($1);
  }
  ;

cmd:
  OPENBRACKETS cmd_list CLOSEBRACKETS {
    //printf("identify list\n");
    $$ = ast_compound($2);
  }
  |
  IF expr cmd {
    //printf("if statement identified\n");
    $$ = ast_if($2, $3);
  }
  |
  WHILE expr cmd {
    //printf("while statement identified\n");
    $$ = ast_while($2, $3);
  }
  |
  VAR NAME ASSIGNMENT expr SMCL {
    //printf("var statement identified\n");
    $$ = ast_var($2, $4);
  }
  |
  expr SMCL {
    $$ = ast_expr($1);
  }
  ;

expr:
  INT {
    $$ = ast_integer($1);
  }
  |
  NAME {
    $$ = ast_name($1);
  }
  |
  expr PLUS expr {
    $$ = ast_operation(PLUS, $1, $3);
  }
  |
  expr MULT expr {
    $$ = ast_operation(MULT, $1, $3);
  }
  |
  expr DIV expr {
    $$ = ast_operation(DIV, $1, $3);
  }
  |
  expr MINUS expr {
    $$ = ast_operation(MINUS, $1, $3);
  }
  |
  expr REMAIN expr {
    $$ = ast_operation(REMAIN, $1, $3);
  }
  |
  expr GREATER expr {
    $$ = ast_binary(GREATER, $1, $3);
  }
  |
  expr LESS expr {
    $$ = ast_binary(LESS, $1, $3);
  }
  |
  expr EQUAL expr {
    $$ = ast_binary(EQUAL, $1, $3);
  }
  |
  expr NOTEQUAL expr {
    $$ = ast_binary(NOTEQUAL, $1, $3);
  }
  |
  expr GREATEREQ expr {
    $$ = ast_binary(GREATEREQ, $1, $3);
  }
  |
  expr LESSEQ expr {
    $$ = ast_binary(LESSEQ, $1, $3);
  }
  |
  NAME ASSIGNMENT expr {
    $$ = ast_assignment($1, $3);
  }
  |
  OPENPARENTHESIS expr CLOSEPARENTHESIS {
    $$ = $2;
  }
  ;
%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}

