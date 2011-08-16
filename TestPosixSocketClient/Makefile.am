AM_CPPFLAGS = -DDEBUG -D_GNU_SOURCE
AM_LDFLAGS =

headerdir = $(includedir)/twsapi


noinst_PROGRAMS =
noinst_PROGRAMS += TestPosixSocketClient
TestPosixSocketClient_SOURCES =
TestPosixSocketClient_SOURCES += PosixTestClient.cpp
TestPosixSocketClient_SOURCES += Main.cpp
noinst_HEADERS =
noinst_HEADERS += PosixTestClient.h
EXTRA_TestPosixSocketClient_SOURCES =
TestPosixSocketClient_LDADD =
TestPosixSocketClient_LDADD += -L$(top_builddir)/PosixSocketClient
TestPosixSocketClient_LDADD += -ltwsapi
TestPosixSocketClient_CPPFLAGS =
TestPosixSocketClient_CPPFLAGS += $(AM_CPPFLAGS)
TestPosixSocketClient_CPPFLAGS += -I$(top_srcdir)/PosixSocketClient/src
TestPosixSocketClient_CPPFLAGS += -I$(top_srcdir)/Shared
TestPosixSocketClient_CXXFLAGS =
TestPosixSocketClient_CXXFLAGS += $(DEFAULT_CXXFLAGS)
TestPosixSocketClient_CXXFLAGS += -Wno-unused-parameter
TestPosixSocketClient_LDFLAGS = $(AM_LDFLAGS)

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

