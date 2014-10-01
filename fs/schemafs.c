#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <fnmatch.h>
#define FUSE_USE_VERSION 29
#include <fuse.h>
#include <unistd.h>
#include <time.h>
#include "schema.h"
#include "schemafs.h"
#include "util/util.h"
#include "util/log.h"

static SchemaEnt*
_fill(const Schema* schema, SchemaEnt* parent);

// This is identical to schema_search() so be sure to duplicate any changes!

SchemaEnt* schemafs_search(SchemaEnt* schema, const char* path_)
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

SchemaEnt* schemafs_fnmatch(SchemaEnt* schema, const char* path_, int create)
{
	char *path, *tok, *_;
	SchemaEnt* new;

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
				if (fnmatch(schema->name, tok, FNM_PATHNAME|FNM_PERIOD) == 0) {
					if (!create)
						goto fail;
					new = _fill(schema->schema, schema->parent);
					new->name = strdup(tok);
					new->hidden = 0;
					new->next = schema->parent->children;
					schema->parent->children = new;
					schema = new;
					break;
				}
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

void schemafs_free(SchemaEnt* ent)
{
	if (ent->next)
		schemafs_free(ent->next);

	switch (ent->schema->type) {
	case SCHEMA_OBJECT:
	case SCHEMA_SUBDIR:
		if (ent->children)
			schemafs_free(ent->children);
		break;
	case SCHEMA_RAW:
		free(ent->data);
		break;
	case SCHEMA_STRING:
		free(ent->str);
		break;
	case SCHEMA_LINK:
		free(ent->link);
		break;
	default:
		break;

	}

	if (ent->name != ent->schema->name)
		free(ent->name);
	free(ent);
}

static SchemaEnt*
_fill(const Schema* schema, SchemaEnt* parent)
{
	SchemaEnt* ent;
	struct fuse_context* ctx;
	struct timespec ts;

	if (schema == NULL)
		return NULL;

	if ((ent = malloc(sizeof(*ent))) == NULL) {
		log_err("Unable to fill schema: %m");
		return NULL;
	}

	ctx = fuse_get_context();
	clock_gettime(CLOCK_REALTIME, &ts);

	ent->name   = schema->name;
	ent->schema = schema;
	ent->parent = parent;
	ent->next   = _fill(schema->next, parent);
	ent->uid    = ctx->uid;
	ent->gid    = ctx->gid;
	ent->size   = 0;
	ent->atim   = ts;
	ent->mtim   = ts;
	ent->ctim   = ts;
	ent->get    = NULL;
	ent->set    = NULL;

	if (strchr(ent->name, '*'))
		ent->hidden = 1;
	else
		ent->hidden = 0;

	switch (schema->type) {
	case SCHEMA_OBJECT:
		ent->mode = S_IFDIR | S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
		ent->children = NULL;
		break;
	case SCHEMA_SUBDIR:
		ent->mode = S_IFDIR | S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
		ent->children = _fill(schema->children, ent);
		break;
	case SCHEMA_LINK:
		ent->mode = S_IFLNK | S_IRWXU | S_IRWXG | S_IRWXO;
		ent->link = NULL;
		ent->hidden = 1;
		break;
	default:
		ent->mode = S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
		ent->data = NULL;
		schemafs_set_type(ent);
		break;
	}

	return ent;
}

static void*
_init(struct fuse_conn_info* conn)
{
	Schema*    schema;
	SchemaEnt* ent;
	struct fuse_context* ctx;
	
	ctx    = fuse_get_context();
	schema = ctx->private_data;
	ent    = _fill(schema, NULL);

	return ent;
}

static int
__getattr(const SchemaEnt* ent, struct stat* st)
{
	st->st_uid   = ent->uid;
	st->st_gid   = ent->gid;
	st->st_mode  = ent->mode;
	st->st_size  = ent->size;
	st->st_atim  = ent->atim;
	st->st_mtim  = ent->mtim;
	st->st_ctim  = ent->ctim;

	switch (ent->mode & S_IFMT) {
	case S_IFDIR:
		st->st_nlink = 2;
		break;
	case S_IFREG:
	case S_IFLNK:
		st->st_nlink = 1;
		break;
	default:
		log_warn("unhandled filetype: %d", ent->mode&S_IFMT);
		st->st_nlink = 1;
	}

	return 0;
}

static int
_getattr(const char* path, struct stat* st)
{
	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_search(ent, path);
	if (!ent || ent->hidden) return -ENOENT;
	return __getattr(ent, st);
}

static int
_utimens(const char* path, const struct timespec tv[2])
{
	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_search(ent, path);
	if (!ent || ent->hidden) return -ENOENT;

	ent->atim = tv[0];
	ent->mtim = tv[0];

	return 0;
}

static int
_chmod(const char* path, mode_t mode)
{
	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_search(ent, path);
	if (!ent || ent->hidden) return -ENOENT;
	if ((mode & S_IFMT) != (ent->mode & S_IFMT)) return -ENOTSUP;
	ent->mode = mode;
	return 0;
}

static int
_chown(const char* path, uid_t uid, gid_t gid)
{
	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_search(ent, path);
	if (!ent || ent->hidden) return -ENOENT;
	ent->uid = uid;
	ent->gid = gid;
	return 0;
}

static int
_open(const char* path, struct fuse_file_info* fi)
{
	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_search(ent, path);
	if (!ent || ent->hidden) return -ENOENT;

	fi->fh = (uint64_t) ent;
	return 0;
}

static int
_release(const char* _, struct fuse_file_info* fi)
{
	SchemaEnt* ent = (SchemaEnt*) fi->fh;
	
	if (ent->schema->type == SCHEMA_COMPOUND) {
		schemafs_compound_handler(ent->data, ent->size);
	}

	return 0;
}

static int
_opendir(const char* path, struct fuse_file_info* fi)
{
	return _open(path, fi);
}

static int
_readdir(const char* _, void* buf, fuse_fill_dir_t filler, off_t o, struct fuse_file_info* fi)
{
	SchemaEnt* ent = (SchemaEnt*) fi->fh;
	struct stat st;

	__getattr(ent, &st);
	filler(buf, ".",  &st, 0);
	if (ent->parent)
		__getattr(ent->parent, &st);
	filler(buf, "..", &st, 0);

	ent = ent->children;
	while (ent) {
		if (!ent->hidden) {
			__getattr(ent, &st);
			filler(buf, ent->name, &st, 0);
		}
		ent = ent->next;
	}

	return 0;
}

static int
_mkdir(const char* path, mode_t mode)
{
	int r = 0;
	char *dirname, *basename;
	breakup_path(path, &dirname, &basename);

	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_search(ent, dirname);
	if (!ent || ent->hidden) {
		r = -ENOENT;
		goto out;
	} else if (ent->schema->type != SCHEMA_OBJECT) {
		// todo, we could recreate schemaents here
		r = -EPERM;
		goto out;
	}

	if (schemafs_search(ent, basename) != NULL) {
		r = -EEXIST;
		goto out;
	}

	// still thinking abou this. don't really need this
	// custom schema, but seems like it will break the least code

	Schema* obj   = malloc(sizeof(*obj));
	assert (obj);
	obj->name     = strdup(basename);
	assert (obj->name);
	obj->type     = SCHEMA_SUBDIR;
	obj->parent   = ent->schema->parent;
	obj->children = ent->schema->children;
	obj->next     = ent->schema->next;

	SchemaEnt* new = _fill(obj, ent);
	new->next = ent->children;
	ent->children = new;

out:
	free(dirname);
	free(basename);
	return r;
}

static int
_rmdir(const char* path)
{
	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_search(ent, path);
	if (!ent || ent->hidden) return -ENOENT;
	if (ent->schema->type != SCHEMA_SUBDIR) return -ENOTDIR;

	if (ent->parent->children == ent)
		ent->parent->children = ent->next;
	else {
		SchemaEnt* cur = ent->parent->children;
		while (cur) {
			if (cur->next == ent) {
				cur->next = ent->next;
				break;
			}
			cur = cur->next;
		}
		assert (cur != NULL); // a check that ent was actually removed
	}

	ent->next = NULL;
	schemafs_free(ent);

	// todo free Schema when it was created in _mkdir(), or
	// stop using that Schema;

	return 0;
}

static int
_create(const char* path, mode_t mode, struct fuse_file_info* fi)
{
	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_fnmatch(ent, path, 1);
	if (!ent) return -ENOENT;
	if (ent->schema->type == SCHEMA_LINK
	    || ent->schema->type == SCHEMA_SUBDIR
	    || ent->schema->type == SCHEMA_OBJECT)
		return -EPERM;
	ent->hidden = 0;
	ent->mode   = mode;
	fi->fh = (uint64_t) ent;
	return 0;
}

static int
_symlink(const char* target, const char* path)
{
	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_fnmatch(ent, path, 1);
	if (!ent) return -ENOENT;
	if (ent->schema->type != SCHEMA_LINK) return -EPERM;

	free(ent->link);
	ent->link = strdup(target);
	ent->hidden = 0;
	return 0;
}

static int
_unlink(const char* path)
{
	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_search(ent, path);
	if (!ent || ent->hidden) return -ENOENT;
	if (ent->schema->type != SCHEMA_LINK) return -EPERM;

	free(ent->link);
	ent->link = NULL;
	ent->hidden = 1;
	return 0;
}

static int
_readlink(const char* path, char* buf, size_t bufsiz)
{
	SchemaEnt* ent = fuse_get_context()->private_data;
	ent = schemafs_search(ent, path);
	if (!ent || ent->hidden) return -ENOENT;
	if (ent->schema->type != SCHEMA_LINK) return -EPERM;

	if (ent->link)
		strncpy(buf, ent->link, bufsiz);
	else
		return -EIO;
	return 0;
}

static int
_read(const char* _, char* buf, size_t bufsiz, off_t o, struct fuse_file_info* fi)
{
	SchemaEnt* ent = (SchemaEnt*) fi->fh;
	if (ent->get)
		return ent->get(ent, buf, bufsiz, o);
	else
		return -ENOTSUP;
}

static int
_write(const char* _, const char* buf, size_t bufsiz, off_t o, struct fuse_file_info* fi)
{
	SchemaEnt* ent = (SchemaEnt*) fi->fh;
	if (ent->set)
		return ent->set(ent, buf, bufsiz, o);
	else
		return -ENOTSUP;
}

static int
_truncate(const char* path, off_t o)
{
	// TODO
	return 0;
}

static int
_rename(const char* oldpath, const char* newpath)
{
	SchemaEnt* ent;
	SchemaEnt* par;
	char* dirname;
	char* basename;

	// Get new parent relative to root
	breakup_path(newpath, &dirname, &basename);
	par = fuse_get_context()->private_data;
	par = schemafs_search(par, dirname);
	if (!par || par->hidden) {
		free(dirname);
		free(basename);
		return -ENOTDIR;
	}

	free(dirname);

	// Check for new relative to new parent
	ent = schemafs_search(par, basename);
	if (ent) {
		free(basename);
		return -EEXIST;
	}

	// Get old relative to root
	ent = fuse_get_context()->private_data;
	ent = schemafs_search(ent, oldpath);
	if (!ent || ent->hidden) {
		free(dirname);
		free(basename);
		return -ENOENT;
	}

	// Allow moves without enforcing the schema. For now this is just laziness
	// but there may be nothing wrong with it

	if (ent->parent->children == ent)
		ent->parent->children = ent->next;
	else {
		SchemaEnt* cur = ent->parent->children;
		while (cur) {
			if (cur->next == ent) {
				cur->next = ent->next;
				break;
			}
			cur = cur->next;
		}
		assert (cur != NULL); // a check that ent was actually removed
	}

	if (ent->name != ent->schema->name)
		free(ent->name);
	ent->name = basename;

	ent->parent = par;
	ent->next = par->children;
	par->children = ent;

	return 0;
}

// Compound:
//  mkdir <filename>
//  rmdir <filename>
//  write <filename> <data>...

void _compound_line_handler(const char* _line)
{
	char* line;
	char* cmd;
	char* filename;
	char* data;
	SchemaEnt* ent;

	if (!(line = strdup(_line)))
		return;

	cmd = line;
	filename = strchr(line, ' ') + 1;
	filename[-1] = '\0';

	if (strcmp(cmd, "mkdir") == 0) {

		log_debug("compound: mkdir <%s>", filename);
		_mkdir(filename, 0755);

	} else if (strcmp(cmd, "rmdir") == 0) {

		log_debug("compound: rmdir <%s>", filename);
		_rmdir(filename);

	} else if (strcmp(cmd, "write") == 0) {

		data = strchr(filename, ' ') + 1;
		data[-1] = '\0';

		log_debug("compound: write <%s> <%s>", filename, data);

		ent = fuse_get_context()->private_data;
		ent = schemafs_search(ent, filename);
		if (!ent || ent->hidden)
			return;

		if (ent->set)
			ent->set(ent, data, strlen(data), 0);
	} else {
		log_warn("compound: WARNING <%s>", line);
	}

	free(line);
}

struct timespec ts_diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

void schemafs_compound_handler(const char* _buf, size_t bufsiz)
{
	char* buf;
	char* line;
	char* line_tok_state;
	int   num_ops = 0;
	struct timespec start;
	struct timespec end;
	struct timespec delta;

	// Assure buffer is a null-terminated string
	buf = malloc(bufsiz + 1);
	memcpy(buf, _buf, bufsiz);
	buf[bufsiz] = '\0';

	line = strtok_r(buf, "\n", &line_tok_state);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	while (line) {
		_compound_line_handler(line);
		line = strtok_r(NULL, "\n", &line_tok_state);
		num_ops++;
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
	delta = ts_diff(start, end);

	log_info("compound: %d ops in %lds %ldns ", num_ops, delta.tv_sec, delta.tv_nsec);

	free(buf);
}

static const struct fuse_operations _ops = {

	.init        = _init,
	.getattr     = _getattr,
	.utimens     = _utimens,
	.open        = _open,
	.release     = _release,
	.opendir     = _opendir,
	.readdir     = _readdir,
	.mkdir       = _mkdir,
	.rmdir       = _rmdir,
	.chown       = _chown,
	.chmod       = _chmod,
	.create      = _create,
	.symlink     = _symlink,
	.unlink      = _unlink,
	.readlink    = _readlink,
	.read        = _read,
	.write       = _write,
	.truncate    = _truncate,
	.rename      = _rename,

	.flag_nullpath_ok   = 1,
	.flag_nopath        = 1,
};

static int
_optproc(void* data, const char* arg, int key, struct fuse_args* args)
{
	return 0;
}

int schemafs_run(int argc, char* argv[], Schema* schema,
		const char* source, const char* mountpoint, const char* options,
		bool multithreaded)
{
	int r;
	struct fuse_args  args = FUSE_ARGS_INIT(argc, argv);
	struct fuse_chan* chan;
	struct fuse*      fuse;

	struct fuse_opt opts[] = {
		FUSE_OPT_END
	};

	r = fuse_opt_parse(&args, NULL, opts, _optproc);
	if (r != 0) {
		log_err("fuse_opt_parse(): %d: %m", r);
		exit(EXIT_FAILURE);
	}

	r = fuse_opt_add_arg(&args, "-o");
	if (r != 0) {
		log_err("fuse_opt_add_arg() error: %m");
		exit(EXIT_FAILURE);
	}
	r = fuse_opt_add_arg(&args, options);
	if (r != 0) {
		log_err("fuse_opt_add_arg() error: %m");
		exit(EXIT_FAILURE);
	}

	char* fsname = malloc(strlen("fsname=") + strlen(source) + 1);
	sprintf(fsname, "fsname=%s", source);
	r = fuse_opt_add_opt(&args.argv[args.argc-1], fsname);
	free(fsname);
	if (r != 0) {
		log_err("fuse_opt_add_arg() error: %m");
		exit(EXIT_FAILURE);
	}

	chan = fuse_mount(mountpoint, &args);
	if (!chan) {
		log_err("fuse_mount(%s) error: %m", mountpoint);
		exit(EXIT_FAILURE);
	}

	fuse = fuse_new(chan, &args, &_ops,
		sizeof(_ops), schema);
	if (!fuse) {
		log_err("fuse_new() error: %m");
		exit(EXIT_FAILURE);
	}

	if (multithreaded)
		return fuse_loop_mt(fuse);
	else
		return fuse_loop(fuse);
}
