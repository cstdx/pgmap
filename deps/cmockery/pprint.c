/*
  Copyright (C) 2014 Yusuke Suzuki <utatane.tea@gmail.com>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#define _POSIX_SOURCE

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static int Write(FILE *stream, const char *format, va_list ap) {
#if defined(_WIN32) && !defined(__MINGW32__)
    return vfprintf_s(stream, format, ap);
#else
    return vfprintf(stream, format, ap);
#endif
}

#define CC_COLOR_BITS 5

typedef enum {
    CC_FG_NONE         = 0  << 0,
    CC_FG_BLACK        = 1  << 0,
    CC_FG_DARK_RED     = 2  << 0,
    CC_FG_DARK_GREEN   = 3  << 0,
    CC_FG_DARK_YELLOW  = 4  << 0,
    CC_FG_DARK_BLUE    = 5  << 0,
    CC_FG_DARK_MAGENTA = 6  << 0,
    CC_FG_DARK_CYAN    = 7  << 0,
    CC_FG_GRAY         = 8  << 0,
    CC_FG_DARK_GRAY    = 9  << 0,
    CC_FG_RED          = 10 << 0,
    CC_FG_GREEN        = 11 << 0,
    CC_FG_YELLOW       = 12 << 0,
    CC_FG_BLUE         = 13 << 0,
    CC_FG_MAGENTA      = 14 << 0,
    CC_FG_CYAN         = 15 << 0,
    CC_FG_WHITE        = 16 << 0,

    CC_BG_NONE         = 0  << CC_COLOR_BITS,
    CC_BG_BLACK        = 1  << CC_COLOR_BITS,
    CC_BG_DARK_RED     = 2  << CC_COLOR_BITS,
    CC_BG_DARK_GREEN   = 3  << CC_COLOR_BITS,
    CC_BG_DARK_YELLOW  = 4  << CC_COLOR_BITS,
    CC_BG_DARK_BLUE    = 5  << CC_COLOR_BITS,
    CC_BG_DARK_MAGENTA = 6  << CC_COLOR_BITS,
    CC_BG_DARK_CYAN    = 7  << CC_COLOR_BITS,
    CC_BG_GRAY         = 8  << CC_COLOR_BITS,
    CC_BG_DARK_GRAY    = 9  << CC_COLOR_BITS,
    CC_BG_RED          = 10 << CC_COLOR_BITS,
    CC_BG_GREEN        = 11 << CC_COLOR_BITS,
    CC_BG_YELLOW       = 12 << CC_COLOR_BITS,
    CC_BG_BLUE         = 13 << CC_COLOR_BITS,
    CC_BG_MAGENTA      = 14 << CC_COLOR_BITS,
    CC_BG_CYAN         = 15 << CC_COLOR_BITS,
    CC_BG_WHITE        = 16 << CC_COLOR_BITS
} cc_color_t;

#ifndef COMMON_LVB_LEADING_BYTE
#define COMMON_LVB_LEADING_BYTE    0x0100
#endif

#ifndef COMMON_LVB_TRAILING_BYTE
#define COMMON_LVB_TRAILING_BYTE   0x0200
#endif

#ifndef COMMON_LVB_GRID_HORIZONTAL
#define COMMON_LVB_GRID_HORIZONTAL 0x0400
#endif

#ifndef COMMON_LVB_GRID_LVERTICAL
#define COMMON_LVB_GRID_LVERTICAL  0x0800
#endif

#ifndef COMMON_LVB_GRID_RVERTICAL
#define COMMON_LVB_GRID_RVERTICAL  0x1000
#endif

#ifndef COMMON_LVB_REVERSE_VIDEO
#define COMMON_LVB_REVERSE_VIDEO   0x4000
#endif

#ifndef COMMON_LVB_UNDERSCORE
#define COMMON_LVB_UNDERSCORE      0x8000
#endif

#ifdef _WIN32

#define PPRINT(name, color, stream) int name(const char* format, ...) {       \
    unsigned int fg;                                                          \
    unsigned int bg;                                                          \
    int result = -EINVAL;                                                     \
    va_list ap;                                                               \
    HANDLE console;                                                           \
    va_start(ap, format);                                                     \
    if (!isatty(fileno((stream))) || ((stream) != stdout && (stream) != stderr)) {\
        result = Write((stream), format, ap);                                 \
        goto finish;                                                          \
    }                                                                         \
    fg = (color) & ((1 << CC_COLOR_BITS) - 1);                                \
    bg = (color) >> CC_COLOR_BITS & ((1 << CC_COLOR_BITS) - 1);               \
    console = GetStdHandle(                                                   \
            (stream) == stdout ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);       \
    if (console == INVALID_HANDLE_VALUE) {                                    \
        result = Write((stream), format, ap);                                 \
        goto finish;                                                          \
    }                                                                         \
    CONSOLE_SCREEN_BUFFER_INFO csbi;                                          \
    if (!GetConsoleScreenBufferInfo(console, &csbi)) {                        \
        result = Write((stream), format, ap);                                 \
        goto finish;                                                          \
    }                                                                         \
    SetConsoleTextAttribute(console, Generate(fg, bg, csbi.wAttributes));     \
    result = Write((stream), format, ap);                                     \
    SetConsoleTextAttribute(console, csbi.wAttributes);                       \
finish:                                                                       \
    va_end(ap);                                                               \
    return result;                                                            \
}
#else
#define PPRINT(name, color, stream) int name(const char* format, ...) {       \
    unsigned int fg;                                                          \
    unsigned int bg;                                                          \
    int result = -EINVAL;                                                     \
    va_list ap;                                                               \
    va_start(ap, format);                                                     \
    if (!isatty(fileno((stream))) || ((stream) != stdout && (stream) != stderr)) {\
        result = Write((stream), format, ap);                                 \
        goto finish;                                                          \
    }                                                                         \
    fg = (color) & ((1 << CC_COLOR_BITS) - 1);                                \
    bg = (color) >> CC_COLOR_BITS & ((1 << CC_COLOR_BITS) - 1);               \
    UnixTerminalColorize((stream), fg, bg);                                   \
    Write((stream), format, ap);                                              \
    UnixTerminalRestore((stream));                                            \
finish:                                                                       \
    va_end(ap);                                                               \
    return result;                                                            \
}

#endif

#ifdef _WIN32
static const WORD FG[] = {
/* NONE         */  0,
/* BLACK        */  0,
/* DARK_RED     */  FOREGROUND_RED,
/* DARK_GREEN   */  FOREGROUND_GREEN,
/* DARK_YELLOW  */  FOREGROUND_RED | FOREGROUND_GREEN,
/* DARK_BLUE    */  FOREGROUND_BLUE,
/* DARK_MAGENTA */  FOREGROUND_RED | FOREGROUND_BLUE,
/* DARK_CYAN    */  FOREGROUND_GREEN | FOREGROUND_BLUE,
/* GRAY         */  FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED,
/* DARK_GRAY    */  FOREGROUND_INTENSITY,
/* RED          */  FOREGROUND_INTENSITY | FOREGROUND_RED,
/* GREEN        */  FOREGROUND_INTENSITY | FOREGROUND_GREEN,
/* YELLOW       */  FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
/* BLUE         */  FOREGROUND_INTENSITY | FOREGROUND_BLUE,
/* MAGENTA      */  FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
/* CYAN         */  FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
/* WHITE        */  FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED,
};

static const WORD BG[] = {
/* NONE         */  0,
/* BLACK        */  0,
/* DARK_RED     */  BACKGROUND_RED,
/* DARK_GREEN   */  BACKGROUND_GREEN,
/* DARK_YELLOW  */  BACKGROUND_RED | BACKGROUND_GREEN,
/* DARK_BLUE    */  BACKGROUND_BLUE,
/* DARK_MAGENTA */  BACKGROUND_RED | BACKGROUND_BLUE,
/* DARK_CYAN    */  BACKGROUND_GREEN | BACKGROUND_BLUE,
/* GRAY         */  BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED,
/* DARK_GRAY    */  BACKGROUND_INTENSITY,
/* RED          */  BACKGROUND_INTENSITY | BACKGROUND_RED,
/* GREEN        */  BACKGROUND_INTENSITY | BACKGROUND_GREEN,
/* YELLOW       */  BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
/* BLUE         */  BACKGROUND_INTENSITY | BACKGROUND_BLUE,
/* MAGENTA      */  BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
/* CYAN         */  BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
/* WHITE        */  BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED,
};

static WORD ExtractForeground(WORD attributes) {
    const WORD mask =
        FOREGROUND_INTENSITY |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_RED;
    return attributes & mask;
}

static WORD ExtractBackground(WORD attributes) {
    const WORD mask =
        BACKGROUND_INTENSITY |
        BACKGROUND_GREEN |
        BACKGROUND_BLUE |
        BACKGROUND_RED;
    return attributes & mask;
}

static WORD ExtractOthers(WORD attributes) {
    const WORD mask =
        COMMON_LVB_LEADING_BYTE |
        COMMON_LVB_TRAILING_BYTE |
        COMMON_LVB_GRID_HORIZONTAL |
        COMMON_LVB_GRID_LVERTICAL |
        COMMON_LVB_GRID_RVERTICAL |
        COMMON_LVB_REVERSE_VIDEO |
        COMMON_LVB_UNDERSCORE;
    return attributes & mask;
}

static WORD Generate(unsigned int fg, unsigned int bg, WORD attributes) {
    WORD result = ExtractOthers(attributes);
    result |= ((fg == 0) ? ExtractForeground(attributes) : FG[fg]);
    result |= ((bg == 0) ? ExtractBackground(attributes) : BG[bg]);
    return result;
}

#else

static inline unsigned int Shift(
        unsigned int val, unsigned int normal, unsigned int bright) {
    if (val == 0) {
        return 9 + normal;
    }
    val -= 1;
    /* background */
    if (val >= 8) {
        return (val - 8) + bright;
    }
    return val + normal;
}

static void UnixTerminalColorize(
        FILE* stream, unsigned int fg, unsigned int bg) {
    fprintf(stream, "\x1B[39;49;%u;%um", Shift(fg, 30, 90), Shift(bg, 40, 100));
}

static void UnixTerminalRestore(FILE* stream) {
    fputs("\x1B[39;49m\x1B[K", stream);
}
#endif  /* _WIN32 */

PPRINT(print_message, CC_FG_DARK_GREEN, stdout)
PPRINT(print_error, CC_FG_DARK_RED, stderr)