AM_CPPFLAGS = -DDEBUG -D_GNU_SOURCE
AM_LDFLAGS =

headerdir = $(includedir)/twsapi


lib_LIBRARIES =
lib_LIBRARIES += libtwsapi.a
libtwsapi_a_SOURCES =
libtwsapi_a_SOURCES += src/EClientSocketBase.cpp
libtwsapi_a_SOURCES += src/EPosixClientSocket.cpp
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
header_HEADERS += src/EPosixClientSocket.h
header_HEADERS += ../Shared/TwsSocketClientErrors.h
noinst_HEADERS =
noinst_HEADERS += ../Shared/EClientSocketBaseImpl.h
noinst_HEADERS += ../Shared/TwsSocketClientErrors.h
noinst_HEADERS += src/EPosixClientSocketPlatform.h

EXTRA_libtwsapi_a_SOURCES =
libtwsapi_a_LIBADD =


libtwsapi_a_CPPFLAGS =
libtwsapi_a_CPPFLAGS += $(AM_CPPFLAGS)
libtwsapi_a_CPPFLAGS += -I../Shared
libtwsapi_a_CXXFLAGS =
libtwsapi_a_CXXFLAGS += $(DEFAULT_CXXFLAGS)
libtwsapi_a_CXXFLAGS += -Wno-switch

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

