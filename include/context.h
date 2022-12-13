#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <stdint.h>
#include <stddef.h>
#include <symbol.h>

/* Warning types for cc_context.warns */
#define CC_WARN_UNREACHABLE (1 << 0)

typedef struct {
  size_t current_line;              /* Line number */
  size_t func_def_line;             /* Line of current function definition */
  size_t current_func_id;           /* Symbol ID of current function */
  const char* current_filename;     /* Filename of current file */
  uint8_t func_has_ret   : 1;       /* 1 if current function has return statement */
  uint8_t func_is_global : 1;       /* 1 if function is global */
  uint16_t warns;                   /* A bitmap of completed warnings if any */
  SYM_PTYPE func_ptype;             /* Current function's ptype */
} cc_context;

#endif
