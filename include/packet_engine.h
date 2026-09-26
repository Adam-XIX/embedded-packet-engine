#ifndef PACKET_ENGINE_H
#define PACKET_ENGINE_H

#include <stdint.h>
#include <stdbool.h>

#define FRAME_START 0xAA
#define FRAME_END   0x55
#define MAX_PAYLOAD 16

typedef enum {
    STATE_WAIT_START,
    STATE_WAIT_LEN,
    STATE_WAIT_PAYLOAD,
    STATE_WAIT_CRC,
    STATE_WAIT_END
} ParserState_t;

typedef struct {
    ParserState_t state;
    uint8_t payload[MAX_PAYLOAD];
    uint8_t payload_idx;
    uint8_t payload_len;
    uint8_t rx_crc;
} RxParser_t;

void parser_init(RxParser_t *parser);
bool parser_process_byte(RxParser_t *parser, uint8_t byte);

#endif