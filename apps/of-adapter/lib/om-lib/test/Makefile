
INC_DIR = ../
LIB_STATIC_DIR = ../
CXXFLAGS = $(shell pkg-config --cflags dbus-1) -ldbus-1 -I$(INC_DIR) \
	-L$(LIB_STATIC_DIR) -Wall -std=c++11

TESTS = test_ipc_dbus_message

all: run

%: %.cc ../libom-net.a ../libom-tools.a ../libom-async.a ../libom-ipc.a \
	../libom-container.a 
	$(CXX) $(CXXFLAGS) $? -o $@ 

run: $(TESTS)
	@for test in $(TESTS); do ./$${test}; done

clean:
	$(RM) $(TESTS)

.PHONY: all run clean
