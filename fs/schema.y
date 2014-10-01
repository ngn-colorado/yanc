%{
#include <string.h>
#include "schema.h"

//#define YYSTYPE char*

extern int yylex(void);

void yyerror (char const* str)
{
	fprintf(stderr, "%s\n", str);
}

Schema* parsed_schema = NULL;

%}

%union {
	char*            str;
	enum schema_type type;
	Schema*          schema;
}

%token          SQUOTE DQUOTE OPEN CLOSE SEMI LINK
%token <str>    NAME
%token <type>   TYPE
%type  <str>    subdir_name
%type  <schema> file object subdir
%type  <schema> entry entries

%%

schema:
	entries {
		Schema* s   = malloc(sizeof(*s));
		s->name     = strdup("/");
		s->type     = SCHEMA_SUBDIR;
		s->children = $1;
		s->next     = NULL;

		parsed_schema = s;

		s = s->children;
		while (s) {
			s->parent = parsed_schema;
			s = s->next;
		}
	}
	;

entries:
	/* empty */ {
		$$ = NULL;
	}
	| entry entries {
		$$       = $1;
		$$->next = $2;
	}
	;

entry:
	file
	| object
	| subdir
	;

file:
	TYPE NAME SEMI {
		$$ = malloc(sizeof(*$$));
		$$->name     = $2;
		$$->type     = $1;
		$$->parent   = NULL;
		$$->children = NULL;
		$$->next     = NULL;
	}
	;

object:
	NAME OPEN entries CLOSE {
		$$ = malloc(sizeof(*$$));
		$$->name     = $1;
		$$->type     = SCHEMA_OBJECT;
		$$->parent   = NULL;
		$$->children = $3;
		$$->next     = NULL;

		Schema* s = $$->children;
		while (s) {
			s->parent = $$;
			s = s->next;
		}
	}
	;

subdir:
	subdir_name OPEN entries CLOSE {
		$$ = malloc(sizeof(*$$));
		$$->name     = $1;
		$$->type     = SCHEMA_SUBDIR;
		$$->parent   = NULL;
		$$->children = $3;
		$$->next     = NULL;

		Schema* s = $$->children;
		while (s) {
			s->parent = $$;
			s = s->next;
		}
	}
	;

subdir_name:
	SQUOTE NAME SQUOTE { $$ = $2; }
	| DQUOTE NAME DQUOTE { $$ = $2; }
	;

%%

extern void yy_scan_string(const char*);
extern void yylex_destroy(void);
extern FILE* yyin;

Schema* schema_parse(const char* str)
{
	if (str)
		yy_scan_string(str);
	yyparse();
	yylex_destroy();
	return parsed_schema;
}

Schema* schema_fparse(const char* filename)
{
	if (filename) {
		if ((yyin = fopen(filename, "r")) == NULL) {
			fprintf(stderr, "schema_fparse(): ERROR: %m\n");
			return NULL;
		}
	}
	yyparse();
	if (filename)
		fclose(yyin);
	yylex_destroy();
	return parsed_schema;
}
