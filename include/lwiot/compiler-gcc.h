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

typedef unsigned char u_char;

#define __compiler_barrier() __sync_synchronize()

#ifdef ESP32
#include <esp_attr.h>
#define RAM_ATTR IRAM_ATTR
#elif defined(ESP8266)
#include <common_macros.h>
#define RAM_ATTR IRAM
#else
#define RAM_ATTR
#endif

#ifdef IDE
/*
 * Visual studio code is far beyond retarded and doesn't understand
 * GCC attributes. Fix it by using an IDE only define.
 */
#ifdef PROGMEM
#undef PROGMEM
#undef __LPM_classic__
#undef __LPM_word_classic__
#endif

#define IRAM_ATTR
#define IMPLEMENT_ISR(x, y)
#ifdef sei
#undef sei
#undef cli

#define sei()
#define cli()
#endif

#define __LPM_classic__(addr) 0
#define __LPM_word_classic__(addr) 0
#define cli()
#define PROGMEM
#endif
