#ifndef _YANC_SCHEMA_H_
#define _YANC_SCHEMA_H_

#include <stdint.h>
#include <stdio.h>

enum schema_type {
	SCHEMA_U8,
	SCHEMA_U16,
	SCHEMA_U32,
	SCHEMA_U64,
	SCHEMA_I8,
	SCHEMA_I16,
	SCHEMA_I32,
	SCHEMA_I64,
	SCHEMA_STRING,
	SCHEMA_RAW,
	SCHEMA_LINK,
	SCHEMA_MAC,
	SCHEMA_IP32,
	SCHEMA_OBJECT,
	SCHEMA_SUBDIR,
	SCHEMA_COMPOUND,
};

typedef struct Schema Schema; struct Schema {

	char*            name;
	enum schema_type type;
	Schema*          parent;
	Schema*          children;
	Schema*          next;
};

Schema*  schema_parse(const char* str);
Schema*  schema_fparse(const char* filename);
char*    schema_pack(const Schema*, char** end);
Schema*  schema_unpack(const char* buffer);

Schema*  schema_search(Schema*, const char* path);
void     schema_printf(const Schema*, FILE*);
uint32_t schema_len(const Schema*);

void     schema_free(Schema*);

#endif/*_YANC_SCHEMA_H_*/
