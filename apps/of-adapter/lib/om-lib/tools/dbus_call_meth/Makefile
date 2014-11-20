#
#  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
#  tools/dbus_call_meth/Makefile
#  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
#  http://ngn.cs.colorado/~oliver
#

INC_DIR = ../../
LIB_STATIC_DIR = ../../

CXXFLAGS = $(shell pkg-config --cflags dbus-1) -ldbus-1 -I$(INC_DIR) -L$(LIB_STATIC_DIR) -Wall -std=c++11
CXXPOSTFIX = -lom-net -lom-tools

BIN_NAME = dbus_call_meth

all: $(BIN_NAME)

%: %.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(CXXPOSTFIX)

spotless:
	$(RM) $(BIN_NAME)
