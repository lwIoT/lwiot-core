/*
 * lwIoT byte order tool.
 *
 * Author: Michel Megens
 * Date: 28/11/2017
 * Email: dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef WIN32
#include <getopt.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

typedef unsigned char bool;

#define false 0
#define true  !false

static void print_usage_short(const char *program)
{
	fprintf(stdout, "%s: [options] <integer>\n", program);
}

static void print_usage_long(const char *program)
{
	print_usage_short(program);
	putchar('\n');
	putchar('\n');

	printf("\t-h\t\t Print this help text\n");
	printf("\t-v\t\t Print the program version string\n");
	printf("\t-s\t\t Treat the input number as a 16-bit integer\n");
	printf("\t-i\t\t Treat the input number as a 32-bit integer\n");
#ifdef WIN32
	printf("\t-l\t\t Treat the input number as a 64-bit integer\n");
#endif
	printf("\t-n\t\t Convert network to host order\n");
}

int main(int argc, char **argv)
{
	bool v_16, v_32;
#ifdef WIN32
	bool v_64;
#endif
	uint64_t num;
	int c;

	v_32 = true;
	v_16 = false;
#ifdef WIN32
	v_64 = false;
#endif

	while ((c = getopt(argc, argv, "hvisl")) != -1)
	{
		switch (c) {
			case 'h':
				print_usage_long(argv[0]);
				exit(-EXIT_SUCCESS);
				break;
			case 'v':
				printf("byteodertool 0.1.0\n");
				exit(-EXIT_SUCCESS);
				break;

			case 'i':
				break;

			case 's':
				v_16 = true;
				v_32 = false;
				break;

#ifdef WIN32
			case 'l':
			v_64 = true;
			v_32 = false;
			break;
#endif

			default:
				print_usage_short(argv[0]);
				exit(-EXIT_FAILURE);
				break;
		}
	}

	if (optind >= argc) {
		print_usage_short(argv[0]);
		return -EXIT_FAILURE;
	}

	num = atoll(argv[optind]);

	if (v_16)
		printf("Result: %u\n", htons(num & 0xFFFF));
	else if (v_32)
		printf("Result: %u\n", htonl((uint32_t)num));
#ifdef WIN32
	else if (v_64)
		printf("Result: %llu\n", htonll(num));
#endif

	return -EXIT_SUCCESS;
}
