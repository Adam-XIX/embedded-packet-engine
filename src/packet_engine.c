#include "../include/packet_engine.h"
#include <stdio.h>

static uint8_t calculate_crc(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
    }
    return crc;
}

void parser_init(RxParser_t *parser) {
    parser->state = STATE_WAIT_START;
    parser->payload_idx = 0;
    parser->payload_len = 0;
    parser->rx_crc = 0;
}

bool parser_process_byte(RxParser_t *parser, uint8_t byte) {
    switch (parser->state) {
        case STATE_WAIT_START:
            if (byte == FRAME_START) {
                parser->payload_idx = 0;
                parser->state = STATE_WAIT_LEN;
            }
            break;

        case STATE_WAIT_LEN:
            if (byte <= MAX_PAYLOAD && byte > 0) {
                parser->payload_len = byte;
                parser->state = STATE_WAIT_PAYLOAD;
            } else {
                parser->state = STATE_WAIT_START;
            }
            break;

        case STATE_WAIT_PAYLOAD:
            parser->payload[parser->payload_idx++] = byte;
            if (parser->payload_idx >= parser->payload_len) {
                parser->state = STATE_WAIT_CRC;
            }
            break;

        case STATE_WAIT_CRC:
            parser->rx_crc = byte;
            parser->state = STATE_WAIT_END;
            break;

        case STATE_WAIT_END:
            parser->state = STATE_WAIT_START;
            if (byte == FRAME_END) {
                uint8_t expected_crc = calculate_crc(parser->payload, parser->payload_len);
                return (expected_crc == parser->rx_crc);
            }
            break;
    }
    return false;
}