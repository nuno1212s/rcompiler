%{
// HEADERS
#include <stdlib.h>
#include "parser.h"
#include <string.h>

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }

\-?[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}

"+" { return PLUS; }
"*" { return MULT; }
"-" { return MINUS; }
"/" { return DIV; }
"%" { return REMAIN; }
"<" { return LESS; }
">" { return GREATER; }
"==" { return EQUAL; }
"!=" { return NOTEQUAL; }
"<=" { return LESSEQ; }
">=" { return GREATEREQ; }
"(" { return OPENPARENTHESIS; }
")" { return CLOSEPARENTHESIS; }
"{" { return OPENBRACKETS; }
"}" { return CLOSEBRACKETS; }
";" { return SMCL; }
"=" { return ASSIGNMENT; }
"," { return COMMA; }
"&&" { return BAND; }
"||" { return BOR; }
"&" { return AND; }
"|" { return OR; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
"var" { return VAR; }
"let" { return VAR; }
"fn" { return FN; }
"true" { return TRUE; }
"false" { return FALSE; }

\".*\" {
  yylval.strValue = strdup(yytext);

  return STR;
}

[a-zA-Z][0-9a-zA-Z\_\!]* {
    yylval.nameValue = strdup(yytext);
    return NAME;
}

.  { yyerror("unexpected character"); }
%%