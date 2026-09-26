#include <stdio.h>
#include "../include/ring_buffer.h"
#include "../include/packet_engine.h"

int main(void) {
    RingBuffer_t rx_buffer;
    RxParser_t parser;

    ring_buffer_init(&rx_buffer);
    parser_init(&parser);

    // Raw stream arriving over hardware buffer
    uint8_t hardware_stream[] = {
        0x77, 0xBB,             // Noise
        0xAA,                   // START
        0x04,                   // LEN (4 bytes)
        0x10, 0x20, 0x30, 0x40, // PAYLOAD
        (0x10 ^ 0x20 ^ 0x30 ^ 0x40), // CRC
        0x55                    // END
    };

    size_t stream_size = sizeof(hardware_stream) / sizeof(hardware_stream[0]);

    printf("=== PHASE 3: Ring Buffer + FSM Parser Pipeline ===\n\n");

    // 1. Hardware Interrupt Simulation: Push bytes into Ring Buffer
    printf("[ISR] Receiving raw stream into Ring Buffer...\n");
    for (size_t i = 0; i < stream_size; i++) {
        if (ring_buffer_push(&rx_buffer, hardware_stream[i])) {
            printf("  Pushed: 0x%02X\n", hardware_stream[i]);
        }
    }

    printf("\n[CPU] Processing Ring Buffer contents through FSM Parser...\n");
    
    // 2. CPU Main Loop: Pop bytes from Ring Buffer and feed FSM
    uint8_t byte;
    while (ring_buffer_pop(&rx_buffer, &byte)) {
        printf("  Popped: 0x%02X -> ", byte);
        
        if (parser_process_byte(&parser, byte)) {
            printf("\n  >>> [SUCCESS] Full Frame Decoded! Payload: ");
            for (uint8_t j = 0; j < parser.payload_len; j++) {
                printf("0x%02X ", parser.payload[j]);
            }
            printf("<<<\n\n");
        } else {
            printf("Parsing...\n");
        }
    }

    return 0;
}