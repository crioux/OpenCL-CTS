#ifndef __INC_DLLIO_HOOKS_H
#define __INC_DLLIO_HOOKS_H

#include<iostream>

struct DLLIO_HOOKS {
    void *ctx;
    void (*stdout_hook)(void *ctx, const char *str);
    void (*stderr_hook)(void *ctx, const char *str);
    // std::istream *cin_stream;
    std::ostream *cout_stream;
    std::ostream *cerr_stream;
};

#endif