/*
 * lwIoT project header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot_opts.h>

#include <lwiot/compiler.h>
#include <lwiot/types.h>
#include <lwiot/port.h>

#ifdef UNUSED
#undef UNUSED
#endif

#define UNUSED(x) (void)x

#define CXX __cplusplus

#ifndef BIT
#define BIT(__x) (1U << __x)
#endif

#define BITS_PER_BYTE 8U
