#include "btree.h"

int main() {
  BTA *btfile;
  btinit();
  btfile = btcrt("test_db", 0, 0);
  binsky(btfile, "akey", 99);
  btcls(btfile);
  return 0;
}
