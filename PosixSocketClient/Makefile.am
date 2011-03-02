AM_CPPFLAGS = -DDEBUG -D_GNU_SOURCE
AM_LDFLAGS =

headerdir = $(includedir)/ibtws


lib_LIBRARIES =
lib_LIBRARIES += libibtws.a
libibtws_a_SOURCES =
libibtws_a_SOURCES += src/EClientSocketBase.cpp
libibtws_a_SOURCES += src/EPosixClientSocket.cpp
header_HEADERS =
header_HEADERS += ../Shared/CommonDefs.h
header_HEADERS += ../Shared/Contract.h
header_HEADERS += ../Shared/EClient.h
header_HEADERS += ../Shared/EClientSocketBase.h
header_HEADERS += ../Shared/EWrapper.h
header_HEADERS += ../Shared/Execution.h
header_HEADERS += ../Shared/IBString.h
header_HEADERS += ../Shared/Order.h
header_HEADERS += ../Shared/OrderState.h
header_HEADERS += ../Shared/ScannerSubscription.h
header_HEADERS += ../Shared/shared_ptr.h
header_HEADERS += ../Shared/TwsSocketClientErrors.h
noinst_HEADERS =
noinst_HEADERS += ../Shared/EClientSocketBaseImpl.h
noinst_HEADERS += src/EPosixClientSocket.h
noinst_HEADERS += src/EPosixClientSocketPlatform.h

EXTRA_libibtws_a_SOURCES =
libibtws_a_LIBADD =


libibtws_a_CPPFLAGS =
libibtws_a_CPPFLAGS += $(AM_CPPFLAGS)
libibtws_a_CPPFLAGS += -I../Shared
libibtws_a_CXXFLAGS =
libibtws_a_CXXFLAGS += $(DEFAULT_CXXFLAGS)
libibtws_a_CXXFLAGS += -Wno-switch

CLEANFILES =
CLEANFILES += *.s
CLEANFILES += *.i

# 
# Help the developers get nice post-processed source files

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<
.cpp.i:
	$(CXXCOMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(cflags) $<
.cpp.s:
	$(CXXCOMPILE) -S -c $(cflags) $<

