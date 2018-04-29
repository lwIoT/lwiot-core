/*
 * Test if lwIoT properly initialises.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdio.h>
#include <stdlib.h>

#include <lwiot/test.h>

int main(int argc, char **argv)
{
	printf("Running lwIoT init test!\n");
	lwiot_init();
	wait_close();

	return -EXIT_SUCCESS;
}
