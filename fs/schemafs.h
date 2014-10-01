#ifndef _SCHEMA_FS_
#define _SCHEMA_FS_

#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "schema.h"

typedef struct SchemaEnt SchemaEnt;
struct SchemaEnt {

	char*         name;
	const Schema* schema;
	SchemaEnt*    parent;
	SchemaEnt*    next;
	SchemaEnt*    children;

	uid_t         uid;
	gid_t         gid;
	mode_t        mode;
	off_t         size;
	struct timespec atim;
	struct timespec mtim;
	struct timespec ctim;

	int           hidden;

	union {
		void*      data;
		uint8_t    u8;
		uint16_t   u16;
		uint32_t   u32;
		uint64_t   u64;
		int8_t     i8;
		int16_t    i16;
		int32_t    i32;
		int64_t    i64;
		char*      str;
		char*      link;
	};

	int (*get)(const SchemaEnt*, char* buf, size_t, off_t);
	int (*set)(SchemaEnt*, const char* buf, size_t, off_t);
};

SchemaEnt* schemafs_search(SchemaEnt* root, const char* path);
void       schemafs_set_type(SchemaEnt*);
void       schemafs_free(SchemaEnt* schema);
int        schemafs_run(int argc, char* argv[], Schema* schema,
		const char* source, const char* mountpoint,
		const char* options, bool multithreaded);

void       schemafs_compound_handler(const char*, size_t);

#endif/*_SCHEMA_FS_*/
