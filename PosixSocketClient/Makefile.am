AM_CPPFLAGS = -DDEBUG -D_GNU_SOURCE
AM_CXXFLAGS = $(DEFAULT_CXXFLAGS)
AM_LDFLAGS =

headerdir = $(includedir)/twsapi


lib_LIBRARIES =
lib_LIBRARIES += libtwsapi.a
libtwsapi_a_SOURCES =
libtwsapi_a_SOURCES += src/EClientSocketBase.cpp
libtwsapi_a_SOURCES += src/EPosixClientSocket.cpp
header_HEADERS =
header_HEADERS += $(top_srcdir)/Shared/CommonDefs.h
header_HEADERS += $(top_srcdir)/Shared/CommissionReport.h
header_HEADERS += $(top_srcdir)/Shared/Contract.h
header_HEADERS += $(top_srcdir)/Shared/EClient.h
header_HEADERS += $(top_srcdir)/Shared/EClientSocketBase.h
header_HEADERS += $(top_srcdir)/Shared/EWrapper.h
header_HEADERS += $(top_srcdir)/Shared/Execution.h
header_HEADERS += $(top_srcdir)/Shared/IBString.h
header_HEADERS += $(top_srcdir)/Shared/Order.h
header_HEADERS += $(top_srcdir)/Shared/OrderState.h
header_HEADERS += $(top_srcdir)/Shared/ScannerSubscription.h
header_HEADERS += $(top_srcdir)/Shared/TagValue.h
header_HEADERS += $(top_srcdir)/Shared/shared_ptr.h
header_HEADERS += src/EPosixClientSocket.h
header_HEADERS += $(top_srcdir)/Shared/TwsSocketClientErrors.h
noinst_HEADERS =
noinst_HEADERS += $(top_srcdir)/Shared/EClientSocketBaseImpl.h
noinst_HEADERS += $(top_srcdir)/Shared/TwsSocketClientErrors.h
noinst_HEADERS += src/EPosixClientSocketPlatform.h

EXTRA_libtwsapi_a_SOURCES =
libtwsapi_a_LIBADD =


libtwsapi_a_CPPFLAGS =
libtwsapi_a_CPPFLAGS += $(AM_CPPFLAGS)
libtwsapi_a_CPPFLAGS += -I$(top_srcdir)/Shared

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

