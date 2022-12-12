#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <parser.h>
#include <ast.h>
#include <gen_x64.h>
#include <symbol.h>
#include <lexer.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <context.h>
#include <unistd.h>


static const char* outputfile = NULL;
static cc_context cc_ctx;
char* g_input_buf = NULL;


static void help(char** argv) {
  printf("Usage: %s inputfiles\n", *argv);
  exit(0);
}

static uint8_t is_dir(const char* fname) {
  struct stat path;
  stat(fname, &path);
  return !(S_ISREG(path.st_mode));
}


static FILE* in_fp = NULL;


static void _on_exit(void) {
  if (in_fp) {
    fclose(in_fp);
  }

  if (g_input_buf) {
    free(g_input_buf);
  }

  destroy_ast_nodes();

  if (g_outfile) {
    fclose(g_outfile);
  }

  if (g_lex_id) {
    free((char*)g_lex_id);
  }

  destroy_symbols();
}


static void init_ctx(const char* current_fname) {
  cc_ctx.current_filename = current_fname;
  cc_ctx.current_line = 1;
}


static void compile(const char* path) {
  in_fp = fopen(path, "r");
  if (!(in_fp)) {
    perror("Failed to open file");
    exit(1);
  }

  if (is_dir(path)) {
    printf("Cannot open directory as file!\n");
    exit(1);
  }

  fseek(in_fp, 0, SEEK_END);
  size_t sz = ftell(in_fp);
  fseek(in_fp, 0, SEEK_SET);

  g_input_buf = calloc(sz + 1, sizeof(char));
  fread(g_input_buf, sizeof(char), sz, in_fp);
  
  /* Parse and pass a pointer to cc_ctx */
  init_ctx(path);
  parse(&cc_ctx);

  free(g_input_buf);
  g_input_buf = NULL;

  fclose(in_fp);
  in_fp = NULL;
}


int main(int argc, char** argv) {
  if (argc < 2) {
    help(argv);
  }

  atexit(_on_exit);

  int opt;
  while ((opt = getopt(argc, argv, "o:")) != -1) {
    switch (opt) {
      case 'o':
        outputfile = optarg;
        break;
    }
  }
  
  uint8_t has_infile = 0;
  for (uint32_t i = 1; i < argc; ++i) {
    if (*argv[i] != '-') {
      if (has_infile) {
        printf("Error: Compiling multiple files is not supported right now..\n");
        exit(1);
      }
      has_infile = 1;
      compile(argv[i]);
    }
  }

  return 0;
}
