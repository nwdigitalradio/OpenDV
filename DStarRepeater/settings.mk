# Makefile-include
#
# Settings for Raspbian Wheezy compile in qemu-debootstrap environment
# Hans-J. Barthen, DL5DI 2013-01-21
# Rename this file to settings.mk before you compile the source for the Raspbian platform
#
	export DATADIR := "/home/opendv/data"
	export BINDIR  := "$(DESTDIR)/usr/bin"
	export LOGDIR  := "/var/log/opendv"
	export CONFDIR := "/home/opendv/dstarrepeater"
	export CC      := $(shell wx-config --cxx)
	export LDFLAGS := 
	export CFLAGS  := -O2 -march=armv7-a -mfpu=vfp -mfloat-abi=hard -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DLOG_DIR='$(LOGDIR)' -DCONF_DIR='$(CONFDIR)' -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -DGPIO -pthread $(shell wx-config --cxxflags)
	export LIBS    := -lasound -lusb-1.0 -lwiringPi $(shell wx-config --libs adv,core)
