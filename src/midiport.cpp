// -*- coding: utf-8-with-signature-dos -*-
// vim:fileencoding=utf-8:ff=dos
//
// Enumerate MIDI IN/OUT ports.

// C++ standard library
#include <cstdlib>
#include <iostream>
#include <stdexcept>

// C++ third party library
#include "RtMidi.h"

// C++ user library
#include "midiutil.hpp"

namespace {

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

inline void usage(std::ostream &out)
{
    out << "usage: " << program_name << " [-hv]" << std::endl;
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

void enum_port_name(RtMidi &port, std::ostream &out)
{
    const auto nport = port.getPortCount();

    for (unsigned int i = 0; i < nport; i++) {
        out << i << "\t" << port.getPortName(i) << std::endl;
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
    using std::endl;

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

    try {
        cout << "# MIDI IN" << endl;
        RtMidiIn midi_in;
        enum_port_name(midi_in, cout);

        cout << endl;

        cout << "# MIDI OUT" << endl;
        RtMidiOut midi_out;
        enum_port_name(midi_out, cout);
        return EXIT_SUCCESS;
    } catch(const std::exception& e) {
        error(e.what());
        return EXIT_FAILURE;
    }
}
