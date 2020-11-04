#pragma once

[[noreturn]] void VerifyFailed(const char* file, int line, const char* message);

#define VERIFY(x) if(!x) VerifyFailed(__FILE__, __LINE__, #x);

