
Portable C++ API for Interactive Brokers TWS
============================================


This twsapi is almost identical to the original IB C++ Posix API. It contains
several bugfixes, usage improvements and it's using the autotools build system.
For more details read "Changes of original IB API" below.

The project homepage (bugtracker, latest git repo) is hosted on [github]
 (https://github.com/rudimeier/twsapi)
Released source tarballs download [here]
 (https://bitbucket.org/rudimeier/twsapi/downloads)
Binary rpms for various Linux distros [here]
 (http://software.opensuse.org/download.html?project=home:rudi_m&package=twsapi)




Installation
------------

 Building from source requires a C++ compiler. It has been very well tested with
 GNU gcc/g++ and Intel icc/icpc on various Linux distros, MacOSX and
 Windows/Cygwin. Native Windows/Mingw is also supported.

 When building from git checkout you need both autotools and libtool.  Also, Don't forget to
 type `autoreconf -vfi` first.

```bash
 ./configure
 make
 make install
```




Repository layout
-----------------

Beside the usual master branch I keep two important branches:
 - ib_orig: the original IB API code from upstream
 - ib_orig_plus: the original IB API plus namespace IB

Both are used to commit upstream updates to be merged into master.

BTW don't blame me for the strange project layout. This comes original from IB
and I don't change it for simply git merging upstream updates.




Changes of original IB API
--------------------------

Here I point out some of the most important things about our modified TWS API.
(This documentation may not be entirely complete.)

__Improvements:__
 - Comfortable `autotools` build chain and `pkg-config` support to make developers
   happy.
 - Generally the socket error handling is stable and reliable now. Error message
   callbacks with code `SOCKET_EXCEPTION` are using standard message strings
   (errno/strerror).
 - Never throwing nor catching any Exceptions.
 - Using non-blocking sockets with well defined connection timeout.
 - Hostnames will be resolved.
 - IPv6 support.
 - Source compatibility between API updates.


Below are some details about the improved reliability of the API interface functions
and callbacks.

__eConnect():__
 1. It's blocking and returns either connected or disconnected.
 2. It may callback informative error messages only. No more checking of
    regular messages.
 3. If any callback is fired then state will still be  "disconnected".
    Return false in this case. There are no "positive callbacks".
 4. There can be multiple callbacks, for example `UPDATE_TWS` and then
    `CONNECT_FAIL` but 2. and 3. are always in place! The client programmer
    does not need to do any cleanup actions on these callbacks.
 5. Before returning false `CONNECT_FAIL` with descriptive message will be fired
    fore sure.


__onReceive():__
 1. Any callback (except `connectionClosed()`) will be fired in state connected,
    even if we are going to be disconnected.
 2. Before `connectionClosed()` is fired we will get a `SOCKET_EXCEPTION`
    callback still in state connected.


__all other req*() functions:__
 1. If called while disconnected then it sends a `NOT_CONNECTED` callback (we
    could have known before).
 2. On error it sends a `SOCKET_EXCEPTION` (still in state connected) and
    then connectionClosed() (already disconnected state).
 3. Another possible callback is `UPDATE_TWS` (does not disconnect us).



__Thus we know about callbacks:__
 1. Within `connectionClosed()` we are disconnected for sure.
 2. Within `error()` we could be disconnected only if code is `CONNECT_FAIL`,
    `UPDATE_TWS` or `NOT_CONNECTED`.
 3. In any other case callbacks will be received in state connected.


__TODO__: In opposite to the statements above `SOCKET_EXCEPTION` may still be fired in
      state connected if `onReceive()` or `onSend()` are called while disconnected
      (we could have known before). To be safe we could return early and send
      `NOT_CONNECTED`.


_API internal implementation notes:_

__bufferedRead()__
 1. Behaves like `::recv()` regarding errno and return value:
      -1: error, errno set
       0: EOF, disconnectd
      >0: success




License
-------

  95% of the source code comes from Interactive Brokers. Please read there
  about any licensing issues:
  http://www.interactivebrokers.com




Contact Information
-------------------

  If you have questions, bug reports, patches etc., contact
  Ruediger Meier <sweet_f_a@gmx.de> (in English or German).
