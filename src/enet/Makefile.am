noinst_LIBRARIES = libmultiplayer.a
libmultiplayer_a_SOURCES = multiplayer.cpp

AM_CPPFLAGS = \
	-I$(top_srcdir)/include
AM_CXXFLAGS = $(GLOBAL_CFLAGS)
AM_CXXFLAGS += $(ENET_CFLAGS)

if SILENCE_7KAA_WARNINGS
AM_CXXFLAGS += -Wno-parentheses
AM_CXXFLAGS += -Wno-unused-variable
endif
