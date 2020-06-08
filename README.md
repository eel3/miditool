MIDI Tools
==========

Command line tools for implement/test/debug MIDI application.

License
-------

All commands are distributed under the [zlib License](./license/miditool).

RtMidi is distributed under the [MIT License](./license/rtmidi).

Target environments
-------------------

Windows, Linux, macOS.

Set up
------

1.  Build commands. Use make and Makefile (see the table below).
    * To build: `make all` or `nmake all`
    * To clean up: `make clean` or `nmake clean`
2.  Put all commands in a directory registered in PATH.

| Directory                      | Target environment | Toolchain                        |
|--------------------------------|--------------------|----------------------------------|
| [./build/linux](./build/linux) | Linux              | GCC + GNU Make                   |
| [./build/macos](./build/macos) | macOS              | Clang + GNU Make                 |
| [./build/mingw](./build/mingw) | Windows            | MinGW (GCC + GNU Make)           |
| [./build/msvc](./build/msvc)   | Windows            | Microsoft C/C++ Compiler + NMAKE |

Usage
-----

Please check help message `<command-name> -h`.

Example
-------

    > :: Example on Windows.
    >
    > :: Enumerate MIDI IN/OUT ports.
    > .\midiport.exe
    # MIDI IN
    0       Yamaha UX16-1 0
    
    # MIDI OUT
    0       Microsoft GS Wavetable Synth 0
    1       Yamaha UX16-1 1
    
    > :: Send MIDI bytes to a MIDI OUT port 1.
    > :: (1 MIDI message per line)
    > :: If you want to exit, send EOF (on Windows: Ctrl-z)
    > .\midisend.exe 1
    0x90 0x3C 0x64
    0x90 0x3C 0x00
    0x90 0x3C 0x64
    0x90 0x3C 0x00
    ^Z
    
    > :: Receive MIDI bytes from a MIDI IN port 0.
    > :: If you want to exit, send SIGINT (Ctrl-c)
    > .\midirecv.exe 0
    0x90 0x3C 0x64
    0x90 0x3C 0x00
    0x90 0x3C 0x64
    0x90 0x3C 0x00
    
    > _
