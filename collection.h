#pragma once
#include <stddef.h>


typedef struct _Collection {
  void *data;
  size_t *data_sizes;
  size_t data_size;

  size_t size;

  int (*append)(struct _Collection *self, const void *item, const size_t size);
  void *(*at)(const struct _Collection *self, const size_t index);

  void (*mem_print)(const struct _Collection *self);
  void (*free)(struct _Collection *self);
} Collection;

Collection *newCollection();
