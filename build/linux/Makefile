# -*- coding: utf-8-unix -*-
# vim:fileencoding=utf-8:ff=unix

# ----------------------------------------------------------

PREFIX         :=
CC             := $(PREFIX)$(CC)
CXX            := $(PREFIX)$(CXX)

CFLAGS          = -pthread
CXXFLAGS        = -pthread
CPPFLAGS        =
LDFLAGS         = -pthread
LDLIBS         := -lasound -lstdc++

WARNADD        :=

# ----------------------------------------------------------

include ../unix-common/common.mk

# ----------------------------------------------------------

RtMidi.o:   CPPFLAGS   += -D__LINUX_ALSA__
