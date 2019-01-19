/*
 * ESP8266 C++ implementations.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" {
#if __cplusplus >= 201402L
	void __attribute__((weak)) operator delete(void *ptr, size_t size) noexcept
	{
		::operator delete(ptr);
	}

	void __attribute__((weak)) operator delete[](void *ptr, size_t size) noexcept
	{
		::operator delete(ptr);
	}
#endif

	int __cxa_guard_acquire(__guard *g)
	{
		return !*(char *)(g);
	}

	void __cxa_guard_release (__guard *g)
	{
		*(char *)g = 1;
	}

	extern void __cxa_pure_virtual(void) __attribute__((weak));
	void __cxa_pure_virtual(void)
	{
		printf("Virtual function not implemented (%s:%i)", __FILE__, __LINE__);
		for(;;);
	}

	void __cxa_guard_abort (__guard *g)
	{
		for(;;);
	}

	extern void __gxx_personality_v0(void) __attribute__((weak));
	void __gxx_personality_v0(void)
	{ }

}
