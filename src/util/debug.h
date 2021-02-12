#pragma once

[[noreturn]] void VerifyFailed(const char* file, int line, const char* message);

#define QB_VERIFY(x) if(!x) VerifyFailed(__FILE__, __LINE__, #x);

