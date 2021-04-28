#pragma once

/* Copied from ucrt implementation. */

#define _NO_CONSOLE_FILENO ((intptr_t)-2)

struct __crt_stdio_stream_data {
    union {
        /* FILE  _public_file; */
        char* _ptr;
    };

    char* _base;
    int              _cnt;
    long             _flags;
    long             _file;
    int              _charbuf;
    int              _bufsiz;
    char* _tmpfname;
    /* CRITICAL_SECTION _lock; */
};

__crt_stdio_stream_data* crtData(FILE* file) {
    return reinterpret_cast<__crt_stdio_stream_data*>(file);
}
