#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <stdint.h>
#include <stddef.h>

typedef struct {
  size_t current_line;              /* Line number */
  const char* current_filename;     /* Filename of current file */
} cc_context;

#endif
