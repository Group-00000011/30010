/*
 * data_structures.h
 *
 *  Created on: Jun 13, 2023
 *      Author: oskar
 */

#ifndef DATA_STRUCTURES_H_
#define DATA_STRUCTURES_H_

#include "stdint.h"

#define fixp_t int32_t

typedef struct listnode {
    void* ptr;
    struct listnode* next;
} listnode_t;


void list_push(listnode_t** head, void* elem);
void* list_pop(listnode_t** head);
void list_add();
void list_append();
void* list_remove(listnode_t** head, uint16_t idx);
listnode_t* list_getnode(listnode_t* head, uint16_t idx);
uint16_t list_length(listnode_t* head);


fixp_t fixp_fromint (fixp_t n);
fixp_t fixp_toint (fixp_t n);
fixp_t fixp_add (fixp_t n1, fixp_t n2);
fixp_t fixp_mult (fixp_t n1, fixp_t n2);
fixp_t fixp_div (fixp_t n1, fixp_t n2);
fixp_t fixp_sqrt(fixp_t val);
void fixp_print (fixp_t n);



#endif /* DATA_STRUCTURES_H_ */
