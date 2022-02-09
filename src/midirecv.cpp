// -*- coding: utf-8-with-signature-dos -*-
// vim:fileencoding=utf-8:ff=dos
//
// Receive MIDI bytes from a MIDI IN port, and show them.

// C++ standard library
#include <cassert>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

// C++ third party library
#include "RtMidi.h"

// C++ user library
#include "midiutil.hpp"

namespace {

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

constexpr sig_atomic_t ATOMIC_TRUE  { static_cast<sig_atomic_t>(1) };
constexpr sig_atomic_t ATOMIC_FALSE { static_cast<sig_atomic_t>(0) };

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

volatile sig_atomic_t want_to_exit { ATOMIC_FALSE };

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

inline void trap_exit(int signum) noexcept
{
    assert((signum == SIGINT) || (signum == SIGTERM));
    (void) signum;

    want_to_exit = ATOMIC_TRUE;
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

inline void set_signal_handler() noexcept
{
    (void) signal(SIGINT,  trap_exit);
    (void) signal(SIGTERM, trap_exit);
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

void receive_callback(double, std::vector<unsigned char> *message, void *)
{
    using std::cout;

    if (message == nullptr) {
        return;
    }

    auto prefix = "";

    cout
        << std::hex
        << std::uppercase
        << std::setfill('0');

    for (auto byte: *message) {
        cout
            << prefix
            << "0x"
            << std::setw(2)
            << static_cast<unsigned int>(byte);
        prefix = " ";
    }
    cout << std::endl;
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

void wait()
{
    using std::chrono::milliseconds;
    using std::this_thread::sleep_for;

    set_signal_handler();

    for (milliseconds timeout { 50 }; !want_to_exit; sleep_for(timeout)) {
        /*EMPTY*/
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
        RtMidiIn port;
        port.setErrorCallback(default_error_callback);
        port.setCallback(receive_callback);
        port.openPort(pn);
        wait();
        port.closePort();
        port.cancelCallback();
        return EXIT_SUCCESS;
    } catch(const std::exception& e) {
        error(e.what());
        return EXIT_FAILURE;
    }
}
