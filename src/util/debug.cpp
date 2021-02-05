#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

void VerifyFailed(const char* file, int line, const char* message) {
    printf("%s(%d): VERIFY(%s) failed.", file, line, message);
    exit(1);
}
