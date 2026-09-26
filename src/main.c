#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define FRAME_START 0xAA
#define FRAME_END   0x55
#define MAX_PAYLOAD 16

typedef struct {
    uint8_t start_byte;
    uint8_t length;
    uint8_t payload[MAX_PAYLOAD];
    uint8_t crc;
    uint8_t end_byte;
} Frame_t;

uint8_t calculate_crc(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
    }
    return crc;
}

bool build_frame(Frame_t *frame, const uint8_t *payload, uint8_t len) {
    if (len > MAX_PAYLOAD) return false;

    frame->start_byte = FRAME_START;
    frame->length = len;
    
    for (uint8_t i = 0; i < len; i++) {
        frame->payload[i] = payload[i];
    }

    frame->crc = calculate_crc(payload, len);
    frame->end_byte = FRAME_END;

    return true;
}

bool parse_frame(const Frame_t *frame) {
    printf("\n--- Test d'analyse du paquet ---\n");

    if (frame->start_byte != FRAME_START || frame->end_byte != FRAME_END) {
        printf("[ERREUR] Marqueurs de debut/fin invalides.\n");
        return false;
    }

    uint8_t expected_crc = calculate_crc(frame->payload, frame->length);
    if (frame->crc != expected_crc) {
        printf("[ERREUR] Erreur CRC ! Attendu: 0x%02X, Recu: 0x%02X\n", expected_crc, frame->crc);
        return false;
    }

    printf("[SUCCES] Paquet Valide ! Donnees: ");
    for (uint8_t i = 0; i < frame->length; i++) {
        printf("0x%02X ", frame->payload[i]);
    }
    printf("\n");
    return true;
}

int main(void) {
    Frame_t tx_packet;
    uint8_t sample_data[] = {0x12, 0x34, 0x56, 0x78};

    printf("Moteur de paquets embarque initialise.\n");

    build_frame(&tx_packet, sample_data, sizeof(sample_data));

    printf("\nTest 1: Transmission normale");
    parse_frame(&tx_packet);

    printf("\nTest 2: Simulation de bruit (donnee corrompue)");
    tx_packet.payload[1] = 0xFF; 
    parse_frame(&tx_packet);

    return 0;
}