# -*- coding: utf-8-unix -*-
# vim:fileencoding=utf-8:ff=unix

# ----------------------------------------------------------

PREFIX         :=
CC             := $(PREFIX)gcc
CXX            := $(PREFIX)g++

CFLAGS          =
CXXFLAGS        =
CPPFLAGS        =
LDFLAGS         =
LDLIBS         := -lstdc++ -lwinmm

WARNADD        :=

# ----------------------------------------------------------

include ../unix-common/common.mk

# ----------------------------------------------------------

RtMidi.o:   CPPFLAGS   += -D__WINDOWS_MM__
