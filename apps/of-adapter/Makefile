#
# yanc: Yet Another Network Controller
# OpenFlow Adapter
#
# (c) 2013  Matthew Monaco  <matthew.monaco@colorado.edu>
#           Oliver Michel   <oliver.michel@colorado.edu>
#
# License: TBD
#

# macro definitions

ADAPTER_BIN_NAME = yanc-of-adapter
LIB_SOURCES = $(wildcard ../../lib/C++/*.cc)
ADAPTER_OBJS = \
	src/adapter_main.o \
	src/yanc_of_adapter.o \
	src/openflow_tools.o \
	src/of_network.o \
	src/of_switch.o \
	src/of_port.o \
	src/of_flow.o \
	src/of_match.o \
	src/of_actions.o \
	src/of_packet_in.o \
	src/of_packet_out.o \
	$(LIB_SOURCES:.cc=.o)

OM_LIBS  = lib/om-lib/libom-net.a lib/om-lib/libom-tools.a lib/om-lib/libom-async.a lib/om-lib/libom-ipc.a

CXX = g++
CXXFLAGS = -std=c++11 -g -Ilib/om-lib -Ilib -I../../lib/C++/ -Wall
LDFLAGS  = -Wall -Llib/om-lib # -L isn't currently used

DBUS_I = $(shell pkg-config --cflags-only-I dbus-1)
DBUS_L = $(shell pkg-config --libs dbus-1)

all: $(ADAPTER_BIN_NAME)

# general compile rule

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(DBUS_I) -c $< -o $@

# executables

$(ADAPTER_BIN_NAME): $(ADAPTER_OBJS) $(OM_LIBS)
	$(CXX) -o $@ $(LDFLAGS) $^ $(DBUS_L)

$(OM_LIBS):
	$(MAKE) -C lib/om-lib

# special targets

clean:
	$(RM) $(ADAPTER_OBJS) .deps
	$(MAKE) -C lib/om-lib clean

spotless: clean
	$(RM) $(ADAPTER_BIN_NAME)
	$(MAKE) -C lib/om-lib spotless

-include .deps
.deps: $(ADAPTER_OBJS:.o=.cc)
	for src in $^; do \
		$(CXX) $(CXXFLAGS) -MM -MT $${src/.cc/.o} $$src; \
	done > $@ 2> /dev/null

.PHONY: all clean spotless om-lib
