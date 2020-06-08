# -*- coding: utf-8-unix -*-
# vim:fileencoding=utf-8:ff=unix

# ----------------------------------------------------------

PREFIX         := xcrun 
CC             := $(PREFIX)$(CC)
CXX            := $(PREFIX)$(CXX)

CFLAGS          =
CXXFLAGS        =
CPPFLAGS        =
LDFLAGS         =
LDLIBS         := -lc++ -framework CoreAudio -framework CoreFoundation -framework CoreMIDI

WARNADD        :=

# ----------------------------------------------------------

SDKROOT        := $(shell xcodebuild -version -sdk macosx | sed -n '/^Path: /s///p')

CPPFLAGS       += -isysroot "$(SDKROOT)"
TARGET_ARCH    := -mmacosx-version-min=10.13 -arch x86_64   # FIXME

# ----------------------------------------------------------

include ../unix-common/common.mk

# ----------------------------------------------------------

RtMidi.o:   CPPFLAGS   += -D__MACOSX_CORE__
