
#include "ring_buffer.h"

/****************************************************
*                 Function Definitions
****************************************************/
void Ring_Buffer::store( uint8_t c)
{
    uint16_t i = (( head +1 ) % RING_BUFFER_MAX_SIZE);

    if( i != tail )
    {   // Store
        ring_buffer[head] = c;
        head = i;
    }
}

uint16_t Ring_Buffer::available()
{
    return ((RING_BUFFER_MAX_SIZE + head - tail) % RING_BUFFER_MAX_SIZE);
}

uint8_t Ring_Buffer::read(void)
{
    uint8_t dat;

    if(head == tail)
        return -1;

    dat = ring_buffer[tail];
    tail = (tail + 1) % RING_BUFFER_MAX_SIZE;

    return dat;
}

uint8_t Ring_Buffer::peek(void)
{
    if(head == tail)
        return -1;

    return ring_buffer[tail];
}

void Ring_Buffer::clear(void)
{
    memset(ring_buffer, 0, RING_BUFFER_MAX_SIZE);
    head = 0;
    tail = 0;
}

bool Ring_Buffer::isFull(void)
{
    uint16_t next_index = ((head + 1) % RING_BUFFER_MAX_SIZE);
    return (next_index == tail);
}
