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

  puts("mallocing a new tree node");
  return ptr;
}

aa* copy_aa(aa* tree){
  puts("copying a tree node");
  return new_aa(tree->left, tree->right, tree->payload, tree->level);
}

void trash_aa(aa* tree){
  /* put tree in garbage list */
  puts("trashing a tree node");
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

aa* decrease_level(aa* tree){
  int should_be;
  aa* t2;
  aa* r2;

  should_be = 1 +
    (
        tree->left->level < tree->right->level
      ? tree->left->level
      : tree->right->level
    );

  if(should_be < tree->level){
    t2 = copy_aa(tree);
    t2->level = should_be;
    if(should_be < tree->right->level){
      r2 = copy_aa(tree->right);
      r2->level = should_be;
      trash_aa(tree->right);
    }
    trash_aa(tree);
    return t2;
  }
  else{
    return tree;
  }
}

void* successor(aa* tree){
  aa* ptr = tree->right;
  if(ptr == NULL){
    abort(); /* do not use successor on leaf node */
  }
  else{
    while(ptr->left != NULL){
      ptr = ptr->left;
    }
    return ptr->payload;
  }
}

void* predecessor(aa* tree){
  aa* ptr = tree->left;
  if(ptr == NULL){
    abort(); /* do not use predecessor on leaf node */
  }
  else{
    while(ptr->right != NULL){
      ptr = ptr->right;
    }
    return ptr->payload;
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

aa* delete(aa* tree, void* key, cmp_func compare){
  void* x;
  int diff;
  aa* node1;
  aa* node2;
  aa* node3;
  aa* node4;

  if(tree == NULL){
    return NULL;
  }
  else{
    diff = compare(tree->payload, key);

    if(diff < 0){
      node1 = copy_aa(tree);
      node1->right = delete(tree->right, key, compare);
      trash_aa(tree);
    }
    else if(diff > 0){
      node1 = copy_aa(tree);
      node1->left = delete(tree->left, key, compare);
      trash_aa(tree);
    }
    else{
      if(tree->left == NULL && tree->right == NULL){
        trash_aa(tree);
        return NULL;
      }
      else if(tree->left == NULL){
        x = successor(tree);
        node1 = copy_aa(tree);
        node1->right = delete(tree->right, x, compare);
        node1->payload = x;
        trash_aa(tree);
      }
      else{
        x = predecessor(tree);
        node1 = copy_aa(tree);
        node1->left = delete(tree->left, x, compare);
        node1->payload = x;
        trash_aa(tree);
      }
    }
  }

  node2 = decrease_level(node1);
  node3 = skew(node2);
  node3->right = skew(node3->right);
  node3->right->right = skew(node3->right->right);
  node4 = split(node3);
  node4->right = split(node4->right);

  return node4;
}

void* find(aa* tree, void* key, cmp_func compare){
  int diff;

  if(tree == NULL){
    return NULL;
  }
  else{
    diff = compare(tree->payload, key);
    if(diff < 0){
      return find(tree->left, key, compare);
    }
    else if(diff > 0){
      return find(tree->right, key, compare);
    }
    else{
      return tree->payload;
    }
  }
}

