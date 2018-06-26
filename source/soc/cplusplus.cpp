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
#ifdef AVR
void *operator new(size_t num)
{
	auto ptr = malloc(num);

	memset(ptr, 0, sizeof(num));
	return ptr;
}

void *operator new[](size_t num)
{
	auto ptr = malloc(num);

	memset(ptr, 0, sizeof(num));
	return ptr;
}

void operator delete(void *ptr)
{
	if(ptr == nullptr)
		return;

	free(ptr);
}

void operator delete[](void *ptr)
{
	if(ptr == nullptr)
		return;

	free(ptr);
}

int atexit( void (*func)(void))
{
	return -1;
}
#endif

/**
 * @brief Lock for data initialisation.
 * @param g Lock to aquire.
 * @return Non zero if the lock was aquired succesfully.
 */
int __cxa_guard_acquire(__guard *g) 
{
	return !*(char *)(g);
}

/**
 * @brief Release the lock aquired with __cxa_guard_acquire
 */
void __cxa_guard_release (__guard *g) 
{
	*(char *)g = 1;
}

extern void __cxa_pure_virtual(void) __attribute__((weak));
/**
 * @brief Temporary implementation for pure virtual functions.
 */
void __cxa_pure_virtual(void)
{
	printf("Virtual function not implemented (%s:%i)", __FILE__, __LINE__);
}

/**
 * @brief Abort the program.
 */
void __cxa_guard_abort (__guard *g)
{
	for(;;);
}

extern void __gxx_personality_v0(void) __attribute__((weak));
void __gxx_personality_v0(void)
{ }

}
