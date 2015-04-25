#include "handle.h"
#include "addresses.h"

#include <sys/timer.h>
#include <string.h>

handle_s events[HANDLE_COUNT];

void handleAlloc(handle_t* handle, int32_t arg, modmenu_s* menu, handleCallback function, int32_t interval, clock_t start_time)
{
	if (function == NULL || interval == 0)
		return;

	if (handle != NULL && handle[0] != INVALID_HANDLE && events[handle[0]].function != NULL)
		return;

	handle_s* ev;
	for (int32_t i = 0; i < HANDLE_COUNT; i++)
	{
		ev = &events[i];
		if (ev->function == function && ev->arg == arg)
			return;
		else if (ev->function == NULL)
		{
			ev->arg = arg;
			ev->menu = menu;
			ev->function = function;
			ev->type = CALL_START;
			ev->handle = handle;
			ev->interval = (interval * (CLOCKS_PER_SEC / 1000));
			ev->time = (start_time == 0 ? clock() : start_time);
			if (handle != NULL)
				handle[0] = (handle_t)i;
			return;
		}
	}
	if (handle != NULL)
		handle[0] = INVALID_HANDLE;
}

handle_t handleAllocTemp(int32_t arg, modmenu_s* menu, handleCallback function, clock_t start_time)
{
	if (function == NULL)
		return INVALID_HANDLE;

	handle_s* ev;
	for (int32_t i = 0; i < HANDLE_COUNT; i++)
	{
		ev = &events[i];
		if (ev->function == function && ev->arg == arg)
			return i;
		else if (ev->function == NULL)
		{
			ev->arg = arg;
			ev->menu = menu;
			ev->function = function;
			ev->type = CALL_TEMP;
			ev->handle = NULL;
			ev->interval = 0;
			ev->time = (start_time == 0 ? clock() : start_time);
			return (handle_t)i;
		}
	}
	return INVALID_HANDLE;
}

bool handleStop(handle_t* handle)
{
	if (handle == NULL)
		return false;

	if (*handle != INVALID_HANDLE)
	{
		events[*handle].type = CALL_STOP;
		*handle = INVALID_HANDLE;
		return true;
	}
	return false;
}

bool handleFree(handle_t* handle)
{
	if (handle == NULL)
		return false;

	if (*handle != INVALID_HANDLE)
	{
		events[*handle].type = CALL_FREE;
		return true;
	}
	return false;
}

void handleExecute()
{
	clock_t time;
	handleCallback function;
	handleCall_t type;
	handle_s* ev;
	handle_t dummy;

	for (handle_t i = 0; i < HANDLE_COUNT; i++)
	{
		ev = &events[i];
		if ((function = ev->function) != NULL)
		{
			if (ev->type != CALL_FREE)
			{
				time = clock();
				if (ev->time <= time)
				{
					ev->time += ev->interval;
					type = ev->type;
					if (ev->handle == NULL)
					{
						dummy = i;
						function(ev->arg, ev->menu, &dummy, type);
					}
					else
					{
						function(ev->arg, ev->menu, ev->handle, type);
					}
					switch (type)
					{
					default:
						break;
					case CALL_START:
						ev->type = CALL_EXECUTE;
						break;
					case CALL_TEMP:
					case CALL_STOP:
						memset(ev, 0, sizeof(handle_s));
						break;
					}
				}
			}
		}
	}
}

void handleStopAll()
{
	clock_t time = clock();
	for (handle_t i = 0; i < HANDLE_COUNT; i++)
	{
		if (events[i].function != NULL)
		{
			events[i].time = time;
			events[i].type = CALL_STOP;
		}
	}
}

void handleClearAll()
{
	memset(&events, 0, sizeof(events));
}
