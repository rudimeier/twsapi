AM_CPPFLAGS = -DDEBUG -D_GNU_SOURCE
AM_CXXFLAGS = $(DEFAULT_CXXFLAGS)
AM_LDFLAGS =

header_HEADERS =
noinst_HEADERS =
noinst_PROGRAMS =
lib_LTLIBRARIES =

lib_LTLIBRARIES += libtwsapi.la
libtwsapi_la_SOURCES =
libtwsapi_la_SOURCES += client/ContractCondition.cpp
libtwsapi_la_SOURCES += client/DefaultEWrapper.cpp
libtwsapi_la_SOURCES += client/EClient.cpp
libtwsapi_la_SOURCES += client/EClientSocket.cpp
libtwsapi_la_SOURCES += client/EDecoder.cpp
libtwsapi_la_SOURCES += client/EMessage.cpp
libtwsapi_la_SOURCES += client/EMutex.cpp
libtwsapi_la_SOURCES += client/EReader.cpp
libtwsapi_la_SOURCES += client/EReaderOSSignal.cpp
libtwsapi_la_SOURCES += client/ESocket.cpp
libtwsapi_la_SOURCES += client/executioncondition.cpp
libtwsapi_la_SOURCES += client/MarginCondition.cpp
libtwsapi_la_SOURCES += client/OperatorCondition.cpp
libtwsapi_la_SOURCES += client/OrderCondition.cpp
libtwsapi_la_SOURCES += client/PercentChangeCondition.cpp
libtwsapi_la_SOURCES += client/PriceCondition.cpp
libtwsapi_la_SOURCES += client/SoftDollarTier.cpp
libtwsapi_la_SOURCES += client/StdAfx.cpp
libtwsapi_la_SOURCES += client/TimeCondition.cpp
libtwsapi_la_SOURCES += client/VolumeCondition.cpp
header_HEADERS += client/bar.h
header_HEADERS += client/CommissionReport.h
header_HEADERS += client/CommonDefs.h
header_HEADERS += client/ContractCondition.h
header_HEADERS += client/Contract.h
header_HEADERS += client/DefaultEWrapper.h
header_HEADERS += client/DepthMktDataDescription.h
header_HEADERS += client/EClient.h
header_HEADERS += client/EClientMsgSink.h
header_HEADERS += client/EClientSocket.h
header_HEADERS += client/EDecoder.h
header_HEADERS += client/EMessage.h
header_HEADERS += client/EMutex.h
header_HEADERS += client/EReader.h
header_HEADERS += client/EReaderOSSignal.h
header_HEADERS += client/EReaderSignal.h
header_HEADERS += client/ESocket.h
header_HEADERS += client/ETransport.h
header_HEADERS += client/EWrapper.h
header_HEADERS += client/executioncondition.h
header_HEADERS += client/Execution.h
header_HEADERS += client/FamilyCode.h
header_HEADERS += client/HistogramEntry.h
header_HEADERS += client/HistoricalTickBidAsk.h
header_HEADERS += client/HistoricalTick.h
header_HEADERS += client/HistoricalTickLast.h
header_HEADERS += client/IExternalizable.h
header_HEADERS += client/MarginCondition.h
header_HEADERS += client/NewsProvider.h
header_HEADERS += client/OperatorCondition.h
header_HEADERS += client/OrderCondition.h
header_HEADERS += client/Order.h
header_HEADERS += client/OrderState.h
header_HEADERS += client/PercentChangeCondition.h
header_HEADERS += client/PriceCondition.h
header_HEADERS += client/PriceIncrement.h
header_HEADERS += client/ScannerSubscription.h
header_HEADERS += client/shared_ptr.h
header_HEADERS += client/SoftDollarTier.h
header_HEADERS += client/StdAfx.h
header_HEADERS += client/TagValue.h
header_HEADERS += client/TickAttrib.h
header_HEADERS += client/TimeCondition.h
header_HEADERS += client/TwsSocketClientErrors.h
header_HEADERS += client/VolumeCondition.h
noinst_HEADERS += client/EPosixClientSocketPlatform.h
EXTRA_libtwsapi_la_SOURCES =
libtwsapi_la_LIBADD =
libtwsapi_la_CPPFLAGS =
libtwsapi_la_CPPFLAGS += $(AM_CPPFLAGS) -fPIC

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
noinst_HEADERS += TestCppClient/AccountSummaryTags.h
noinst_HEADERS += TestCppClient/AvailableAlgoParams.h
noinst_HEADERS += TestCppClient/ContractSamples.h
noinst_HEADERS += TestCppClient/FAMethodSamples.h
noinst_HEADERS += TestCppClient/OrderSamples.h
noinst_HEADERS += TestCppClient/ScannerSubscriptionSamples.h
noinst_HEADERS += TestCppClient/StdAfx.h
noinst_HEADERS += TestCppClient/TestCppClient.h
EXTRA_testclient_SOURCES =
testclient_LDADD =
testclient_LDADD += libtwsapi.la
testclient_CPPFLAGS =
testclient_CPPFLAGS += $(AM_CPPFLAGS)
testclient_CPPFLAGS += -Iclient
testclient_LDFLAGS = $(AM_LDFLAGS)

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

