#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdint.h>
#include "export.h"

#ifdef _DEBUG
#define DBGPRINT(x)				printf(x)
#define DBGPRINTF(x, ...)		printf(x, __VA_ARGS__)
#else
#define DBGPRINTF(x, ...)
#endif

GAME_EXPORT int32_t printf(const char* format, ...);
GAME_EXPORT int32_t sprintf(char* destination, const char* format, ...);
GAME_EXPORT int32_t snprintf(char* destination, size_t size, const char* format, ...);

#endif /* __STDIO_H__ */
