#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "util/log.h"
#include "schemafs.h"

// TODO: these all need to be polished, the size value needs to be checked and
// new lines should be added to the output where appropriate

// get integers ////////////////////////////////////////////////////////////////

static int
_get_u8(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	int r;
	r = snprintf(buf, bufsiz, "%hhu\n", ent->u8);
	return (r >= 0) ? r : -errno;
}

static int
_get_u16(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	int r;
	r = snprintf(buf, bufsiz, "%hu\n", ent->u16);
	return (r >= 0) ? r : -errno;
}

static int
_get_u32(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	int r;
	r = snprintf(buf, bufsiz, "%u\n", ent->u32);
	return (r >= 0) ? r : -errno;
}

static int
_get_u64(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	int r;
	r = snprintf(buf, bufsiz, "%lu\n", ent->u64);
	return (r >= 0) ? r : -errno;
}

static int
_get_i8(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	int r;
	r = snprintf(buf, bufsiz, "%hhd\n", ent->i8);
	return (r >= 0) ? r : -errno;
}

static int
_get_i16(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	int r;
	r = snprintf(buf, bufsiz, "%hd\n", ent->i16);
	return (r >= 0) ? r : -errno;
}

static int
_get_i32(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	int r;
	r = snprintf(buf, bufsiz, "%d\n", ent->i32);
	return (r >= 0) ? r : -errno;
}

static int
_get_i64(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	int r;
	r = snprintf(buf, bufsiz, "%ld\n", ent->i64);
	return (r >= 0) ? r : -errno;
}

// set integers ////////////////////////////////////////////////////////////////

static int
_set_u8(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	uint8_t tmp;
	char    bufcpy[1024];

	if (bufsiz >= sizeof(bufcpy)-1)
		return -ENOSPC;

	memcpy(bufcpy, buf, bufsiz);
	bufcpy[bufsiz] = '\0';

	if (sscanf(bufcpy, "%hhu", &tmp) != 1)
		return -EINVAL;
	ent->u8   = tmp;
	ent->size = _get_u8(ent, bufcpy, sizeof(bufcpy), 0);
	return bufsiz;
}

static int
_set_u16(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	uint16_t tmp;
	char     bufcpy[1024];

	if (bufsiz >= sizeof(bufcpy)-1)
		return -ENOSPC;

	memcpy(bufcpy, buf, bufsiz);
	bufcpy[bufsiz] = '\0';

	if (sscanf(bufcpy, "%hu", &tmp) != 1)
		return -EINVAL;
	ent->u16 = tmp;
	ent->size = _get_u16(ent, bufcpy, sizeof(bufcpy), 0);
	return bufsiz;
}

static int
_set_u32(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	uint32_t tmp;
	char     bufcpy[1024];

	if (bufsiz >= sizeof(bufcpy)-1)
		return -ENOSPC;

	memcpy(bufcpy, buf, bufsiz);
	bufcpy[bufsiz] = '\0';

	if (sscanf(bufcpy, "%u", &tmp) != 1)
		return -EINVAL;
	ent->u32 = tmp;
	ent->size = _get_u32(ent, bufcpy, sizeof(bufcpy), 0);
	return bufsiz;
}

static int
_set_u64(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	uint64_t tmp;
	char     bufcpy[1024];

	if (bufsiz >= sizeof(bufcpy)-1)
		return -ENOSPC;

	memcpy(bufcpy, buf, bufsiz);
	bufcpy[bufsiz] = '\0';

	if (sscanf(bufcpy, "%lu", &tmp) != 1)
		return -EINVAL;
	ent->u64 = tmp;
	ent->size = _get_u64(ent, bufcpy, sizeof(bufcpy), 0);
	return bufsiz;
}

static int
_set_i8(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	int8_t  tmp;
	char    bufcpy[1024];

	if (bufsiz >= sizeof(bufcpy)-1)
		return -ENOSPC;

	memcpy(bufcpy, buf, bufsiz);
	bufcpy[bufsiz] = '\0';

	if (sscanf(bufcpy, "%hhd", &tmp) != 1)
		return -EINVAL;
	ent->i8 = tmp;
	ent->size = _get_i8(ent, bufcpy, sizeof(bufcpy), 0);
	return bufsiz;
}

static int
_set_i16(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	int16_t tmp;
	char    bufcpy[1024];

	if (bufsiz >= sizeof(bufcpy)-1)
		return -ENOSPC;

	memcpy(bufcpy, buf, bufsiz);
	bufcpy[bufsiz] = '\0';

	if (sscanf(bufcpy, "%hd", &tmp) != 1)
		return -EINVAL;
	ent->i16 = tmp;
	ent->size = _get_i16(ent, bufcpy, sizeof(bufcpy), 0);
	return bufsiz;
}

static int
_set_i32(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	int32_t tmp;
	char    bufcpy[1024];

	if (bufsiz >= sizeof(bufcpy)-1)
		return -ENOSPC;

	memcpy(bufcpy, buf, bufsiz);
	bufcpy[bufsiz] = '\0';

	if (sscanf(bufcpy, "%d", &tmp) != 1)
		return -EINVAL;
	ent->i32 = tmp;
	ent->size = _get_i32(ent, bufcpy, sizeof(bufcpy), 0);
	return bufsiz;
}

static int
_set_i64(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	int64_t tmp;
	char    bufcpy[1024];

	if (bufsiz >= sizeof(bufcpy)-1)
		return -ENOSPC;

	memcpy(bufcpy, buf, bufsiz);
	bufcpy[bufsiz] = '\0';

	if (sscanf(bufcpy, "%ld", &tmp) != 1)
		return -EINVAL;
	ent->i64 = tmp;
	ent->size = _get_i64(ent, bufcpy, sizeof(bufcpy), 0);
	return bufsiz;
}

// raw /////////////////////////////////////////////////////////////////////////

static int
_get_raw(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	size_t len;
	void*  data;

	if (ent->data == NULL)
		return 0;

	data = ent->data;
	len  = ent->size;
	if (o >= len) return -ERANGE;
	data += o;
	len  -= o;

	if (len > bufsiz)
		len = bufsiz;
	memcpy(buf, data, len);
	return len;
}

static int
_set_raw(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	size_t min_size;

	min_size = o + bufsiz;

	if (min_size > ent->size) {
		void* tmp = realloc(ent->data, min_size);
		if (!tmp)
			return -ENOSPC;
		ent->data  = tmp;
		ent->size = min_size;
	}

	memcpy(ent->data + o, buf, bufsiz);
	return bufsiz;
}

// string //////////////////////////////////////////////////////////////////////

static int
_get_str(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	return _get_raw(ent, buf, bufsiz, o);
}

static int
_set_str(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	const char* end = buf + bufsiz;

	while (buf < end && (*buf == ' ' || *buf == '\t' || *buf == '\n')) {
		buf++;
		bufsiz--;
	}

	while (end > buf && (*end == ' ' || *end == '\t' || *end == '\n')) {
		end--;
		bufsiz--;
	}

	return _set_raw(ent, buf, bufsiz, o);
}

// Ethernet addr ///////////////////////////////////////////////////////////////

static int
_get_mac(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	int r;
	r = snprintf(buf, bufsiz, "%02lx:%02lx:%02lx:%02lx:%02lx:%02lx\n",
		(ent->u64 >> 40) & 0xFF, (ent->u64 >> 32) & 0xFF, (ent->u64 >> 24) & 0xFF,
		(ent->u64 >> 16) & 0xFF, (ent->u64 >>  8) & 0xFF, (ent->u64 >>  0) & 0xFF
	);
	return (r >= 0) ? r : -errno;
}

static int
_set_mac(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	int r;
	uint8_t a, b, c, d, e, f;

	if (o != 0) {
		log_warn("set_mac() nonzero offset");
		return -EIO;
	}

	r = sscanf(buf, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
			&a, &b, &c, &d, &e, &f);

	if (r == 1) {
		// Fallback to single number
		uint64_t hwaddr = strtoull(buf, NULL, 0);
		if (errno) return -errno;
		ent->u64 = hwaddr;
		return bufsiz;
	}

	if (r != 6)
		return (r < 0) ? -errno : -EINVAL;

	ent->u64 = 0;
	ent->u64 |= a; ent->u64 <<= 8;
	ent->u64 |= b; ent->u64 <<= 8;
	ent->u64 |= c; ent->u64 <<= 8;
	ent->u64 |= d; ent->u64 <<= 8;
	ent->u64 |= e; ent->u64 <<= 8;
	ent->u64 |= f;
	
	ent->size = 2*6 + 5 + 1;

	return bufsiz;
}

// IP addr /////////////////////////////////////////////////////////////////////

static int
_get_ip32(const SchemaEnt* ent, char* buf, size_t bufsiz, off_t o)
{
	int      r;
	uint32_t addr = ent->u64 &   0xFFFFFFFF;
	uint8_t  mask = (ent->u64 & 0xFF00000000) >> 32;

	r = snprintf(buf, bufsiz, "%u.%u.%u.%u/%u\n",
		(addr >> 24) & 0xFF,
		(addr >> 16) & 0xFF,
		(addr >>  8) & 0xFF,
		(addr >>  0) & 0xFF,
		mask
	);

	return (r >= 0) ? r : -errno;
}

static int
_set_ip32(SchemaEnt* ent, const char* buf, size_t bufsiz, off_t o)
{
	int r;
	uint8_t a,b,c,d, m;

	if (o != 0) {
		log_warn("set_ip32() nonzero offset");
		return -EIO;
	}

	r = sscanf(buf, "%hhu.%hhu.%hhu.%hhu/%hhu", &a, &b, &c, &d, &m);
	if (r == 4) {
		m = 32;
		r++;
	}

	if (r != 5)
		return (r < 0) ? -errno : -EINVAL;

	ent->u64 = 0;
	ent->u64 |= m; ent->u64 <<= 8;
	ent->u64 |= a; ent->u64 <<= 8;
	ent->u64 |= b; ent->u64 <<= 8;
	ent->u64 |= c; ent->u64 <<= 8;
	ent->u64 |= d;

	ent->size = 4;
	ent->size += (a < 100) ? ((a < 10) ? 1 : 2) : 3;
	ent->size += (b < 100) ? ((b < 10) ? 1 : 2) : 3;
	ent->size += (c < 100) ? ((c < 10) ? 1 : 2) : 3;
	ent->size += (d < 100) ? ((d < 10) ? 1 : 2) : 3;
	ent->size += (m < 100) ? ((m < 10) ? 1 : 2) : 3;

	return bufsiz;
}

////////////////////////////////////////////////////////////////////////////////

struct _ops {
	int (*get) (const SchemaEnt*, char*, size_t, off_t);
	int (*set) (SchemaEnt*, const char*, size_t, off_t);
};

static const struct _ops _ops[] = {
	[SCHEMA_U8]      = { _get_u8,     _set_u8 },
	[SCHEMA_U16]     = { _get_u16,    _set_u16 },
	[SCHEMA_U32]     = { _get_u32,    _set_u32 },
	[SCHEMA_U64]     = { _get_u64,    _set_u64 },
	[SCHEMA_I8]      = { _get_i8,     _set_i8 },
	[SCHEMA_I16]     = { _get_i16,    _set_i16 },
	[SCHEMA_I32]     = { _get_i32,    _set_i32 },
	[SCHEMA_I64]     = { _get_i64,    _set_i64 },
	[SCHEMA_STRING]  = { _get_str,    _set_str },
	[SCHEMA_RAW]     = { _get_raw,    _set_raw },
	[SCHEMA_MAC]     = { _get_mac,    _set_mac },
	[SCHEMA_IP32]    = { _get_ip32,   _set_ip32 },
	[SCHEMA_COMPOUND]= { _get_raw,    _set_raw },
};

void schemafs_set_type(SchemaEnt* ent)
{
	ent->get = _ops[ent->schema->type].get;
	ent->set = _ops[ent->schema->type].set;
}
