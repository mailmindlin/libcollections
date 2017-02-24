#include <stdlib.h>//For malloc, free

#include "../locality.h"//Mostly for UNUSED macro
#include "Hash.h"
#include "xxhash.h"

static void release_noop(void* p) {
	UNUSED(p);
}

static unsigned long Identity_apply(void* p, void* v) {
	UNUSED(p);
	return (unsigned long) v;
}

IntFunction* Identity_init(IntFunction* fn) {
	fn->apply = &Identity_apply;
	fn->release = &release_noop;
	fn->priv = NULL;
	return fn;
}

static unsigned long StringHash_apply(void* p, void* v) {
	UNUSED(p);
	if (v == NULL)
		return 0;
	char c;
	unsigned long result = 0;
	for (char* str = (char*) v; (c = *str) != '\0'; str++)
		result = result * 31 + ((unsigned long) c);
	return result;
}

IntFunction* StringHash_init(IntFunction* fn) {
	fn->apply = &StringHash_apply;
	fn->release = &release_noop;
	fn->priv = NULL;
	return fn;
}

struct FixedXXH_data {
	unsigned long seed;
	size_t length;
};

static unsigned long FixedXXH_apply(void* priv, void* value) {
	if (value == NULL)
		return 0;
	struct FixedXXH_data* data = (struct FixedXXH_data*) priv;
	//XXH64 is over 2x as fast as XXH32 on 64-bit machines (as could be expected), but is MUCH slower on 32-bit computers (1/3x).
	if (sizeof(unsigned long) >= 8)
		return (unsigned long) XXH64(value, data->length, (unsigned long long) data->seed);
	else
		return (unsigned long) XXH32(value, data->length, (unsigned int) data->seed);
}

IntFunction* FixedXXH_init(IntFunction* fn, unsigned long seed, size_t structSize) {
	if ((fn->priv = malloc(sizeof(struct FixedXXH_data))) == NULL)
		return NULL;
	((struct FixedXXH_data*)fn->priv)->seed = seed;
	((struct FixedXXH_data*)fn->priv)->length = structSize;
	fn->apply = &FixedXXH_apply;
	fn->release = &free;
	fn->priv = NULL;
	return fn;
}
