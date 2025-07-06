SUBDIRS = 7kaaInterface

noinst_LIBRARIES = libambition.a
libambition_a_SOURCES = \
	Ambition_building.cc \
	Ambition_config.cc \
	Ambition_control.cc \
	Ambition_coordinates.cc \
	Ambition_entity.cc \
	Ambition_inn.cc \
	Ambition_input.cc \
	Ambition_minimap.cc \
	Ambition_news.cc \
	Ambition_polity.cc \
	Ambition_remote.cc \
	Ambition_repository.cc \
	Ambition_serialisation.cc \
	Ambition_spy.cc \
	Ambition_time.cc \
	Ambition_trade.cc \
	Ambition_unit.cc \
	Ambition_user_interface.cc \
	Ambition_version.cc \
	Ambition_vga.cc \
	version_details.cc \
	# end

AM_CPPFLAGS = \
	-I$(top_srcdir)/include/ambition \
	# end

AM_CXXFLAGS = $(GLOBAL_CFLAGS)
AM_CXXFLAGS += $(AMBITION_CFLAGS)
AM_CXXFLAGS += $(SDL_CFLAGS)
AM_CXXFLAGS += $(BOOST_CPPFLAGS)

Ambition_version.cc: git_string.h

git_string.h:
	@if [ -d "$(top_srcdir)/.git" ]; then \
		echo "Using git to generate the version string."; \
		git describe --always --dirty --long --tags \
		| awk --field-separator="-" \
			'{OFS=FS; \
			sub(/-rc[[:digit:]]+/,""); \
			$$1=""; \
			print "#define GIT_STRING \""$$(0)"\"" \
			}' \
			> git_string.h; \
		echo "Generated git_string.h with content:"; cat git_string.h; \
	elif [ -e "git_string.h" ]; then \
		echo "Using provided git_string.h with content:"; cat git_string.h; \
	else \
		echo "No git_string.h provided and not inside a git repository to be able" \
			"to generate one."; \
	fi

.PHONY: git_string.h

all-am: git_string.h
