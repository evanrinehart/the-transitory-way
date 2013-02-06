#include <stdio.h>
#include <stdlib.h>

typedef struct aa {
  struct aa* left;
  struct aa* right;
  void* payload;
  int level;
} aa;

typedef int cmp_func(void* a, void* b);

aa* new_aa(aa* left, aa* right, void* payload, int level){
  aa* ptr = malloc(sizeof(struct aa));

  if(ptr == NULL){
    abort();
  }

  ptr->left = left;
  ptr->right = right;
  ptr->payload = payload;
  ptr->level = level;

  return ptr;
}

void* find(aa* tree, void* key, cmp_func compare){
  int foo;

  if(tree == NULL){
    return NULL;
  }
  else{
    foo = compare(tree->payload, key);
    if(foo == 0){
      return tree->payload;
    }
    else if(foo < 0){
      return find(tree->left, key, compare);
    }
    else if(foo > 0){
      return find(tree->right, key, compare);
    }
    else{
      abort(); /* impossible */
    }
  }
}

void insert(aa* tree, void* key, void* payload, cmp_func compare){
  /* find where to insert, insert, rebalance */
}

void delete(aa* tree, void* key, cmp_func compare){
  /* find what to delete, swap with leaf, delete leaf, rebalance */
}



