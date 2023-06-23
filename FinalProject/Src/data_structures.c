/*
 * data_structures.c
 *
 *  Created on: Jun 13, 2023
 *      Author: oskar
 */

#include <stdlib.h>
#include <stdio.h>
#include "data_structures.h"
// TODO Add macro to convert to/from fixp_t



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
	free(node_to_remove->ptr);
	return node_to_remove;
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

/*fixp_t fixp_fromint (fixp_t n) {
	return n << 14;
}

fixp_t fixp_toint(fixp_t n) {
	return n >> 14;
}*/

/*fixp_t fixp_add (fixp_t n1, fixp_t n2) {
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
}*/




//fixp_t fixp_sqrt(fixp_t val) { // This function removes decimals may (will) be inaccurate
//	val >>= 14;
//	uint32_t a, b;
//
//    if (val < 2) return val; /* avoid div/0 */
//    a = 1255;       /* starting point is relatively unimportant */
//    b = val / a; a = (a + b)>>1;
//    b = val / a; a = (a + b)>>1;
//    b = val / a; a = (a + b)>>1;
//    b = val / a; a = (a + b)>>1;
//    if (val < 20000) {
//        b = val / a; a = (a + b)>>1;    // < 17% error Max
//        b = val / a; a = (a + b)>>1;    // < 5%  error Max
//    }
//
//    return a <<= 14;
//}

//static uint16_t ftbl[33]={0,1,1,2,2,4,5,8,11,16,22,32,45,64,90,128,181,256,362,512,724,1024,1448,2048,2896,4096,5792,8192,11585,16384,23170,32768,46340};
//static uint16_t ftbl2[32]={ 32768,33276,33776,34269,34755,35235,35708,36174,36635,37090,37540,37984,38423,38858,39287,39712,40132,40548,40960,41367,41771,42170,42566,42959,43347,43733,44115,44493,44869,45241,45611,45977};


/*int fisqrt (int16_t val1, int16_t val2) {
    int cnt=0;
    int t = val;
    while (t) {cnt++;t>>=1;}
    if (6>=cnt)    t=(val<<(6-cnt));
    else           t=(val>>(cnt-6));

    return (ftbl[cnt]*ftbl2[t&31])>>15;
}*/

//uint16_t int_mag(int16_t val1, int16_t val2) { // This function removes decimals may (will) be inaccurate
//	int32_t v = val1*val1 + val2*val2;
//
//	//uint64_t a, b;
//
////    if (v < 2) return v; /* avoid div/0 */
////    a = 1255;       /* starting point is relatively unimportant */
////    b = v / a; a = (a + b)>>1;
////    b = v / a; a = (a + b)>>1;
////    b = v / a; a = (a + b)>>1;
////    b = v / a; a = (a + b)>>1;
////    if (v < 20000) {
////        b = v / a; a = (a + b)>>1;    // < 17% error Max
////        b = v / a; a = (a + b)>>1;    // < 5%  error Max
////    }
//
//	int32_t hi = v;
//	int32_t lo = 0;
//	int32_t mid = (hi + lo) / 2;
//	int32_t mid2 = mid*mid;
//	while (lo < hi-1 && mid2 != v) {
//	    if(mid2 < v) {
//	        lo = mid;
//	    } else {
//	        hi = mid;
//	    }
//	    mid = (hi + lo) / 2;
//	    mid2 = mid*mid;
//	}
//
//    return mid;
//}

void fixp_print (fixp_t n) {
	// Prints a signed 18.14 fixed point number
	if ((n & 0x80000000) != 0) { // Handle negative numbers
		printf("-");
		n = ~n + 1;
	}
	printf("%ld.%04ld", n >> 14, 10000 * (uint32_t)(n & 0x3FFF) >> 14);
	// Print a maximum of 4 decimal digits to avoid overflow
}

