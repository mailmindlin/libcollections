#ifndef __LIBCOLLECTIONS_COMMON_FUNCTIONS_H
#define __LIBCOLLECTIONS_COMMON_FUNCTIONS_H

typedef struct {
	void (*apply) (void* priv, void* data);
	void (*release) (void* priv);
	void* priv;
} Consumer;

typedef struct {
	void* (*apply) (void* priv, void* data);
	void (*release) (void* priv);
	void* priv;
} Function;

/**
 * A comparison function that is able to produce a total ordering of a set of elements.
 * Care should be given such that the ordering of all elements is consitent (for example,
 * if cmp(a,b) > 0, cmp(b,c) > 0, then cmp(a,c) must > 0).
 * Returns a positive number if a > b
 * Returns 0 if a is equivalent to b
 * Returns a negative number if a < b
 */
typedef struct {
	int (*apply) (void* priv, const void* a, const void* b);
	void (*release) (void* priv);
	void* priv;
} Comparator;

typedef struct {
	unsigned long (*apply) (void* priv, void* data);
	void (*release) (void* priv);
	void* priv;
} IntFunction;

#endif