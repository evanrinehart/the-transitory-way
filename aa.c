#include <stdio.h>
#include <stdlib.h>

#include <aa.h>

static aa* new_aa(aa* left, aa* right, void* payload, int level){
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

static aa* copy_aa(aa* tree){
  return new_aa(tree->left, tree->right, tree->payload, tree->level);
}

static void trash_aa(aa* tree){
  /* put tree in garbage list */
  if(tree != NULL){
  }
}

static aa* skew(aa* tree){
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

static aa* split(aa* tree){
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

static aa* decrease_level(aa* tree){
  int l1;
  int l2;
  int should_be;
  aa* t2;
  aa* r2;

  if(tree == NULL){
    abort(); /* do not decrease level on empty tree */
  }

  l1 = tree->left==NULL ? 0 : tree->left->level;
  l2 = tree->right==NULL ? 0 : tree->right->level;
  should_be = (l1 < l2 ? l1 : l2) + 1;

  if(should_be < tree->level){
    t2 = copy_aa(tree);
    t2->level = should_be;
    if(tree->right != NULL && should_be < tree->right->level){
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

static void* successor(aa* tree){
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

static void* predecessor(aa* tree){
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

aa* aa_insert(aa* tree, void* item, cmp_func compare){
  aa* node1;
  aa* node2;
  aa* node3;
  int diff;

  if(tree == NULL){
    return new_aa(NULL, NULL, item, 1);
  }
  else{
    diff = compare(tree->payload, item);

    if(diff < 0){
      node1 = copy_aa(tree);
      node1->left = aa_insert(tree->left, item, compare);
      trash_aa(tree);
    }
    else if(diff > 0){
      node1 = copy_aa(tree);
      node1->right = aa_insert(tree->right, item, compare);
      trash_aa(tree);
    }
    else{
      node1 = copy_aa(tree);
      node1->payload = item;
      trash_aa(tree);
      return node1;
    }
  }

  node2 = skew(node1);
  node3 = split(node2);

  return node3;
}

aa* aa_delete(aa* tree, void* item, cmp_func compare){
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
    diff = compare(tree->payload, item);

    if(diff > 0){
      node1 = copy_aa(tree);
      node1->right = aa_delete(tree->right, item, compare);
      trash_aa(tree);
    }
    else if(diff < 0){
      node1 = copy_aa(tree);
      node1->left = aa_delete(tree->left, item, compare);
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
        node1->right = aa_delete(tree->right, x, compare);
        node1->payload = x;
        trash_aa(tree);
      }
      else{
        x = predecessor(tree);
        node1 = copy_aa(tree);
        node1->left = aa_delete(tree->left, x, compare);
        node1->payload = x;
        trash_aa(tree);
      }
    }
  }

  node2 = decrease_level(node1);
  node3 = skew(node2);
  node3->right = skew(node3->right);
  if(node3->right != NULL){
    node3->right->right = skew(node3->right->right);
  }
  node4 = split(node3);
  node4->right = split(node4->right);

  return node4;
}

void* aa_find(aa* tree, void* item, cmp_func compare){
  int diff;

  if(tree == NULL){
    return NULL;
  }
  else{
    diff = compare(tree->payload, item);
    if(diff < 0){
      return aa_find(tree->left, item, compare);
    }
    else if(diff > 0){
      return aa_find(tree->right, item, compare);
    }
    else{
      return tree->payload;
    }
  }
}

void print_aa(aa* tree){
  if(tree != NULL){
    printf("([%c] %d ", *((char*)tree->payload), tree->level);
    printf("L");
    print_aa(tree->left);
    printf(" R");
    print_aa(tree->right);
    printf(")");
  }
  else{
    printf("()");
  }
}

char* new_payload(char c){
  char* ptr = malloc(1);
  *ptr = c;
  return ptr;
}

int cmp_char(void* v1, void* v2){
  char* c1 = v1;
  char* c2 = v2;
  return *c1 - *c2;
}

int main(){
  aa* tree = NULL;

  puts("OK");

  int i;
  int c = 33;
  for(i=0; i<50; i++){
    tree = aa_insert(tree, new_payload(c), cmp_char);
    c++;
  }

  print_aa(tree);
  puts("");
  puts("inserted 50 things");

  printf("searching for @: %p\n", aa_find(tree, new_payload('@'), cmp_char));

  tree = aa_delete(tree, new_payload('@'), cmp_char);

  print_aa(tree);
  puts("");
  puts("deleted @");

  printf("searching for @: %p\n", aa_find(tree, new_payload('@'), cmp_char));
}
