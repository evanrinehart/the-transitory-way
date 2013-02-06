typedef struct aa {
  struct aa* left;
  struct aa* right;
  void* payload;
  int level;
} aa;

typedef int cmp_func(void* a, void* b);

aa* aa_insert(aa* tree, void* payload, cmp_func compare);
aa* aa_delete(aa* tree, void* key, cmp_func compare);
void* aa_find(aa* tree, void* key, cmp_func compare);
