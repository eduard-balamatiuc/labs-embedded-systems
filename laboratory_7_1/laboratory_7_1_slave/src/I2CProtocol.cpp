#include "../include/I2CProtocol.h"

uint8_t I2CProtocol::calculateChecksum(const DataPacket& packet) {
    uint8_t sum = 0;
    
    // Add header
    sum += packet.head;
    
    // Add length
    sum += packet.length;
    
    // Add payload bytes
    for (int i = 0; i < packet.length; i++) {
        sum += packet.payload[i];
    }
    
    return sum;
}

void I2CProtocol::createDistancePacket(DataPacket& packet, int distance) {
    // Setup header
    packet.head = PROTOCOL_HEAD;
    
    // Convert int to bytes (assuming 2-byte int)
    packet.payload[0] = (distance >> 8) & 0xFF;  // High byte
    packet.payload[1] = distance & 0xFF;         // Low byte
    
    // Set payload length
    packet.length = 2;
    
    // Calculate checksum
    packet.checksum = calculateChecksum(packet);
}

bool I2CProtocol::isPacketValid(const DataPacket& packet) {
    // Check header
    if (packet.head != PROTOCOL_HEAD) {
        return false;
    }
    
    // Check payload length is valid
    if (packet.length > MAX_PAYLOAD_SIZE) {
        return false;
    }
    
    // Verify checksum
    uint8_t calculatedChecksum = calculateChecksum(packet);
    return (calculatedChecksum == packet.checksum);
} 