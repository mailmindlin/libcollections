/**
 * Usefull hash functions
 */
#ifndef __LIBCOLLECTIONS_HASH_H
#define __LIBCOLLECTIONS_HASH_H

#include <stddef.h> //size_t

#include "../locality.h"
#include "Functions.h"

LIBCOLLECTIONS_PUBLIC IntFunction* Identity_init(IntFunction* fn);
LIBCOLLECTIONS_PUBLIC IntFunction* StringHash_init(IntFunction* fn);
LIBCOLLECTIONS_PUBLIC IntFunction* FixedXXH_init(IntFunction* fn, size_t structSize);

#endif //__LIBCOLLECTIONS_HASH_H
