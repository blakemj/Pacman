#include "pacman_printf.h"
#include "gl.h"
#include "printf.h"
#include "font.h"
#include <stdarg.h>
#include "strings.h"

//This is the max length of the buffer
#define INIT_BUFFER_LENGTH 1024

//This creates a buffer that is saved with global variables
static char buf[INIT_BUFFER_LENGTH];

/*
* This function will use the vsnprintf function written in the libmypi.a to save the string
* into a buffer. This buffer will then be returned.
*/
char* pacman_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(buf, INIT_BUFFER_LENGTH, format, args);    
    va_end(args);
    return buf;
}
