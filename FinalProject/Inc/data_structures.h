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

#define fixp_fromint(a) ((a)<<14)
#define fixp_toint(a) ((a)>>14)
#define fixp_add(a,b) ((a)+(b))
#define fixp_sub(a,b) ((a)-(b))
#define fixp_mult(a,b) ((((int64_t) (a)) * ((int64_t) (b)))>>14)
#define fixp_div(a,b) ((((int64_t) (a)) << 14) / ((int64_t) (b)))

typedef struct listnode {
    void* ptr;
    struct listnode* next;
} listnode_t;

void list_push(listnode_t** head, void* elem);
void* list_pop(listnode_t** head);
void* list_remove_next(listnode_t* head);

void fixp_print (fixp_t n);

#endif /* DATA_STRUCTURES_H_ */
