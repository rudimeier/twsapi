AM_CPPFLAGS = -DDEBUG -D_GNU_SOURCE
AM_CXXFLAGS = $(DEFAULT_CXXFLAGS)
AM_LDFLAGS =

header_HEADERS =
noinst_HEADERS =
noinst_PROGRAMS =
lib_LTLIBRARIES =

twsapi_musthave_cpp =
twsapi_musthave_cpp += client/ContractCondition.cpp
twsapi_musthave_cpp += client/DefaultEWrapper.cpp
twsapi_musthave_cpp += client/EClient.cpp
twsapi_musthave_cpp += client/EDecoder.cpp
twsapi_musthave_cpp += client/EMessage.cpp
twsapi_musthave_cpp += client/ESocket.cpp
twsapi_musthave_cpp += client/executioncondition.cpp
twsapi_musthave_cpp += client/MarginCondition.cpp
twsapi_musthave_cpp += client/OperatorCondition.cpp
twsapi_musthave_cpp += client/OrderCondition.cpp
twsapi_musthave_cpp += client/PercentChangeCondition.cpp
twsapi_musthave_cpp += client/PriceCondition.cpp
twsapi_musthave_cpp += client/SoftDollarTier.cpp
twsapi_musthave_cpp += client/StdAfx.cpp
twsapi_musthave_cpp += client/TimeCondition.cpp
twsapi_musthave_cpp += client/VolumeCondition.cpp
twsapi_musthave_h =
twsapi_musthave_h += client/bar.h
twsapi_musthave_h += client/CommissionReport.h
twsapi_musthave_h += client/CommonDefs.h
twsapi_musthave_h += client/ContractCondition.h
twsapi_musthave_h += client/Contract.h
twsapi_musthave_h += client/DefaultEWrapper.h
twsapi_musthave_h += client/DepthMktDataDescription.h
twsapi_musthave_h += client/EClient.h
twsapi_musthave_h += client/EClientMsgSink.h
twsapi_musthave_h += client/EDecoder.h
twsapi_musthave_h += client/EMessage.h
twsapi_musthave_h += client/ESocket.h
twsapi_musthave_h += client/ETransport.h
twsapi_musthave_h += client/EWrapper.h
twsapi_musthave_h += client/EWrapper_prototypes.h
twsapi_musthave_h += client/executioncondition.h
twsapi_musthave_h += client/Execution.h
twsapi_musthave_h += client/FamilyCode.h
twsapi_musthave_h += client/HistogramEntry.h
twsapi_musthave_h += client/HistoricalTickBidAsk.h
twsapi_musthave_h += client/HistoricalTick.h
twsapi_musthave_h += client/HistoricalTickLast.h
twsapi_musthave_h += client/IExternalizable.h
twsapi_musthave_h += client/MarginCondition.h
twsapi_musthave_h += client/NewsProvider.h
twsapi_musthave_h += client/OperatorCondition.h
twsapi_musthave_h += client/OrderCondition.h
twsapi_musthave_h += client/Order.h
twsapi_musthave_h += client/OrderState.h
twsapi_musthave_h += client/PercentChangeCondition.h
twsapi_musthave_h += client/PriceCondition.h
twsapi_musthave_h += client/PriceIncrement.h
twsapi_musthave_h += client/ScannerSubscription.h
twsapi_musthave_h += client/SoftDollarTier.h
twsapi_musthave_h += client/StdAfx.h
twsapi_musthave_h += client/TagValue.h
twsapi_musthave_h += client/TickAttrib.h
twsapi_musthave_h += client/TimeCondition.h
twsapi_musthave_h += client/TwsSocketClientErrors.h
twsapi_musthave_h += client/VolumeCondition.h

lib_LTLIBRARIES += libtwsapi.la
libtwsapi_la_SOURCES =
libtwsapi_la_LIBADD =
libtwsapi_la_CPPFLAGS =
libtwsapi_la_CPPFLAGS += $(AM_CPPFLAGS) -fPIC
libtwsapi_la_CPPFLAGS += -I$(srcdir)/client
## must have
libtwsapi_la_SOURCES += $(twsapi_musthave_cpp)
header_HEADERS += $(twsapi_musthave_h)
noinst_HEADERS += client/EPosixClientSocketPlatform.h
## suppl
libtwsapi_la_SOURCES += suppl/EPosixClientSocket.cpp
libtwsapi_la_SOURCES += suppl/EReaderST.cpp
header_HEADERS += suppl/EPosixClientSocket.h
header_HEADERS += suppl/EReaderST.h

if WITH_IBORIG
lib_LTLIBRARIES += libtwsapi-iborig.la
libtwsapi_iborig_la_SOURCES =
libtwsapi_iborig_la_LIBADD =
libtwsapi_iborig_la_LDFLAGS =
libtwsapi_iborig_la_LDFLAGS += -pthread
libtwsapi_iborig_la_CPPFLAGS =
libtwsapi_iborig_la_CPPFLAGS += -pthread
libtwsapi_iborig_la_CPPFLAGS += $(AM_CPPFLAGS) -fPIC
libtwsapi_iborig_la_CPPFLAGS += -I$(srcdir)/client
## must have
libtwsapi_iborig_la_SOURCES += $(twsapi_musthave_cpp)
header_HEADERS += $(twsapi_musthave_h)
noinst_HEADERS += client/EPosixClientSocketPlatform.h
## iborig
libtwsapi_iborig_la_SOURCES += client/EClientSocket.cpp
libtwsapi_iborig_la_SOURCES += client/EMutex.cpp
libtwsapi_iborig_la_SOURCES += client/EReader.cpp
libtwsapi_iborig_la_SOURCES += client/EReaderOSSignal.cpp
header_HEADERS += client/EClientSocket.h
header_HEADERS += client/EMutex.h
header_HEADERS += client/EReader.h
header_HEADERS += client/EReaderOSSignal.h
header_HEADERS += client/EReaderSignal.h
if HAVE_SSL
## ssl
libtwsapi_iborig_la_SOURCES += ssl/EClientSocketSSL.cpp
libtwsapi_iborig_la_SOURCES += ssl/EReaderSSL.cpp
libtwsapi_iborig_la_SOURCES += ssl/ESocketSSL.cpp
libtwsapi_iborig_la_SOURCES += ssl/StdAfx.cpp
header_HEADERS += ssl/EClientSocketSSL.h
header_HEADERS += ssl/EReaderSSL.h
header_HEADERS += ssl/ESocketSSL.h
header_HEADERS += ssl/StdAfx.h
libtwsapi_iborig_la_CPPFLAGS += $(openssl_CFLAGS)
endif
endif

if WITH_IBORIG
noinst_PROGRAMS += testclient
testclient_SOURCES =
testclient_SOURCES += TestCppClient/AccountSummaryTags.cpp
testclient_SOURCES += TestCppClient/AvailableAlgoParams.cpp
testclient_SOURCES += TestCppClient/ContractSamples.cpp
testclient_SOURCES += TestCppClient/Main.cpp
testclient_SOURCES += TestCppClient/OrderSamples.cpp
testclient_SOURCES += TestCppClient/ScannerSubscriptionSamples.cpp
testclient_SOURCES += TestCppClient/StdAfx.cpp
testclient_SOURCES += TestCppClient/TestCppClient.cpp
testclient_SOURCES += TestCppClient/Utils.cpp
noinst_HEADERS += TestCppClient/AccountSummaryTags.h
noinst_HEADERS += TestCppClient/AvailableAlgoParams.h
noinst_HEADERS += TestCppClient/ContractSamples.h
noinst_HEADERS += TestCppClient/FAMethodSamples.h
noinst_HEADERS += TestCppClient/OrderSamples.h
noinst_HEADERS += TestCppClient/ScannerSubscriptionSamples.h
noinst_HEADERS += TestCppClient/StdAfx.h
noinst_HEADERS += TestCppClient/TestCppClient.h
noinst_HEADERS += TestCppClient/Utils.h
testclient_LDADD =
testclient_LDADD += libtwsapi-iborig.la
testclient_CPPFLAGS =
testclient_CPPFLAGS += $(AM_CPPFLAGS)
testclient_CPPFLAGS += -I$(srcdir)/client
testclient_LDFLAGS = $(AM_LDFLAGS)

if HAVE_SSL
noinst_PROGRAMS += testclientssl
testclientssl_SOURCES =
testclientssl_SOURCES += TestCppClientSSL/Main.cpp
testclientssl_SOURCES += TestCppClientSSL/StdAfx.cpp
testclientssl_SOURCES += TestCppClientSSL/TestCppClientSSL.cpp
noinst_HEADERS += TestCppClientSSL/TestCppClientSSL.h
noinst_HEADERS += TestCppClientSSL/StdAfx.h
testclientssl_LDADD =
testclientssl_LDADD += libtwsapi-iborig.la
testclientssl_LDADD += $(openssl_LIBS)
testclientssl_CPPFLAGS =
testclientssl_CPPFLAGS += $(AM_CPPFLAGS)
testclientssl_CPPFLAGS += -I$(srcdir)/client
testclientssl_CPPFLAGS += $(openssl_CFLAGS)
testclientssl_LDFLAGS = $(AM_LDFLAGS)
endif
endif

CLEANFILES =
CLEANFILES += *.s
CLEANFILES += *.i

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

