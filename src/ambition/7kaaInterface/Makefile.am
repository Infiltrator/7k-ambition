noinst_LIBRARIES = lib7kaa-ambition-interface.a
lib7kaa_ambition_interface_a_SOURCES = \
  building.cc \
  config.cc \
  control.cc \
  draw.cc \
  inn.cc \
  input.cc \
  polity.cc \
  serialisation.cc \
  unit.cc \
  # end

AM_CPPFLAGS = \
  -I$(top_srcdir)/include/ambition \
  -I$(top_srcdir)/include/ambition/7kaaInterface \
  # end

AM_CXXFLAGS = $(GLOBAL_CFLAGS)
AM_CXXFLAGS += $(AMBITION_CFLAGS)
AM_CXXFLAGS += $(SDL_CFLAGS)
