// -*- coding: utf-8-with-signature-dos -*-
// vim:fileencoding=utf-8:ff=dos
//
// Utility functions for command implementation.

#ifndef MIDI_UTIL_HPP_INCLUDED
#define MIDI_UTIL_HPP_INCLUDED

// C++ standard library
#include <climits>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

// C++ third party library
#include "RtMidi.h"

namespace {

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

std::string program_name;

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

#define STREQ(s1, s2) (((s1)[0] == (s2)[0]) && (std::strcmp((s1), (s2)) == 0))

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

std::string trim_right(const std::string &s, const std::string &chars = "\t\n\v\f\r ")
{
    using std::string;

    auto rpos = s.find_last_not_of(chars);

    return (rpos == string::npos) ? string() : s.substr(0, rpos + 1);
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

std::string my_basename(const char * const s)
{
    using std::string;

    static const string DOT { "." };
    static const string SEP { "/" };
#if defined(_WIN32) || defined(_WIN64)
    static const string WSEP { "\\" };
    static const string SEPS { SEP + WSEP };
#else /* defined(_WIN32) || defined(_WIN64) */
    static const string SEPS { SEP };
#endif /* defined(_WIN32) || defined(_WIN64) */

    if (s == nullptr) {
        return DOT;
    }

    string path { s };

    if (path.empty()) {
        return DOT;
    }
    if (path == SEP) {
        return SEP;
    }
#if defined(_WIN32) || defined(_WIN64)
    if (path == WSEP) {
        return WSEP;
    }
#endif /* defined(_WIN32) || defined(_WIN64) */

    path = trim_right(path, SEPS);

    auto pos = path.find_last_of(SEPS);
    return (pos == string::npos) ? path : path.substr(pos + 1);
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

inline void error(const std::string &s)
{
    std::cerr << program_name << ": " << s << std::endl;
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

long stol_strictly(const std::string &s, const int base = 10)
{
    using std::invalid_argument;

    long n;
    std::size_t idx { 0 };

    try {
        n = std::stol(s, &idx, base);
    } catch (...) {
        throw invalid_argument(s);
    }

    if (idx != s.size()) {
        throw invalid_argument(s);
    }

    return n;
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

unsigned int port_number_from_string(const std::string &s)
{
    auto n = stol_strictly(s, 0);
    if ((n < 0) || (n > INT_MAX)) {
        throw std::invalid_argument(s);
    }

    return static_cast<unsigned int>(n);
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

unsigned char byte_from_string(const std::string &s)
{
    auto n = stol_strictly(s, 0);
    if ((n < 0) || (n > UCHAR_MAX)) {
        throw std::invalid_argument(s);
    }

    return static_cast<unsigned char>(n);
}

/* ---------------------------------------------------------------------- */
/*  */
/* ---------------------------------------------------------------------- */

void default_error_callback(RtMidiError::Type type, const std::string &errorText, void *)
{
    switch (type) {
    case RtMidiError::WARNING:
    case RtMidiError::DEBUG_WARNING:
        break;
    default:
        throw RtMidiError(errorText, type);
    }
}

} // namespace

#endif // ndef MIDI_UTIL_HPP_INCLUDED
