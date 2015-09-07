AM_CPPFLAGS = @VMOD_INCLUDES@ -Wall -Werror

vmoddir = @VMOD_DIR@
vmod_LTLIBRARIES = libvmod_cowsay.la

libvmod_cowsay_la_LDFLAGS = -module -export-dynamic -avoid-version -shared

libvmod_cowsay_la_SOURCES = \
	vmod_cowsay.c

nodist_libvmod_cowsay_la_SOURCES = \
	vcc_if.c \
	vcc_if.h

vmod_cowsay.lo: vcc_if.c vcc_if.h

vcc_if.c: vcc_if.h

vcc_if.h: @VMODTOOL@ $(top_srcdir)/src/vmod_cowsay.vcc
	@VMODTOOL@ $(top_srcdir)/src/vmod_cowsay.vcc

VMOD_TESTS = $(top_srcdir)/src/tests/*.vtc
.PHONY: $(VMOD_TESTS)

$(top_srcdir)/src/tests/*.vtc: libvmod_cowsay.la
	@VARNISHTEST@ -Dvarnishd=@VARNISHD@ -Dvmod_topbuild=$(abs_top_builddir) $@

check: $(VMOD_TESTS)

EXTRA_DIST = \
	vmod_cowsay.vcc \
	$(VMOD_TESTS)

CLEANFILES = \
	$(builddir)/vcc_if.c \
	$(builddir)/vcc_if.h \
	$(builddir)/vmod_cowsay.rst \
	$(builddir)/vmod_cowsay.man.rst
