#ifndef __LIBCOLLECTIONS_LOCALITY_H
#define __LIBCOLLECTIONS_LOCALITY_H

#if defined _WIN32 || defined __CYGWIN__
	#ifdef BUILDING_DLL
		#ifdef __GNUC__
			#define LIBCOLLECTIONS_PUBLIC __attribute__ ((dllexport))
		#else
			#define LIBCOLLECTIONS_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
		#endif
	#else
		#ifdef __GNUC__
			#define LIBCOLLECTIONS_PUBLIC __attribute__ ((dllimport))
		#else
			#define LIBCOLLECTIONS_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
		#endif
	#endif
	#define LIBCOLLECTIONS_LOCAL
#else
	#if __GNUC__ >= 4
		#define LIBCOLLECTIONS_PUBLIC __attribute__ ((visibility ("default")))
		#define LIBCOLLECTIONS_LOCAL  __attribute__ ((visibility ("hidden")))
	#else
		#define LIBCOLLECTIONS_PUBLIC
		#define LIBCOLLECTIONS_LOCAL
	#endif
#endif

#ifndef WEAKREF
#define WEAKREF(fn) __attribute__ ((weak, alias(#fn)))
#endif

//Unused parameter macro
#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

#endif