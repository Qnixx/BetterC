#include <ast.h>
#include <stdlib.h>


static astnode_t** allocd_nodes = NULL;
static size_t allocd_nodes_idx = 0;


astnode_t* mkastnode(ast_nodetype_t op, astnode_t* left, astnode_t* mid, astnode_t* right, uint64_t val_int) {
  if (allocd_nodes == NULL) {
    allocd_nodes = malloc(sizeof(astnode_t*));
  }

  astnode_t* n = malloc(sizeof(astnode_t));
  n->op = op;
  n->left = left;
  n->mid = mid;
  n->right = right;
  n->val_int = val_int;

  allocd_nodes[allocd_nodes_idx++] = n;
  allocd_nodes = realloc(allocd_nodes, sizeof(astnode_t*) * (allocd_nodes_idx + 2));

  return n;
}


astnode_t* mkastleaf(ast_nodetype_t op, uint64_t val_int) {
  return mkastnode(op, NULL, NULL, NULL, val_int);
}


astnode_t* mkastunary(ast_nodetype_t op, astnode_t* left, uint64_t val_int) {
  return mkastnode(op, left, NULL, NULL, val_int);
}


void destroy_ast_nodes(void) {
  if (!(allocd_nodes)) return;

  for (size_t i = 0; i < allocd_nodes_idx; ++i) {
    free(allocd_nodes[i]);
  }
  
  free(allocd_nodes);
  allocd_nodes = NULL;
  allocd_nodes_idx = 0;
}
