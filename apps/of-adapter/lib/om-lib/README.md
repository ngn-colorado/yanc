Olli's C++ Library
==================

1 General
---------

A general purpose C++ library with a focus on systems and networking tasks.

[https://bitbucket.org/omichel/om-lib](https://bitbucket.org/omichel/om-lib)


2 Contributors
--------------

* [Oliver Michel](http://ngn.cs.colorado/~oliver)
 <oliver dot michel at editum dot de>

* [Matthew Monaco](http://ngn.cs.colorado/~matt)
 <matt at 0x01b dot net>

3 Usage
-------

### Integrate into Project

* Add a git submodule: `git submodule add https://omichel@bitbucket.org/omichel/om-lib.git`
* Add to your build environment:
```
#!Makefile
$(OM_LIBS):
        $(MAKE) -C lib/om-lib # or whereever the library is located
```
### Use Static Library

* Choose which sub-libraries you require
```
#!Makefile
OM_LIBS  = lib/om-lib/libom-net.a lib/om-lib/libom-tools.a
```
* Add the static libraries as dependencies to your main executable
```
#!Makefile
$(EXE): $(OBJS) $(OM_LIBS)
	$(CXX) -o $@ $(LDFLAGS) $^
```

4 Modules
---------

###net

* DatagramSocket
* RawSocket
* StreamClient
* StreamListener
* StreamConnection
* TunnelDevice

###tools

* dir
* file
* logger
* random
* string
* time

###async
	
* EPollWrapper
* MultiplexInterface
* Multiplexer

###ipc
	
* dbus::Connection
* dbus::Message

###container

* RingBuffer