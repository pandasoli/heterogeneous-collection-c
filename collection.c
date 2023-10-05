#include "collection.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


static int append(Collection *self, const void *item, const size_t size) {
  if (self == NULL || item == NULL)
    return 1;

  // Add item
  self->data = realloc(self->data, self->data_size + size);
  if (self->data == NULL) return 2;

  memcpy(self->data + self->data_size, item, size);

  // Set new size info
  self->data_size += size;

  self->data_sizes = realloc(
    self->data_sizes,
    sizeof(unsigned short) * (self->size + 1)
  );
  if (self->data_sizes == NULL) return 2;

  self->data_sizes[self->size] = size;
  self->size++;

  return 0;
}

static void *at(const Collection *self, const size_t index) {
  if (self == NULL || index >= self->size)
    return NULL;

  size_t column = 0;

  for (size_t i = 0; i < index; i++)
    column += self->data_sizes[i];

  return self->data + column;
}

static void free_(Collection *self) {
  free(self->data);
  free(self->data_sizes);
  free(self);
}

static void mem_print(const Collection *self) {
  const size_t columns = 4;

  size_t itemi = 0;
  size_t edge_data_size = self->data_sizes[itemi];
  unsigned short color = 2;

  for (int row = 0; row < self->data_size; row += columns) {
    size_t row_size = self->data_size - row;
    row_size = row_size > columns ? columns : row_size;

    printf("%4d | ", row);

    for (int col = 0; col < row_size; col++) {
      if (row + col == edge_data_size) {
        itemi++;
        edge_data_size += self->data_sizes[itemi];
        color = color + 1 == 8 ? 2 : color + 1;
      }

      printf("\e[3%dm", color);
      if (col == 0) putchar(' ');
      printf("%02x ", ((char *) self->data)[row + col]);
    }

    printf("\e[0m");
    printf("  ");

    for (int col = 0; col < row_size; col++) {
      char ch = ((char *) self->data)[row + col];
      const int is_visible = 33 <= ch && ch <= 126;

      putchar(is_visible ? ch : '.');
    }


    putchar('\n');
  }
}


Collection *newCollection() {
  Collection *res = malloc(sizeof(Collection));
  if (res == NULL) return NULL;

  res->data = malloc(1);
  if (res->data == NULL) {
    free(res);
    return NULL;
  }

  res->data_sizes = malloc(1);
  if (res->data_sizes == NULL) {
    free(res->data);
    free(res);
    return NULL;
  }

  res->size = 0;

  res->append = &append;
  res->at = &at;
  res->mem_print = &mem_print;
  res->free = &free_;

  return res;
}
