// events.h

#ifndef EVENTS_H_
#define EVENTS_H_

#include "types.h"

#define EVENT_QUEUE_SIZE 16
#define EVENT_QUEUE_MASK (EVENT_QUEUE_SIZE - 1)

typedef enum {
    EVENT_NONE = 0,
    EVENT_BALL_PIN_COLLISION = 1
} EventType;

typedef struct {
    uint8_t type;
    GameObject* obj;
    uint8_t data0;
    uint8_t data1;
} Event;

void events_clear(void);
uint8_t events_push(uint8_t type, GameObject* obj, uint8_t data0, uint8_t data1);
uint8_t events_poll(Event* out);

#endif // EVENTS_H_
