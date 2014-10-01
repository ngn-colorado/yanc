#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "schema.h"

typedef struct {
	uint32_t type;
	uint32_t n_children;
	uint32_t n_next;
	char     name[];
} PackedSchema;

// PACK ////////////////////////////////////////////////////////////////////////

void _pack(const Schema* schema, char* buf, char** cur_, char** end_)
{
	char* cur = *cur_;
	char* end = *end_;

	while (schema) {

		PackedSchema* ps;
		size_t        ps_len;

		ps     = (void*) cur;
		ps_len = sizeof(PackedSchema) + strlen(schema->name) + 1;

		if (end - cur < ps_len) {
			buf = realloc(buf, (end - buf) + ps_len + BUFSIZ);
			end += ps_len + BUFSIZ;
			assert (buf);
		}

		ps->type       = schema->type;
		ps->n_children = schema_len(schema->children);
		ps->n_next     = schema_len(schema->next);
		strncpy(ps->name, schema->name, ps_len - sizeof(PackedSchema));

		cur += ps_len;

		if (schema->type == SCHEMA_OBJECT || schema->type == SCHEMA_SUBDIR)
			_pack(schema->children, buf, &cur, &end);

		schema = schema->next;
	}

	*cur_ = cur;
	*end_ = end;
}

char* schema_pack(const Schema* schema, char** r_end)
{
	char* buf = malloc(BUFSIZ);
	char* cur = buf;
	char* end = buf + BUFSIZ;
	_pack(schema, buf, &cur, &end);
	if (r_end)
		*r_end = end;
	return buf;
}

// UNPACK //////////////////////////////////////////////////////////////////////

Schema* _unpack(PackedSchema** packed, Schema* parent)
{
	uint32_t      n_children, n_next;
	char*         next;
	Schema*       schema;

	if ((schema = malloc(sizeof(Schema))) == NULL) {
		fprintf(stderr, "schema_unpack: ERROR out of memory\n");
		return NULL;
	}
	
	if ((schema->name = strdup((*packed)->name)) == NULL) {
		fprintf(stderr, "schema_unpack: ERROR out of memory\n");
		free(schema);
		return NULL;
	}

	schema->type   = (*packed)->type;
	schema->parent = parent;
	n_children     = (*packed)->n_children;
	n_next         = (*packed)->n_next;

	next = ((char*) (*packed + 1)) + strlen(schema->name) + 1;
	*packed = (void*) next;

	switch (schema->type) {
	case SCHEMA_OBJECT:
	case SCHEMA_SUBDIR:
		if (n_children == 0)
			schema->children = NULL;
		else
			schema->children = _unpack(packed, schema);
		break;
	default:
		assert (n_children == 0);
		schema->children = NULL;
	}

	if (n_next == 0)
		schema->next = NULL;
	else
		schema->next = _unpack(packed, parent);

	return schema;
}

Schema* schema_unpack(const char* buf)
{
	Schema* schema;
	PackedSchema* packed = (void*) buf;
	schema = _unpack(&packed, NULL);
	return schema;
}

// SEARCH //////////////////////////////////////////////////////////////////////
//
// This is identical to schemafs_search() so be sure to duplicate any changes!

Schema* schema_search(Schema* schema, const char* path_)
{
	char *path, *tok, *_;

	if (!schema)
		return NULL;
	if (!path_)
		return schema;

	path = strdup(path_);
	assert (path);
	tok  = strtok_r(path, "/", &_);

	if (!tok && path[0] != '/')
		goto fail;
	if (tok == path)
		while (schema->parent != NULL)
			schema = schema->parent;

	while (tok) {

		if (strcmp(tok, "..") == 0) {

			schema = (schema->parent) ? : schema;

		} else if (strcmp(tok, ".") != 0) {

			schema = schema->children;

			while (schema) {
				if (strcmp(schema->name, tok) == 0)
					break;
				schema = schema->next;
			}

			if (!schema)
				goto fail;
		}

		tok = strtok_r(NULL, "/", &_);
	}

	free(path);
	return schema;
fail:
	free(path);
	return NULL;
}

// PRINT ///////////////////////////////////////////////////////////////////////

static const char* _schema_typenames[] = {
	"u8", "u16", "u32", "u64",
	"str", "raw",
	"ln",
	"eth", "ip32",
	"object", "subdir",
};

static void _printf(const Schema* schema, FILE* file, int level)
{
	int i;

	while (schema) {

		for (i = 0; i < level; i++)
			putc('\t', file);

		fprintf(file, "%-6s %s\n", _schema_typenames[schema->type], schema->name);

		switch (schema->type) {
		case SCHEMA_OBJECT:
		case SCHEMA_SUBDIR:
			_printf(schema->children, file, level+1);
			break;
		default:
			break;

		}
		schema = schema->next;
	}
}

void schema_printf(const Schema* schema, FILE* file)
{
	_printf(schema, file, 0);
}

// LEN /////////////////////////////////////////////////////////////////////////

uint32_t schema_len(const Schema* s)
{
	uint32_t len = 0;
	while (s) {
		len++;
		s = s->next;
	}
	return len;
}

// FREE ////////////////////////////////////////////////////////////////////////

void schema_free(Schema* schema)
{
	if (!schema)
		return;

	schema_free(schema->children);
	schema_free(schema->next);

	free(schema->name);
	free(schema);
}
