#ifndef List_h
#define List_h

#include <stdlib.h>

struct ListNode;

/**
 * Represents a node of a linked list
 **/
typedef struct ListNode {
	struct ListNode* next;
	struct ListNode* prev;
	void* value;
} ListNode;

/**
 * Represents the whole linked list
 **/
typedef struct List {
	int count;
	ListNode* first;
	ListNode* last;
} List;

/**
 * Creates a linked list
 * @returns a List struct or NULL if the creation failed
 **/
List* List_create();

/**
 * Destroys a linked list
 * @param list the linked list to destroy
 * @returns 0 on success, -1 on error
 **/
int List_destroy(List* list);

/**
 * Clears all values in a linked list
 * @param list the linked list to clear
 **/
int List_clear(List* list);

/**
 * Clears, then destroys a linked list
 * @param list the linked list to clear and destroy
 * @returns 0 on success, -1 on error
 **/
int List_clear_destroy(List* list);

/**
 * Count of a linked list
 * @param A the linked list to count
 * @returns the number of elements in the list
 * */
#define List_count(A) ((A)->count)

/**
 * First element of a linked list
 * @param A the linked list
 * @returns the first element in the provided list (NULL on empty list)
 **/
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)

/**
 * Last element of a linked list
 * @param A the linked list
 * @returns the last element in the provided list (NULL on empty list)
 **/
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

/**
 * Inserts a value to the end of a linked list
 * @param list the linked list
 * @param value value to insert
 * @returns 0 on success, -1 on error
 **/
int List_push(List* list, void* value);

/**
 * Removes the last item of a linked list
 * @param list list to remove from
 * @returns the removed element
 **/
void* List_pop(List* list);

/**
 * Adds a value to the front of a linked list
 * @param list the linked list
 * @param value the value to insert
 * @returns 0 on success, -1 on error
 **/
int List_unshift(List* list, void* value);

/**
 * Removes a value from the front of a linked list
 * @param list list to remove from
 * @returns the removed element
 **/
void* List_shift(List* list);

/**
 * Removes a node from a linked list
 * @param list the list to remove from
 * @param node the ListNode to remove
 * @returns the removed value
 **/
void* List_remove(List* list, ListNode* node);

/**
 * Copies a linked list
 * @param list the list to copy
 * @returns a new linked list as copy
 **/
List* List_copy(List* list);

/**
 * Splits a linked list
 * @param list list to split
 * @param start value to start the split on
 * @param end value to end the split on
 * @returns the splitted part of the provided list
 **/
List* List_split(List* list, void* start, void* end);

/**
 * Joins two linked lists
 * @param first list that will be on the front of the joined list
 * @param second list that will be on the end of the joined list
 * @return the joined list
 **/
List* List_join(List* first, List* second);

/**
 * Macro to iterate over a linked list
 * @param L the list
 * @param S the start (e.g. first or last->prev)
 * @param M the 'move' ('next' or 'prev')
 * @param V the iterator variable
 **/
#define LIST_FOREACH(L, S, M, V) \
	ListNode* _node = NULL; \
	ListNode* V = NULL; \
	for (V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif