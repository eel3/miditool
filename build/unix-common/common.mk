# -*- coding: utf-8-unix -*-
# vim:fileencoding=utf-8:ff=unix

# ----------------------------------------------------------

root-dir       := ../..

lib-dir        := $(root-dir)/lib
rtmidi-dir     := $(lib-dir)/rtmidi

src-dir        := $(root-dir)/src

# ----------------------------------------------------------

VPATH          := $(rtmidi-dir) $(src-dir)

include-dirs   := $(addprefix -I ,$(VPATH) .)

target-name    := midiport midirecv midisend

lib-object-files   := RtMidi.o

object-files   := $(lib-object-files) $(addsuffix .o,$(target-name))

depend-files   := $(subst .o,.d,$(object-files))

# ----------------------------------------------------------

ifeq "$(strip $(DEBUG))" ""
  CPPFLAGS     += -DNDEBUG
  OPTIM        += -O2
else
  CPPFLAGS     += -DDEBUG
  OPTIM        += -g
endif

ifneq "$(strip $(PROFILE))" ""
  OPTIM        += --coverage
  cov-files     = $(subst .o,.gcno,$(object-files)) $(subst .o,.gcda,$(object-files)) $(wildcard *.gcov)
endif

OPTIM          +=

WARN_COMMON    ?= -pedantic -Wall -Wextra
WARN_C         ?=
WARN_CXX       ?=

CWARN          ?= -std=c89 $(WARN_COMMON) $(WARN_C)
CXXWARN        ?= -std=c++11 $(WARN_COMMON) $(WARN_CXX)

CFLAGS         += $(OPTIM) $(CWARN) $(WARNADD)
CXXFLAGS       += $(OPTIM) $(CXXWARN) $(WARNADD)
CPPFLAGS       += $(include-dirs)
LDFLAGS        += $(OPTIM)

# ----------------------------------------------------------

phony-targets  := all clean usage

.PHONY: $(phony-targets)

usage:
	# usage: $(MAKE) [DEBUG=1] [PROFILE=1] $(patsubst %,[%],$(phony-targets))

all: $(target-name)

.SECONDEXPANSION:
$(target-name): $$@.o $(lib-object-files)

midiport.o: WARN_COMMON    += -Wno-unused-function

clean:
	$(RM) $(target-name) $(object-files) $(depend-files) $(cov-files)

# ----------------------------------------------------------

ifneq "$(MAKECMDGOALS)" ""
ifneq "$(MAKECMDGOALS)" "clean"
ifneq "$(MAKECMDGOALS)" "usage"
  -include $(depend-files)
endif
endif
endif

# $(call make-depend,source-file,object-file,depend-file,cc,flags)
make-depend = $4 -MM -MF $3 -MP -MT $2 $5 $(CPPFLAGS) $1

%.o: %.c
	$(call make-depend,$<,$@,$(subst .o,.d,$@),$(CC),$(CFLAGS))
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.o: %.cpp
	$(call make-depend,$<,$@,$(subst .o,.d,$@),$(CXX),$(CXXFLAGS))
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
