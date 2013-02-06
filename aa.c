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

aa* copy_aa(aa* tree){
  return new_aa(tree->left, tree->right, tree->payload, tree->level);
}

void trash_aa(aa* tree){
  /* put tree in garbage list */
}

aa* skew(aa* tree){
  aa* l2;
  aa* t2;

  if(tree == NULL){
    return NULL;
  }
  else if(tree->left == NULL){
    return tree;
  }
  else if(tree->left->level == tree->level){
    l2 = copy_aa(tree->left);
    t2 = copy_aa(tree);
    t2->left = l2->right;
    l2->right = t2;
    trash_aa(tree->left);
    trash_aa(tree);
    return l2;
  }
  else{
    return tree;
  }
}

aa* split(aa* tree){
  aa* t2;
  aa* r2;

  if(tree == NULL){
    return NULL;
  }
  else if(tree->right == NULL || tree->right->right == NULL){
    return tree;
  }
  else if(tree->level == tree->right->right->level){
    r2 = copy_aa(tree->right);
    t2 = copy_aa(tree);
    t2->right = r2->left;
    r2->left = t2;
    r2->level += 1;
    trash_aa(tree->right);
    trash_aa(tree);
    return r2;
  }
  else{
    return tree;
  }
}

aa* insert(aa* tree, void* key, void* payload, cmp_func compare){
  aa* node1;
  aa* node2;
  aa* node3;

  if(tree == NULL){
    return new_aa(NULL, NULL, payload, 1);
  }
  else if(compare(tree->payload, key) < 0){
    node1 = copy_aa(tree);
    node1->left = insert(tree->left, key, payload, compare);
    trash_aa(tree->left);
  }
  else if(compare(tree->payload, key) > 0){
    node1 = copy_aa(tree);
    node1->right = insert(tree->right, key, payload, compare);
    trash_aa(tree->right);
  }
  else{
    node1 = copy_aa(tree);
    node1->payload = payload;
    trash_aa(tree);
    return node1;
  }

  node2 = skew(node1);
  node3 = split(node2);

  return node3;
}

void delete(aa* tree, void* key, cmp_func compare){
  /* ... */
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

