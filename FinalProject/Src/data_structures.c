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
	if (*head) {
		listnode_t* old_head = *head;
		*head = (*head)->next;
		void* to_return = old_head->ptr;
		free(old_head);
		return to_return;
	}
	return NULL;
}

void* list_remove_next(listnode_t* head) { // Remove node next to head in list [O(1)]
	if (head->next) {
		listnode_t* to_remove = head->next;
		head->next = head->next->next;
		void* ptr_to_entity = to_remove->ptr;
		free(to_remove); // Free the node
		return ptr_to_entity; // Return the struct pointed to by the node, to be freed elsewhere
	}
	return NULL;
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

void fixp_print (fixp_t n) {
	// Prints a signed 18.14 fixed point number
	if ((n & 0x80000000) != 0) { // Handle negative numbers
		printf("-");
		n = ~n + 1;
	}
	printf("%ld.%04ld", n >> 14, 10000 * (uint32_t)(n & 0x3FFF) >> 14);
	// Print a maximum of 4 decimal digits to avoid overflow
}

