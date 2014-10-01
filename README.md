# Yanc

Yanc is a collection of programs which make up a software defined
network (SDN) control plane. The core component of Yanc is a
schema-based filesystem. Also included in this repository are an
OpenFlow driver, a Bro driver, and some basic SDN applications.

Over the next couple of weeks, we will import each component into this
repository.

## Building the Schema Filesystem

The only dependency for the filesystem is
[FUSE](http://fuse.sourceforge.net). On Debian-based distributions, this
would be `libfuse-dev`. On Archlinux, simply `fuse`.

Building and running should then be very straightforward:

	make
	sudo mkdir /net
	sudo chown <user>:<group> /net
	./yanc -f /net

Using `/net` is optional, but it's what we've grown accustomed to using.

<!--- vim: set noet tw=72 nofoldenable : -->
