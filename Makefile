
progname    = yanc
objects     = yanc.o \
              util/util.o \
              fs/libschema.o \
              fs/schemafs.o \
              fs/types.o \

schema_objs = fs/schema.yy.o fs/schema.tab.o

pkgs     = fuse
CC       = gcc
CFLAGS   = -Wall -std=gnu99 -g -I. -pthread $(shell pkg-config --cflags $(pkgs))
LDFLAGS  = -Wall -g $(shell pkg-config --libs $(pkgs)) -lpthread -lfl

$(progname): $(objects) $(schema_objs)
	$(CC) $^ -o $@ $(LDFLAGS) $(SCHEMA_LDFLAGS)

$(schema_objs): fs/schema.y fs/schema.l fs/schema.h fs/libschema.c
	$(MAKE) -C fs $(notdir $@)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include .deps
.deps: $(objects:.o=.c)
	for file in $^; do \
		$(CC) $(CFLAGS) -MM -MT $${file/.c/.o} $$file ; \
	done > .deps 2>/dev/null

clean:
	$(RM) $(objects) .deps
	$(MAKE) -C test clean
	$(MAKE) -C fs clean

spotless: clean
	$(RM) $(progname)
	$(MAKE) -C test spotless
	$(MAKE) -C fs spotless

check:
	$(MAKE) -C test

.PHONY: clean spotless check
