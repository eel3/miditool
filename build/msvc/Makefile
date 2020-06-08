# -*- coding: cp932-dos -*-
# vim:fileencoding=cp932:ff=dos

# ----------------------------------------------------------

root_dir        = ..\..

lib_dir         = $(root_dir)\lib
rtmidi_dir      = $(lib_dir)\rtmidi

src_dir         = $(root_dir)\src

# ----------------------------------------------------------

vpath           = $(rtmidi_dir) $(src_dir)

include_dirs    = $(vpath)

target_name     = midiport.exe midirecv.exe midisend.exe

lib_object_files    = RtMidi.obj

object_files    = $(lib_object_files) $(target_name:.exe=.obj)

lib_files       = winmm.lib

win_ver         = 0x0A00

# ----------------------------------------------------------

!ifdef DEBUG
ccdefs      = /MTd /Zi /D WIN32;_DEBUG;_CONSOLE;WINVER=$(win_ver);_WIN32_WINNT=$(win_ver);_CRT_SECURE_NO_WARNINGS;_WINDOWS;DEBUG;__WINDOWS_MM__
!else
ccdefs      = /MT /D WIN32;NDEBUG;_CONSOLE;WINVER=$(win_ver);_WIN32_WINNT=$(win_ver);_CRT_SECURE_NO_WARNINGS;_WINDOWS;__WINDOWS_MM__
!endif

CFLAGS      = /nologo /GL /GS /RTCs /RTCu /W4 $(ccdefs:;= /D ) /I $(include_dirs: = /I )
CXXFLAGS    = /nologo /EHsc /GL /GS /RTCs /RTCu /W4 $(ccdefs:;= /D ) /I $(include_dirs: = /I )

# ----------------------------------------------------------

phony_targets   = all clean usage

usage: FORCE
	:: usage: $(MAKE) [DEBUG=1] [$(phony_targets: =] [)]

all: $(target_name)

$(target_name): $$(@B).obj $(lib_object_files)
	link.exe /LTCG /OUT:$@ /SUBSYSTEM:CONSOLE $** $(lib_files)

clean: FORCE
	del /F $(target_name) $(object_files) *.pdb

FORCE:

# ----------------------------------------------------------

{$(rtmidi_dir)}.c.obj::
	$(CC) $(CFLAGS) /c $<
{$(rtmidi_dir)}.cpp.obj::
	$(CXX) $(CXXFLAGS) /c $<

{$(src_dir)}.c.obj::
	$(CC) $(CFLAGS) /c $<
{$(src_dir)}.cpp.obj::
	$(CXX) $(CXXFLAGS) /c $<
