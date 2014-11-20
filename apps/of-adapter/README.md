Yanc OpenFlow Adapter
=====================

*Connects the yanc filesystem to multiple OpenFlow switches.*

(c) 2013 Oliver Michel and Matthew Monaco, University of Colorado Boulder

Compilation
-----------

First make sure that you have the om-lib [1] submodule.

	$ git submodule init
	$ git submodule update

Then simply

	$ make

Currently, there is not an `install` target.

Usage
-----
`yanc-of-adapter [-v[v[v]]] [-h] [-p listening-port] <yanc-fs-path> <dbus-addr>`

References
----------

[1] [https://bitbucket.org/omichel/om-lib](https://bitbucket.org/omichel/om-lib)
