#include "../include/ring_buffer.h"

void ring_buffer_init(RingBuffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

bool ring_buffer_push(RingBuffer_t *rb, uint8_t byte) {
    if (rb->count >= RING_BUFFER_SIZE) {
        return false; // Buffer full (overflow protection)
    }
    
    rb->buffer[rb->head] = byte;
    rb->head = (rb->head + 1) % RING_BUFFER_SIZE;
    rb->count++;
    return true;
}

bool ring_buffer_pop(RingBuffer_t *rb, uint8_t *byte) {
    if (rb->count == 0) {
        return false; // Buffer empty
    }

    *byte = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
    rb->count--;
    return true;
}

bool ring_buffer_is_empty(const RingBuffer_t *rb) {
    return (rb->count == 0);
}