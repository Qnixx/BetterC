#include <diag.h>
#include <stdio.h>


static void diag_common(cc_context* cc_ctx, const char* fmt, va_list args) {
  vprintf(fmt, args);
  printf("%s: line %d\n", cc_ctx->current_filename, cc_ctx->current_line);
}


void cc_diag_err(cc_context* cc_ctx, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  printf("\e[0;31merror: ");
  diag_common(cc_ctx, fmt, ap);
  va_end(ap);
}


void cc_diag_warning(cc_context* cc_ctx, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  printf("\e[0;35mwarning: ");
  diag_common(cc_ctx, fmt, ap);
  printf("\n");
  va_end(ap);
}


void cc_diag_note(cc_context* cc_ctx, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  printf("\e[0;36mnote: ");
  diag_common(cc_ctx, fmt, ap);
  va_end(ap);
}

void cc_diag_showfunc(cc_context* cc_ctx) {
  printf("\e[0;36mnote: function defined here: line %d\n", cc_ctx->func_def_line);
}
