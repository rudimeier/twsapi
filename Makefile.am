LANG = C
LC_ALL = C

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = PosixSocketClient TestPosixSocketClient

dist_doc_DATA =
dist_doc_DATA += API_VersionNum.txt
dist_doc_DATA += ChangeLog
dist_doc_DATA += ChangeLog-IB
dist_doc_DATA += README

EXTRA_DIST =
EXTRA_DIST += @PACKAGE@.spec

dist_noinst_SCRIPTS =
dist_noinst_SCRIPTS += gen_twsapi_config_h

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = lib@PACKAGE@.pc
pkgconfig_DATA += @PACKAGE@.pc

# Rule to build RPM distribution package
rpm: dist-gzip
	rpmbuild -ta $(distdir).tar.gz
