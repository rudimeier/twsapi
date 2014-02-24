LANG = C
LC_ALL = C

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = PosixSocketClient TestPosixSocketClient

nodist_header_HEADERS =
nodist_header_HEADERS += twsapi_config.h

twsapi_config.h: config.h API_VersionNum.txt gen_twsapi_config_h \
	Shared/EClientSocketBaseImpl.h
	$(srcdir)/gen_twsapi_config_h config.h $(srcdir)/API_VersionNum.txt \
		$(srcdir)/Shared/EClientSocketBaseImpl.h > $@.tmp \
		&& mv $@.tmp $@

dist_doc_DATA =
dist_doc_DATA += API_VersionNum.txt
dist_doc_DATA += ChangeLog
dist_doc_DATA += ChangeLog-IB
dist_doc_DATA += README.md

EXTRA_DIST =
EXTRA_DIST += @PACKAGE@.spec

dist_noinst_SCRIPTS =
dist_noinst_SCRIPTS += gen_twsapi_config_h

distclean-local:
	-rm -f twsapi_config.h

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc

## Rule to build RPM distribution package
rpm: dist-gzip
	rpmbuild -ta $(distdir).tar.gz
