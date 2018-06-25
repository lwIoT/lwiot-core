/*
 * ESP8266 C++ implementations.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" {

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

void __gxx_personality_v0(void)
{ }

}
