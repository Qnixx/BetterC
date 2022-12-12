#ifndef DIAG_H_
#define DIAG_H_

#include <stdint.h>
#include <stdarg.h>
#include <context.h>


void cc_diag_err(cc_context* cc_ctx, const char* fmt, ...);
void cc_diag_warning(cc_context* cc_ctx, const char* fmt, ...);
void cc_diag_note(cc_context* cc_ctx, const char* fmt, ...);
void cc_diag_showfunc(cc_context* cc_ctx);                          /* Shows line where current function is defined */


#endif
