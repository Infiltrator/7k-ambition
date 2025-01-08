noinst_LIBRARIES = libambition.a
libambition_a_SOURCES = \
	Ambition_config.cc \
	Ambition_version.cc \
	Ambition_vga.cc

AM_CPPFLAGS = \
	-I$(top_srcdir)/include/ambition

AM_CXXFLAGS = $(GLOBAL_CFLAGS)
AM_CXXFLAGS += $(AMBITION_CFLAGS)
AM_CXXFLAGS += $(SDL_CFLAGS)

git_string.h:
	echo -n '#define GIT_STRING "' > git_string.h \
	&& git describe --always --dirty --long --tags \
		| cut -f2- -d- | tr -d '\n' >> git_string.h \
	&& echo '"' >> git_string.h

.PHONY: git_string.h

all-am: git_string.h
