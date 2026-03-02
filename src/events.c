// events.c

#include "events.h"
#include <stddef.h>

static Event g_events[EVENT_QUEUE_SIZE];
static uint8_t g_head = 0;
static uint8_t g_tail = 0;
static uint8_t g_count = 0;

void events_clear(void)
{
    g_head = 0;
    g_tail = 0;
    g_count = 0;
}

uint8_t events_push(uint8_t type, GameObject* obj, uint8_t data0, uint8_t data1)
{
    if (g_count >= EVENT_QUEUE_SIZE)
        return 0;

    g_events[g_head].type = type;
    g_events[g_head].obj = obj;
    g_events[g_head].data0 = data0;
    g_events[g_head].data1 = data1;

    g_head = (uint8_t)((g_head + 1) & EVENT_QUEUE_MASK);
    g_count++;
    return 1;
}

uint8_t events_poll(Event* out)
{
    if (g_count == 0 || out == NULL)
        return 0;

    *out = g_events[g_tail];
    g_tail = (uint8_t)((g_tail + 1) & EVENT_QUEUE_MASK);
    g_count--;
    return 1;
}

/* End of events.c */
