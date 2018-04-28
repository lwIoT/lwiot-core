/*
 * E/STACK - GCC compiler header
 *
 * Author: Michel Megens
 * Date: 12/12/2017
 * Email: dev@bietje.net
 */

#ifndef __COMPILER_H__
#error "Don't include compiler-gcc.h directly, include compiler.h instead"
#endif

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define __compiler_likely(x) __builtin_expect(!!(x), 1)
#define __compiler_unlikely(x) __builtin_expect(!!(x), 0)

#define DLL_EXPORT

#define __maybe __attribute__((weak))

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define __compiler_co(ptr, type, member) ({		\
		const typeof( ((type *)0)->member) *__mptr = (ptr); \
		(type *)( ( char *)__mptr - offsetof(type,member) );})

#ifndef __always_inline__
#define __always_inline__ __attribute__((__always_inline__))
#endif
typedef unsigned char u_char;

#define __compiler_barrier() __sync_synchronize()
