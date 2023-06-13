/*
 * data_structures.c
 *
 *  Created on: Jun 13, 2023
 *      Author: oskar
 */

#include <stdlib.h>
#include "data_structures.h"

void list_push(listnode_t** head, void* elem) { // Add node to the beginning of list
	listnode_t* new_node = malloc(sizeof (listnode_t));
	if (new_node == 0)
		return;
	new_node->ptr = elem;
	new_node->next = *head;
	*head = new_node;
}

void list_pop(listnode_t** head) { // Remove node from the beginning of list
	listnode_t* old_head = *head;
	*head = (*head)->next;
	free(old_head);
}

void list_add() { // Add node to specific place in list

}

void list_append() { // Add node to end of list

}

void list_remove() { // Remove node from specific place in list

}

void* list_getnode(listnode_t* head, uint16_t idx) { // Return pointer to node at idx
 return 0;
}

uint16_t list_length(listnode_t* head) { // Return the number of nodes in list
	return 0;
}
