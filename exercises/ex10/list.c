#include "list.h"
#include "dbg.h"

static int is_valid(List* list) {
	check(list->count >= 0, "Count of list is less than zero");
	check((list->first != NULL) || (list->count == 0),
	      "Got a non-empty list that has a first NULL");
	check((list->last != NULL) || (list->count == 0), "Got a non-empty list that has a last NULL");
	return 1;
error:
	return 0;
}

List* List_create() {
	List* list = calloc(1, sizeof(List));
	check_mem(list);
	return list;
error:
	return NULL;
}

int List_destroy(List* list) {
	check(is_valid(list), "List is invalid");
	LIST_FOREACH(list, first, next, cur) {
		if (cur->prev) {
			free(cur->prev);
		}
	}
	if (list->last) free(list->last);
	free(list);
	return 0;
error:
	return -1;
}

int List_clear(List* list) {
	check(is_valid(list), "List is invalid");
	LIST_FOREACH(list, first, next, cur) {
		if (cur->value) free(cur->value);
	}
	return 0;
error:
	return -1;
}

int List_clear_destroy(List* list) {
	check(is_valid(list), "List is invalid");
	List_clear(list);
	List_destroy(list);
	return 0;
error:
	return -1;
}

int List_push(List* list, void* value) {
	check(is_valid(list), "List is invalid");
	ListNode* node = calloc(1, sizeof(ListNode));
	check_mem(node);
	node->value = value;

	if (list->last == NULL) {
		list->first = node;
		list->last = node;
	} else {
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}

	list->count++;
	return 0;
error:
	return -1;
}

void* List_pop(List* list) {
	check(is_valid(list), "List is invalid");
	ListNode* node = list->last;
	return node != NULL ? List_remove(list, node) : NULL;
error:
	return NULL;
}

int List_unshift(List* list, void* value) {
	check(is_valid(list), "List is invalid");
	ListNode* node = calloc(1, sizeof(ListNode));
	check_mem(node);
	node->value = value;

	if (list->first == NULL) {
		list->first = node;
		list->last = node;
	} else {
		node->next = list->first;
		list->first->prev = node;
		list->first = node;
	}

	list->count++;
	return 0;
error:
	return -1;
}

void* List_shift(List* list) {
	check(is_valid(list), "List is invalid");
	ListNode* node = list->first;
	return node != NULL ? List_remove(list, node) : NULL;
error:
	return NULL;
}

void* List_remove(List* list, ListNode* node) {
	void* result = NULL;
	check(is_valid(list), "List is invalid");
	check(list->first && list->last, "List is empty.");
	check(node, "node can't be NULL");

	if (node == list->first && node == list->last) {
		list->first = NULL;
		list->last = NULL;
	} else if (node == list->first) {
		list->first = node->next;
		check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
		list->first->prev = NULL;
	} else if (node == list->last) {
		list->last = node->prev;
		check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
		list->last->next = NULL;
	} else {
		ListNode* after = node->next;
		ListNode* before = node->prev;
		after->prev = before;
		before->next = after;
	}

	list->count--;
	result = node->value;
	free(node);

error:
	return result;
}