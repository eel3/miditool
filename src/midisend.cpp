// -*- coding: utf-8-with-signature-dos -*-
// vim:fileencoding=utf-8:ff=dos
//
// Convert from texts to bytes, then send bytes to a MIDI OUT port.

// C++ standard library
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// C++ third party library
#include "RtMidi.h"

// C++ user library
#include "midiutil.hpp"

namespace {

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

enum MessageType : unsigned char {
    // Channel message:
    NOTE_OFF = 0x80,
    NOTE_ON = 0x90,
    POLY_AFTERTOUCH = 0xA0,
    CONTROL_CHANGE = 0xB0,
    PROGRAM_CHANGE = 0xC0,
    CHANNEL_AFTERTOUCH = 0xD0,
    PITCH_WHEEL = 0xE0,

    // System message:
    SYSEX = 0xF0,
    MTC_QUARTER_FRAME = 0xF1,
    SONG_POSITION = 0xF2,
    SONG_SELECT = 0xF3,
    UNDEFINED_1 = 0xF4,
    UNDEFINED_2 = 0xF5,
    TUNE_REQUEST = 0xF6,
    EOX = 0xF7,
    TIMING_CLOCK = 0xF8,
    UNDEFINED_3 = 0xF9,
    START = 0xFA,
    CONTINUE = 0xFB,
    STOP = 0xFC,
    UNDEFINED_4 = 0xFD,
    ACTIVE_SENSE = 0xFE,
    SYSTEM_RESET = 0xFF
};

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

inline void usage(std::ostream &out)
{
    out << "usage: " << program_name << " [-hv] <port-number>" << std::endl;
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

inline void version()
{
    std::cout << program_name << " 1.0.1" << std::endl;
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

inline bool is_status_byte(const unsigned char byte)
{
    return (byte & static_cast<unsigned char>(0x80)) != 0;
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

inline bool is_data_byte(const unsigned char byte)
{
    return !is_status_byte(byte);
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

inline std::size_t guess_message_size(const unsigned char status_byte)
{
    assert(is_status_byte(status_byte));

    switch (status_byte & static_cast<unsigned char>(0xF0)) {
    case NOTE_OFF:
    case NOTE_ON:
    case POLY_AFTERTOUCH:
    case CONTROL_CHANGE:
    case PITCH_WHEEL:
        return 3;
    case PROGRAM_CHANGE:
    case CHANNEL_AFTERTOUCH:
        return 2;
    default:
        switch (status_byte) {
        case SYSEX:
            return 0;
        case MTC_QUARTER_FRAME:
        case SONG_SELECT:
            return 2;
        case SONG_POSITION:
            return 3;
        case UNDEFINED_1:
        case UNDEFINED_2:
        case UNDEFINED_3:
        case UNDEFINED_4:
            return 1;
        default:
            return 1;
        }
    }
    /*NOTREACHED*/
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

bool is_sysex(const std::vector<unsigned char> &bytes)
{
    if (bytes.size() < 2) {
        return false;
    } else if (bytes.front() != SYSEX) {
        return false;
    } else if (bytes.back() != EOX) {
        return false;
    } else if (std::any_of(std::next(bytes.cbegin()), std::prev(bytes.cend()), is_status_byte)) {
        return false;
    } else {
        return true;
    }
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

bool is_midi_message(const std::vector<unsigned char> &bytes)
{
    if (bytes.empty()){
        return false;
    } else if (is_data_byte(bytes.front())) {
        return false;
    } else if (bytes.front() == SYSEX) {
        return is_sysex(bytes);
    } else if (bytes.size() != guess_message_size(bytes.front())) {
        return false;
    } else if (std::any_of(std::next(bytes.cbegin()), bytes.cend(), is_status_byte)) {
        return false;
    } else {
        return true;
    }
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

void do_midisend(RtMidiOut& port, std::istream &in)
{
    using std::string;

    while (!in.eof()) {
        string line;
        std::getline(in, line);
        std::istringstream iss { line };
        std::vector<unsigned char> msg;
        auto have_invalid_byte = false;

        while (!iss.eof()) {
            string word;
            iss >> word;
            if (word.empty()) {
                continue;
            }
            try {
                msg.push_back(byte_from_string(word));
            } catch (...) {
                have_invalid_byte = true;
                break;
            }
        }

        if (have_invalid_byte) {
            error("Invalid MIDI message: " + line);
            continue;
        }
        if (msg.empty()) {
            continue;
        }
        if (is_midi_message(msg)) {
            port.sendMessage(&msg);
        } else {
            error("Invalid MIDI message: " + line);
        }
    }
}

} // namespace

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
    using std::cerr;
    using std::cout;

    program_name = my_basename(argv[0]);

    for (; (argc > 1) && (argv[1][0] == '-') && (argv[1][1] != '\0'); argc--, argv++) {
        if (argv[1][1] == '-') {
            const char *p { &argv[1][2] };

            if (*p == '\0') {
                argc--, argv++;
                break;
            } else if (STREQ(p, "help")) {
                usage(cout);
                return EXIT_SUCCESS;
            } else if (STREQ(p, "version")) {
                version();
                return EXIT_SUCCESS;
            } else {
                usage(cerr);
                return EXIT_FAILURE;
            }
            continue;
        }

        const char *p { &argv[1][1] };

        do switch (*p) {
        case 'h':
            usage(cout);
            return EXIT_SUCCESS;
        case 'v':
            version();
            return EXIT_SUCCESS;
        default:
            usage(cerr);
            return EXIT_FAILURE;
        } while (*++p != '\0');
    }

    if (argc != 2) {
        usage(cerr);
        return EXIT_FAILURE;
    }

    unsigned int pn { 0 };
    try {
        pn = port_number_from_string(argv[1]);
    } catch (...) {
        error("Invalid port number: " + std::string(argv[1]));
        return EXIT_FAILURE;
    }

    try {
        RtMidiOut port;
        port.setErrorCallback(default_error_callback);
        port.openPort(pn);
        do_midisend(port, std::cin);
        port.closePort();
        return EXIT_SUCCESS;
    } catch(const std::exception& e) {
        error(e.what());
        return EXIT_FAILURE;
    }
}
