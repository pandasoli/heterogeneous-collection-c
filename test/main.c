#include <stdio.h>
#include "../collection.h"

typedef struct {
  char name[30];
  unsigned short age;
} Person;


int main(void) {
  Person panda = {"Panda", 18};
  int num = 12;

  Collection *coll = newCollection();
  if (coll == NULL) {
    perror("Could not create collection\n");
    return 1;
  }

  coll->append(coll, &panda, sizeof(panda));
  coll->append(coll, &num, sizeof(num));

  printf("%s\n", ((Person *) coll->at(coll, 0))->name);
  printf("%d\n", ((Person *) coll->at(coll, 0))->age);
  printf("%d\n", *((int *) coll->at(coll, 1)));
  printf("\n");

  coll->mem_print(coll);
  coll->free(coll);

  return 0;
}
