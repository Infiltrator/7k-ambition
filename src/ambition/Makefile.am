SUBDIRS = 7kaaInterface

noinst_LIBRARIES = libambition.a
libambition_a_SOURCES = \
	Ambition_config.cc \
	Ambition_inn.cc \
	Ambition_input.cc \
	Ambition_minimap.cc \
	Ambition_remote.cc \
	Ambition_unit.cc \
	Ambition_version.cc \
	Ambition_vga.cc \
	# end

AM_CPPFLAGS = \
	-I$(top_srcdir)/include/ambition \
	# end

AM_CXXFLAGS = $(GLOBAL_CFLAGS)
AM_CXXFLAGS += $(AMBITION_CFLAGS)
AM_CXXFLAGS += $(SDL_CFLAGS)

Ambition_version.cc: git_string.h

git_string.h:
	git describe --always --dirty --long --tags \
    | awk --field-separator="-" \
      '{OFS=FS; \
        sub(/-rc[[:digit:]]+/,""); \
        $$1=""; \
        print "#define GIT_STRING \""$$(0)"\"" \
      }' \
      > git_string.h

.PHONY: git_string.h

all-am: git_string.h
