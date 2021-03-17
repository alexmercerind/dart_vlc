ACLOCAL_AMFLAGS = -I m4

MEDIALIB_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/include -I$(top_srcdir)/src

libvlcppdir = $(includedir)/vlcpp

libvlcpp_HEADERS =          \
	vlcpp/common.hpp              \
	vlcpp/Equalizer.hpp           \
	vlcpp/EventManager.hpp        \
	vlcpp/Instance.hpp            \
	vlcpp/Internal.hpp            \
	vlcpp/MediaDiscoverer.hpp     \
	vlcpp/Media.hpp               \
	vlcpp/MediaLibrary.hpp        \
	vlcpp/MediaList.hpp           \
	vlcpp/MediaListPlayer.hpp     \
	vlcpp/MediaPlayer.hpp         \
	vlcpp/Dialog.hpp			  \
	vlcpp/RendererDiscoverer.hpp  \
	vlcpp/Picture.hpp			  \
	vlcpp/structures.hpp          \
	vlcpp/vlc.hpp                 \
	$(NULL)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libvlcpp.pc

if HAVE_EXAMPLES
noinst_PROGRAMS = helloworld tests imem discovery

AM_CPPFLAGS = $(vlc_CFLAGS) -Wextra -Wall

if HAVE_WERROR
# This is meant to make our builds fail if a deprecated method is present
AM_CPPFLAGS += -Werror
endif

helloworld_SOURCES = examples/helloworld/main.cpp
helloworld_LDADD = $(vlc_LIBS)
imem_SOURCES = examples/imem/imem.cpp
imem_LDADD = $(vlc_LIBS)
tests_SOURCES = test/main.cpp
tests_LDADD = $(vlc_LIBS)
discovery_SOURCES = examples/renderers/discovery.cpp
discovery_LDADD = $(vlc_LIBS)

endif
