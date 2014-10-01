prog = schema
objs = yanc-schema.o libschema.o schema.yy.o schema.tab.o

CC       = gcc -Wall -g
LEX      = flex
YACC     = bison -d
LDFLAGS  = -lfl

yanc-$(prog): $(objs)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $<

%.yy.c: %.l %.tab.h
	$(LEX) -o $@ $<


%.tab.h %.tab.c: %.y
	$(YACC) $<

-include .deps
.deps: $(objs:.o=.c)
	for file in $^; do \
		$(CC) $(CFLAGS) -MM -MT $${file/.c/.o} $$file ; \
	done > .deps 2>/dev/null

clean:
	$(RM) $(objs) $(prog).yy.c $(prog).tab.h $(prog).tab.c .deps

spotless: clean
	$(RM) yanc-$(prog)

.PHONY: clean spotless
