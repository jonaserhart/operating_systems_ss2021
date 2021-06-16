#include "bstree.h"
#include "dbg.h"
#include <math.h>

typedef struct bstree {
	// reference the right child
	bstree* right;
	// reference the left child
	bstree* left;
	// holds the value of the node
	value_t value;
} bstree;

/**
 * Creates and returns a new binary tree.
 * Must be deallocated by bstree_destroy.
 */
bstree* bstree_create() {
	// allocate memory
	bstree* tree = malloc(sizeof(bstree));
	// value of root is not initialized
	tree->value = NAN;
	// check if allocation worked
	check_mem(tree);
	// return the tree
	return tree;
// if memory allocation failed, we are out of memory
error:
	return NULL;
}

/**
 * method to create a tree and initialize it with a value.
 * @param value: value to initialize the tree with
 **/
static bstree* bstree_create_with_value(value_t value) {
	check(isnan(value) == 0, "nan passed");
	bstree* tree = bstree_create();
	tree->value = value;
	check_mem(tree);
	return tree;
error:
	return NULL;
}

/**
 * Destroys and deallocates all memory for the given tree 't'
 */
void bstree_destroy(bstree* t) {
	if (t == NULL) {
		return;
	}
	bstree_destroy(t->left);
	bstree_destroy(t->right);
	free(t);
}

/**
 * Inserts the given value `v` into tree `t`.
 * If the value is already in `t`, no changes are made.
 * @param t: tree to insert value into
 * @param v: value to insert
 */
void bstree_insert(bstree* t, value_t v) {
	// check if 't' was allocated (is not NULL)
	check_mem(t);
	// check if v is a number
	check(isnan(v) == 0, "nan passed");
	// if the value is found
	if (t->value == v) {
		// do nothing
		return;
	}
	// t is newly created root or other node with no value
	if (isnan(t->value)) {
		// set the value
		t->value = v;
		return;
	}
	// if the value cannot be inserted here, search for a spot to insert the value
	if (t->value < v) {
		// value has to be inserted in the right subtree
		if (t->right == NULL) {
			// if there is a spot to the right
			// insert the value
			t->right = bstree_create_with_value(v);
		} else {
			// else: search further to the right
			bstree_insert(t->right, v);
		}
	} else {
		// value has to be inserted in the left subtree
		if (t->left == NULL) {
			// if there is a spot to the left
			// insert the value
			t->left = bstree_create_with_value(v);
		} else {
			// else: search further to the left
			bstree_insert(t->left, v);
		}
	}
error:
	return;
}

/**
 * Returns the node in tree 't' containing the highest value
 * @param t: tree to search the maxmimum node in
 */
bstree* bstree_maximum_node(bstree* t) {
	if (t->right == NULL) {
		return t;
	}
	return bstree_maximum_node(t->right);
}

/**
 * Aux method to remove an element from the tree
 *
 * this methods always returns the root of the given tree
 * if the given tree was restructured during this operation,
 * the restructured version will be returned
 *
 * @param t: tree to remove value from
 * @param v: value to remove
 */
static bstree* bstree_remove_aux(bstree* t, value_t v) {
	if (t == NULL) {
		return NULL;
	}
	// search for right value
	if (v > t->value) {
		t->right = bstree_remove_aux(t->right, v);
	} else if (v < t->value) {
		t->left = bstree_remove_aux(t->left, v);
	} else {
		// node with given value has been found
		if (t->left == NULL && t->right == NULL) {
			// if tree has no children, just destroy the tree
			bstree_destroy(t);
			return NULL;
		} else if (t->left == NULL || t->right == NULL) {
			// if tree has only one child
			bstree* temp;
			// find the non-NULL child
			if (t->left == NULL) {
				temp = t->right;
			} else {
				temp = t->left;
			}
			free(t); // cannot use bstree_destroy, because it also destroys subtrees
			// return the non-NULL child to the parent caller in recursion
			// if you look at lines 112-116, the value returned from a recursive call will be
			// set to the specified subtree
			return temp;
		} else {
			// if the node has two children
			// get the in-order predecessor (go one left then max right)
			bstree* temp = bstree_maximum_node(t->left);
			// change the value of the node to be deleted
			t->value = temp->value;
			// delete the node that just moved to the deleted node's position
			t->left = bstree_remove_aux(t->left, temp->value);
		}
	}
	// return new tree (not needed in this case but easier to implement recursively with signature
	// 'bstree* bstree_remove_aux')
	return t;
}

/**
 * Removes the given value `v` from tree `t`.
 */
void bstree_remove(bstree* t, value_t v) {
	check(isnan(v) == 0, "nan passed");
	check(t != NULL, "passed NULL for 't'");
	bstree_remove_aux(t, v);
error:
	return;
}

/**
 * Returns true if the given tree `t` contains `v`, false otherwise.
 */
bool bstree_contains(const bstree* t, value_t v) {
	check(isnan(v) == 0, "nan passed");
	if (t == NULL) {
		return false;
	}
	if (isnan(t->value)) {
		return false;
	}
	if (t->value < v) {
		return bstree_contains(t->right, v);
	}
	if (t->value > v) {
		return bstree_contains(t->left, v);
	}
	return true;
error:
	return false;
}

/**
 * Returns the smallest value in tree `t` or NaN if NULL was passed
 */
value_t bstree_minimum(const bstree* t) {
	check(t != NULL, "passed NULL for 't'");
	// if no more left children, return value
	if (t->left == NULL) {
		return t->value;
	}
	// else continue search
	return bstree_minimum(t->left);
error:
	return NAN;
}

/**
 * Returns the largest value in tree `t` or NaN if NULL was passed
 */
value_t bstree_maximum(const bstree* t) {
	check(t != NULL, "passed NULL for 't'");
	// if no more right children return value
	if (t->right == NULL) {
		return t->value;
	}
	// else continue search
	return bstree_minimum(t->right);
error:
	return NAN;
}

/**
 * Auxilary function for depth
 * @param t: tree to calculate depth in
 * @param v: value to calculate depth for
 */
static int32_t bstree_depth_aux(const bstree* t, value_t v) {
	if (t == NULL) {
		return -1;
	}
	if (t->value == v) {
		return 0;
	}
	if (t->value < v) {
		return 1 + bstree_depth_aux(t->right, v);
	} else {
		return 1 + bstree_depth_aux(t->left, v);
	}
}

/**
 * Returns the depth of the node in tree `t` containing the value `v`.
 * Returns -1 if the value `v` does not exist.
 */
int32_t bstree_depth(const bstree* t, value_t v) {
	check(isnan(v) == 0, "nan passed");
	if (bstree_contains(t, v) == 0) {
		return -1;
	}
	return bstree_depth_aux(t, v);
error:
	return NAN;
}

/**
 * Returns the number of values in the given bstree `t`.
 * NOTE: This should complete in O(1) time.
 * ANOTHERNOTE: this does not compute in O(1) time but in O(height of tree),
 * there is to much potential inconsitency (that i'm willing to handle)
 * when incrementing and decrementing the
 * size of a tree on each operation in c
 */
int32_t bstree_size(const bstree* t) {
	if (t == NULL) {
		return 0;
	}
	if (isnan(t->value)) {
		return 0;
	} else {
		return (bstree_size(t->left) + 1 + bstree_size(t->right));
	}
}

/**
 * Auxilary function to print a tree
 * @param t: tree to print
 * @param out: stream to print to
 */
static void bstree_print_aux(const bstree* t, FILE* out) {
	fprintf(out, "[");
	if (t->left != NULL) {
		// print left side
		bstree_print_aux(t->left, out);
		fprintf(out, ", ");
	}
	// print value
	printf(VALUE_T_FORMAT, t->value);
	if (t->right != NULL) {
		// print right side
		fprintf(out, ", ");
		bstree_print_aux(t->right, out);
	}
	fprintf(out, "]");
}

/**
 * Prints the given bstree `t` to the supplied output stream `out`.
 *
 * output format: [<LEFT>, VAL, <RIGHT>] : <SIZE>
 * example empty: [ NIL ] : 0
 * example 3,4,7 in a balanced tree: [[3], 4, [7]] : 3
 */
void bstree_print(const bstree* t, FILE* out) {
	check(out != NULL, "no stream passed");
	if (t == NULL || isnan(t->value)) {
		fprintf(out, "[ NIL ] : 0\n");
		return;
	}
	int size = bstree_size(t);
	bstree_print_aux(t, out);
	fprintf(out, " : %d\n", size);
error:
	return;
}
