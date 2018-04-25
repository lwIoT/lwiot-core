/*
 * lwIoT getopt header file.
 *
 * Author: Michel Megens
 * Date: 28/11/2017
 * Email: dev@bietje.net
 */

#pragma once

extern int getopt(int nargc, char * const nargv[], const char *ostr);
extern char *optarg;
extern int optind, opterr, optopt;
