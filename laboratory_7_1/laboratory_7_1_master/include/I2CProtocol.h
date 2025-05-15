#ifndef I2C_PROTOCOL_H
#define I2C_PROTOCOL_H

#include <Arduino.h>

// Protocol definitions
#define PROTOCOL_HEAD      0xAA  // Start of message
#define MAX_PAYLOAD_SIZE   8     // Maximum size of payload

// Packet structure
typedef struct {
    uint8_t head;         // Protocol header (0xAA)
    uint8_t length;       // Length of payload
    uint8_t payload[MAX_PAYLOAD_SIZE];  // Data payload
    uint8_t checksum;     // Simple checksum for verification
} DataPacket;

class I2CProtocol {
public:
    // Calculate checksum (simple byte sum)
    static uint8_t calculateChecksum(const DataPacket& packet);
    
    // Parse distance value from packet payload
    static int parseDistanceValue(const DataPacket& packet);
    
    // Verify if packet is valid (correct header and checksum)
    static bool isPacketValid(const DataPacket& packet);
};

#endif // I2C_PROTOCOL_H 