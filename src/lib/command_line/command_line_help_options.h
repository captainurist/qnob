#pragma once

#include <cstddef> /* For size_t. */

struct CommandLineHelpOptions {
    /** Indentation before the option names. */
    size_t indent = 2;

    /** Max length of the option names field. */
    size_t maxHeaderLength = 28;

    /** Spacing between option names & option description. */
    size_t spacing = 2;

    /** Max line length, will wrap after this point. */
    size_t maxLineLength = 80;

    /** Whether default values are printed in description. */
    bool printDefaults = true;
};
