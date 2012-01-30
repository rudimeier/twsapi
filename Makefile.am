LANG = C
LC_ALL = C

SUBDIRS = PosixSocketClient TestPosixSocketClient

doc_DATA =
doc_DATA += README
doc_DATA += ChangeLog

dist_doc_DATA =
dist_doc_DATA += API_VersionNum.txt

EXTRA_DIST =
EXTRA_DIST += @PACKAGE@.spec

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = lib@PACKAGE@.pc

# Rule to build RPM distribution package
rpm: dist-gzip
	rpmbuild -ta $(distdir).tar.gz
