/*
 * Base 64 encoding header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>
#include <stdio.h>

#include <lwiot/types.h>
#include <lwiot/log.h>

#define BASE64_CHARS_PER_LINE 72

#define base64_encode_expected_len_nonewlines(n) ((((4 * (n)) / 3) + 3) & ~3)
#define base64_encode_expected_len(n) \
(base64_encode_expected_len_nonewlines(n) + ((n / ((BASE64_CHARS_PER_LINE * 3) / 4)) + 1))

#define base64_decode_expected_len(n) ((n * 3) / 4)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	step_A, step_B, step_C
} base64_encodestep;

typedef struct {
	base64_encodestep step;
	char result;
	int stepcount;
	int stepsnewline;
} base64_encodestate;

extern DLL_EXPORT void base64_init_encodestate(base64_encodestate* state_in);
extern DLL_EXPORT void base64_init_encodestate_nonewlines(base64_encodestate* state_in);
extern DLL_EXPORT char base64_encode_value(char value_in);
extern DLL_EXPORT int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in);
extern DLL_EXPORT int base64_encode_blockend(char* code_out, base64_encodestate* state_in);
extern DLL_EXPORT int base64_encode_chars(const char* plaintext_in, int length_in, char* code_out);

/* Decoding */
typedef enum {
	step_a, step_b, step_c, step_d
} base64_decodestep;

typedef struct {
	base64_decodestep step;
	char plainchar;
} base64_decodestate;

extern DLL_EXPORT void base64_init_decodestate(base64_decodestate* state_in);
extern DLL_EXPORT int base64_decode_value(char value_in);
extern DLL_EXPORT int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in);
extern DLL_EXPORT int base64_decode_chars(const char* code_in, const int length_in, char* plaintext_out);

#ifdef __cplusplus
}
#endif
