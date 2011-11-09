#include <stdlib.h>
#include "list.h"
#include "common.h"
#include "safe.h"

/* 
 * Allocate a new list header on the heap and initialize it 
 *
 * RETURNS:
 *  pointer to a list_t linked list on the heap
 */
list_t *list_init(void) {
    list_t *list = (list_t *)smalloc(sizeof(list_t));
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

/* 
 * Add an element to the end of a list 
 * 
 * PARAMETERS:
 *  list   - the linked list to add the new element to
 *  entity - the 'object' to add to the linked list
 */
void list_add(list_t *list, obj_t *entity) {
    if (entity == NULL) {
        exit(EXIT_FAILURE);
    }
    
    entity->next = NULL;

    if (list->head == NULL) {
        list->head = entity;
        list->tail = entity;
    } else {
        list->tail->next = entity;
        list->tail = entity;
    }
}

/*
 * Delete all of the list objects and the list
 *
 * PARAMETERS:
 *  list - list_t to free
 */
void list_del(list_t *list) {
    obj_t *current;
    obj_t *temp;

    if (list->head == NULL) {
        free(list);
    } else {
        current = list->head;

        while (current != NULL) {
            temp = current->next;
            current->obj_free(current);
            current = temp;
        }

        free(list);
    }
}
