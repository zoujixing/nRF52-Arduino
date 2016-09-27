
#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <string.h>
#include <stdint.h>

#define RING_BUFFER_MAX_SIZE 64


class Ring_Buffer
{
public:
    uint8_t  ring_buffer[RING_BUFFER_MAX_SIZE];
    uint16_t head;
    uint16_t tail;

public:
    Ring_Buffer(void){
        memset(ring_buffer, 0, RING_BUFFER_MAX_SIZE);
        head = 0;
        tail = 0;
    }

    void     store(uint8_t c);
    uint16_t available(void);
    uint8_t  read(void);
    uint8_t  peek(void);
    void     clear(void);
    bool     isFull(void);
};

#endif

