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

int I2CProtocol::parseDistanceValue(const DataPacket& packet) {
    if (!isPacketValid(packet) || packet.length < 2) {
        return -1;  // Error or invalid packet
    }
    
    // Combine high and low bytes to form the distance value
    int distance = (packet.payload[0] << 8) | packet.payload[1];
    return distance;
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