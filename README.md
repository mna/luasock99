# luasock99

A Lua module that exposes the [socket99] C library in Lua, making it easier to create TCP/UDP/Unix Domain client and server sockets.

* Canonical repository: https://git.sr.ht/~mna/luasock99
* Issue tracker: https://todo.sr.ht/~mna/luasock99

## Install

Via Luarocks:

```
$ luarocks install luasock99
```

## API

Assuming `local sock99 = require 'sock99'`. You can check out the tests for actual examples of using the API.

### `open(t)`

Open the socket as configured by table t, which may have the following fields:
* host (string): the hostname to connect to, for TCP/UDP sockets.
* port (integer): the port number to connect to, for TCP/UDP sockets.
* path (string): the Unix Domain socket path.
* ipv4 (string): IPv4 address, if host and path are missing.
* ipv6 (string): IPv6 address, instead of using host.
* server (bool): set to true to create a server socket (i.e. `listen` instead of `connect`).
* datagram (bool): set to true for datagram socket instead of stream.
* nonblocking (bool): set to true for a non-blocking socket.
* backlog (integer): the size of the backlog, for server socket.
TODO: add support for sockopts

It returns the file descriptor (integer) on success, or `nil` followed by a system-specific error message and error code on error. The file descriptor can be used with the standard POSIX calls (e.g. with `luaposix`: `posix.unistd.close`, `posix.sys.socket.send`, etc.).

Note that this is a raw integer, as such it cannot install a garbage collector finalizer to close itself - it must be closed explicitly.

TODO: for Lua 5.4, set a metatable with `__close` method.

## Development

Clone the project and install the required development dependencies:

* luaunit (unit test runner)
* luaposix (for tests)

If like me you prefer to keep your dependencies locally, per-project, then I recommend using my [llrocks] wrapper of the `luarocks` cli, which by default uses a local `lua_modules/` tree.

```
$ llrocks install ...
```

To run tests:

```
$ llrocks run test/*.lua
```

## License

The [BSD 3-clause][bsd] license for the Lua module. For the `socket99` C library, see the `deps/socket99/socket99.c` header.

[bsd]: http://opensource.org/licenses/BSD-3-Clause
[llrocks]: https://git.sr.ht/~mna/llrocks
[socket99]: https://github.com/silentbicycle/socket99
