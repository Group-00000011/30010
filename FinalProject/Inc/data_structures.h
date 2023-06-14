/*
 * data_structures.h
 *
 *  Created on: Jun 13, 2023
 *      Author: oskar
 */

#ifndef DATA_STRUCTURES_H_
#define DATA_STRUCTURES_H_

#include "stdint.h"

typedef struct listnode {
    void* ptr;
    struct listnode* next;
} listnode_t;


void list_push(listnode_t** head, void* elem);
void list_pop(listnode_t** head);
void list_add();
void list_append();
void list_remove(listnode_t** head, uint16_t idx);
void* list_getnode(listnode_t* head, uint16_t idx);
uint16_t list_length(listnode_t* head);


#endif /* DATA_STRUCTURES_H_ */
