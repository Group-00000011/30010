/*
 * data_structures.c
 *
 *  Created on: Jun 13, 2023
 *      Author: oskar
 */

#include <stdlib.h>
#include <stdio.h>
#include "data_structures.h"




void list_push(listnode_t** head, void* elem) { // Add node to the beginning of list [O(1)]
	listnode_t* new_node = malloc(sizeof (listnode_t));
	if (new_node == 0)
		return;
	new_node->ptr = elem;
	new_node->next = *head;
	*head = new_node;
}

void* list_pop(listnode_t** head) { // Remove node from the beginning of list [O(1)]
	listnode_t* old_head = *head;
	*head = (*head)->next;
	return old_head->ptr;
}

void list_add() { // Add node to specific place in list

}

void list_append() { // Add node to end of list

}

void* list_remove(listnode_t** head, uint16_t idx) { // Remove node from specific place in list [O(n)]
	if (idx == 0) { // If idx is 0, remove==pop
		return list_pop(head);
	}
	listnode_t* current = *head;
	for (uint16_t i = 0; i < idx-1; i++) { // Go to the idx'th node
		current = current->next;
		if (current == NULL) // If end of list is reached, return null
			return NULL;
	}
	listnode_t* node_to_remove = current->next;
	current->next = current->next->next;
	return node_to_remove->ptr;
}

listnode_t* list_getnode(listnode_t* head, uint16_t idx) { // Return pointer to node at idx [O(n)]
	if (idx == 0) {
		return head;
	}
	for (uint16_t i = 0; i < idx-1; i++) {
		head = head->next;
		if (head == NULL)
			return NULL;
	}
	return head;
}

uint16_t list_length(listnode_t* head) { // Return the number of nodes in list [O(n)]
	uint16_t s = 0;
	listnode_t* current = head;
	while (current != NULL) {
		current = current->next;
		s++;
		if (current == head)
			break;
	}
	return s;
}

fixp_t fixp_fromint (fixp_t n) {
	return n << 14;
}

fixp_t fixp_toint(fixp_t n) {
	return n >> 14;
}

fixp_t fixp_add (fixp_t n1, fixp_t n2) {
	return n1 + n2;
}

fixp_t fixp_sub (fixp_t n1, fixp_t n2) {
	return n1 - n2;
}

fixp_t fixp_mult (fixp_t n1, fixp_t n2) {
	int64_t result = (int64_t) n1 * (int64_t) n2;
	return result >> 14;
}

fixp_t fixp_div (fixp_t n1, fixp_t n2) {
	return ((int64_t) n1 << 14) / (int64_t) n2;
}

fixp_t fixp_sqrt(fixp_t val) { // This function removes decimals may (will) be inaccurate
	val >>= 14;
	uint32_t a, b;

    if (val < 2) return val; /* avoid div/0 */
    a = 1255;       /* starting point is relatively unimportant */
    b = val / a; a = (a + b)>>1;
    b = val / a; a = (a + b)>>1;
    b = val / a; a = (a + b)>>1;
    b = val / a; a = (a + b)>>1;
    if (val < 20000) {
        b = val / a; a = (a + b)>>1;    // < 17% error Max
        b = val / a; a = (a + b)>>1;    // < 5%  error Max
    }

    return a <<= 14;
}

void fixp_print (fixp_t n) {
	// Prints a signed 18.14 fixed point number
	if ((n & 0x80000000) != 0) { // Handle negative numbers
		printf("-");
		n = ~n + 1;
	}
	printf("%ld.%04ld", n >> 14, 10000 * (uint32_t)(n & 0x3FFF) >> 14);
	// Print a maximum of 4 decimal digits to avoid overflow
}

