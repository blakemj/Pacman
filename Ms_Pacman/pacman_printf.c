#include "pacman_printf.h"
#include "gl.h"
#include "printf.h"
#include "font.h"
#include <stdarg.h>
#include "strings.h"

//
#define INIT_BUFFER_LENGTH 1024

//
static char buf[INIT_BUFFER_LENGTH];

/*
*
*/
char* pacman_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(buf, INIT_BUFFER_LENGTH, format, args);    
    va_end(args);
    return buf;
}
