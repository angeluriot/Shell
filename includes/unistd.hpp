#ifndef UNISTD_H
#define UNISTD_H

#ifdef __unix__
#include <unistd.h>
#include <termios.h>
#else
#include <stdlib.h>
#include <io.h>
#include <process.h>
#include <direct.h>
#endif

#endif
