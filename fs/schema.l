%{
#include <stdlib.h>
#include <stdio.h>
#include "schema.h"
#include "schema.tab.h"
%}

%option nounput noinput

%%

u8                  yylval.type = SCHEMA_U8;     return TYPE;
u16                 yylval.type = SCHEMA_U16;    return TYPE;
u32                 yylval.type = SCHEMA_U32;    return TYPE;
u64                 yylval.type = SCHEMA_U64;    return TYPE;
i8                  yylval.type = SCHEMA_I8;     return TYPE;
i16                 yylval.type = SCHEMA_I16;    return TYPE;
i32                 yylval.type = SCHEMA_I32;    return TYPE;
i64                 yylval.type = SCHEMA_I64;    return TYPE;
str                 yylval.type = SCHEMA_STRING; return TYPE;
raw                 yylval.type = SCHEMA_RAW;    return TYPE;
linke|ln            yylval.type = SCHEMA_LINK;   return TYPE;
eth|mac             yylval.type = SCHEMA_MAC;    return TYPE;
ip32                yylval.type = SCHEMA_IP32;   return TYPE;
compound            yylval.type = SCHEMA_COMPOUND; return TYPE;
\{                  return OPEN;
\}                  return CLOSE;
[-_a-zA-Z0-9.\*]+   yylval.str = strdup(yytext); return NAME;
\"                  return DQUOTE;
\'                  return SQUOTE;
\n                  /* ignore EOL */
[ \t]+              /* ignore whitespace */
\#.*$               /* ignore comments */
;                   return SEMI;

%%
