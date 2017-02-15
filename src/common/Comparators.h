#ifndef __LIBCOLLECTIONS_COMMON_COMPARATORS_H
#define __LIBCOLLECTIONS_COMMON_COMPARATORS_H

#include <stddef.h>//size_t

#include "Functions.h"
#include "../locality.h"

LIBCOLLECTIONS_PUBLIC Comparator* PointerComparator_init(Comparator* comparator);

LIBCOLLECTIONS_PUBLIC Comparator* FixedLengthValueComparator_init(Comparator* comparator, size_t length);

LIBCOLLECTIONS_PUBLIC Comparator* StringComparator_init(Comparator* comparator);
#endif //__LIBCOLLECTIONS_COMMON_COMPARATORS_H
