/*
 * lwIoT - Unit test header
 *
 * Author: Michel Megens
 * Date:   13/12/2017
 * Email:  dev@bietje.net
 */

#ifndef __TEST_H__
#define __TEST_H__

#include <stdio.h>
#include <lwiot_opts.h>

#ifdef HAVE_CI
#define wait_close()
#else
#define wait_close() getchar()
#endif

#endif /* !__TEST_H__ */
