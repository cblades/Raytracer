#include "ray.h"
#include "object.h"

#ifndef LIST_H
#define LIST_H
/* Allocate a new list header on the heap and initialize it */
list_t *list_init(void);

/* Add an element to the end of a list */
void list_add(list_t *, obj_t *);;

/* Delete a list and all of its elements */
void list_del(list_t *list);
#endif
