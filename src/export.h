#ifndef __EXPORT_H__
#define __EXPORT_H__

#include "addresses.h"

#ifdef __cplusplus
#define LONGCALL extern "C" __attribute__((longcall))
#else
#define LONGCALL extern __attribute__((longcall))
#endif

#define GAME_EXPORT LONGCALL

#endif /* __EXPORT_H__ */
