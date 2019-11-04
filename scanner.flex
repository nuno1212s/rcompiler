%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

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
";" { return SMCL; }

.  { yyerror("unexpected character"); }
%%

