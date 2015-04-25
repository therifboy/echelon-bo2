#ifndef __UTILS_H__
#define __UTILS_H__

#include <sys/process.h>

#ifdef ARRAY_SIZE
#undef ARRAY_SIZE
#endif
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)		(sizeof(x) / sizeof(x[0]))
#endif

#ifdef STRING_ARRAY
#undef STRING_ARRAY
#endif
#ifndef STRING_ARRAY
#define STRING_ARRAY(x)		ARRAY_SIZE(x), x
#endif

#ifdef TTY_Write
#undef TTY_Write
#endif
#ifdef _DEBUG
#define TTY_Write(x)		console_write(x, sizeof(x))
#else
#define TTY_Write(x)
#endif

#ifdef STRZ
#undef STRZ
#endif
#ifdef STR
#undef STR
#endif

#define STRZ(x)		#x
#define STR(x)		STRZ(x)

#ifdef FUNCTION
#undef FUNCTION
#endif
#ifdef _DEBUG
#define FUNCTION			TTY_Write(STR(__FILE__) ": " STR(__LINE__) "\n")
#else
#define FUNCTION
#endif

#define HIWORD(x)	((x) >> 16)
#define LOWORD(x)	((x) & 0xFFFF)

#define HIDWORD(x)	((x) >> 32)
#define LODWORD(x)	((x) & 0x00000000FFFFFFFFLL)

#define Sleep(x)	(sys_timer_usleep(x * 1000))

extern inline void console_write(const char* msg,
								 size_t len);

extern inline void console_write(const char* msg,
				   size_t len)
{
	uint32_t writelen;
	system_call_4(403, 0, (uintptr_t)msg, len, (uintptr_t)&writelen);
}

extern inline int sys_dbg_read_process_memory(sys_pid_t pid, uint64_t process_ea,
											  size_t size, void* data);

extern inline int sys_dbg_read_process_memory(sys_pid_t pid, uint64_t process_ea,
											  size_t size, void* data)
{
	system_call_4(904, pid, process_ea, size, (uintptr_t)data);
	return_to_user_prog(int);
}

extern inline int sys_dbg_write_process_memory(sys_pid_t pid, uint64_t process_ea,
											   size_t size, const void* data);

extern inline int sys_dbg_write_process_memory(sys_pid_t pid, uint64_t process_ea,
											   size_t size, const void* data)
{
	system_call_4(905, pid, process_ea, size, (uintptr_t)data);
	return_to_user_prog(int);
}

#endif /* __UTILS_H__ */
