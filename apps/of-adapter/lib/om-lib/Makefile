#
#  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
#  Makefile
#  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
#  http://ngn.cs.colorado/~oliver
#

# shared target names
LIB_NET       = libom-net.so
LIB_TOOLS     = libom-tools.so
LIB_ASYNC     = libom-async.so
LIB_IPC       = libom-ipc.so
LIB_CONTAINER = libom-container.so

# static target names
AR_NET       = libom-net.a
AR_TOOLS     = libom-tools.a
AR_ASYNC     = libom-async.a
AR_IPC       = libom-ipc.a
AR_CONTAINER = libom-container.a

# dependency names
NET_NAMES       = net agent io_interface datagram_socket stream_client \
		            stream_listener stream_connection inotify_handler \
		            tunnel_device
TOOLS_NAMES     = tools logger time random string file dir
ASYNC_NAMES     = multiplex_interface multiplexer epoll_wrapper
IPC_NAMES       = dbus/dbus dbus/connection dbus/message
CONTAINER_NAMES = ring_buffer

# external libary flags
DBUS_I = $(shell pkg-config --cflags-only-I dbus-1)
DBUS_L = $(shell pkg-config --libs dbus-1)

# compiler/linker flags
AR       = ar
CXX      = g++
CXXFLAGS = -std=c++11 -Wall -I. -fPIC
LDFLAGS  = 

# object files
NET_OBJS       = $(addprefix om/net/,       $(addsuffix .o, $(NET_NAMES)))
TOOLS_OBJS     = $(addprefix om/tools/,     $(addsuffix .o, $(TOOLS_NAMES)))
ASYNC_OBJS     = $(addprefix om/async/,     $(addsuffix .o, $(ASYNC_NAMES)))
IPC_OBJS       = $(addprefix om/ipc/,       $(addsuffix .o, $(IPC_NAMES)))
CONTAINER_OBJS = $(addprefix om/container/, $(addsuffix .o, $(CONTAINER_NAMES)))

# target rules
all: so ar
so: $(LIB_NET) $(LIB_TOOLS) $(LIB_ASYNC) $(LIB_IPC) $(LIB_CONTAINER)
ar: $(AR_NET) $(AR_TOOLS) $(AR_ASYNC) $(AR_IPC) $(AR_CONTAINER)

# target dependencies
$(AR_NET):       $(NET_OBJS)
$(AR_TOOLS):     $(TOOLS_OBJS)
$(AR_ASYNC):     $(ASYNC_OBJS)
$(AR_IPC):       $(IPC_OBJS)
$(AR_CONTAINER): $(CONTAINER_OBJS)

# per library compile/link rules

om/net/%.o: om/net/%.cc om/net/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

om/tools/%.o: om/tools/%.cc om/tools/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

om/async/%.o: om/async/%.cc om/async/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

om/ipc/dbus/%.o: om/ipc/dbus/%.cc om/ipc/dbus/%.h 
	$(CXX) $(CXXFLAGS) $(DBUS_I) -c $< -o $@

om/ipc/%.o: om/ipc/%.cc om/ipc/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

om/container/%.o: om/container/%.cc om/container/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_NET): $(NET_OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS) $(DBUS_L)

$(LIB_TOOLS): $(TOOLS_OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

$(LIB_ASYNC): $(ASYNC_OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

$(LIB_IPC): $(IPC_OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS) $(DBUS_L)

$(LIB_CONTAINER): $(CONTAINER_OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)


%.a:
	$(AR) -cvr $@ $^

test:
	$(MAKE) -C ./test

examples:
	$(MAKE) -C ./examples

clean:
	$(RM) $(NET_OBJS) $(TOOLS_OBJS) $(ASYNC_OBJS) $(IPC_OBJS) $(CONTAINER_OBJS)

spotless: clean
	$(RM) $(LIB_NET) $(LIB_TOOLS) $(LIB_ASYNC) $(LIB_IPC) $(LIB_CONTAINER)
	$(RM) $(AR_NET) $(AR_TOOLS) $(AR_ASYNC) $(AR_IPC) $(AR_CONTAINER)

.PHONY: all ar so examples clean spotless test
