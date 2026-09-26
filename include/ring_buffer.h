#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define RING_BUFFER_SIZE 64

typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    size_t head; // Write index
    size_t tail; // Read index
    size_t count; // Number of items stored
} RingBuffer_t;

void ring_buffer_init(RingBuffer_t *rb);
bool ring_buffer_push(RingBuffer_t *rb, uint8_t byte);
bool ring_buffer_pop(RingBuffer_t *rb, uint8_t *byte);
bool ring_buffer_is_empty(const RingBuffer_t *rb);

#endif