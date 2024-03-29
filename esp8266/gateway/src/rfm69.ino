/*

RFM69 MODULE

Copyright (C) 2016-2017 by Xose Pérez <xose dot perez at gmail dot com>

*/

#include "RFM69Manager.h"

// -----------------------------------------------------------------------------
// Locals
// -----------------------------------------------------------------------------

RFM69Manager * _radio;

struct _node_t {
  unsigned long count = 0;
  unsigned long missing = 0;
  unsigned long duplicates = 0;
  unsigned char lastPacketID = 0;
};

_node_t _nodeInfo[255];
unsigned char _nodeCount;
unsigned long _packetCount;

// -----------------------------------------------------------------------------
// Private
// -----------------------------------------------------------------------------

void _processMessage(packet_t * data) {

    blink(5, 1);

    DEBUG_MSG_P(
        PSTR("[MESSAGE] messageID:%d senderID:%d targetID:%d packetID:%d name:%s value:%s rssi:%d\n"),
        data->messageID,
        data->senderID,
        data->targetID,
        data->packetID,
        data->name,
        data->value,
        data->rssi
    );

    // Count seen nodes and packets
    if (_nodeInfo[data->senderID].count == 0) ++_nodeCount;
    ++_packetCount;

    // Detect duplicates and missing packets
    // packetID==0 means device is not sending packetID info
    if (data->packetID > 0) {
        if (_nodeInfo[data->senderID].count > 0) {

            unsigned char gap = data->packetID - _nodeInfo[data->senderID].lastPacketID;

            if (gap == 0) {
                DEBUG_MSG_P(PSTR("DUPLICATED\n"));
                _nodeInfo[data->senderID].duplicates = _nodeInfo[data->senderID].duplicates + 1;
                return;
            }

            if ((gap > 1) && (data->packetID > 1)) {
                DEBUG_MSG_P(PSTR("MISSING PACKETS!!\n"));
                _nodeInfo[data->senderID].missing = _nodeInfo[data->senderID].missing + gap - 1;
            }
        }

    }

    _nodeInfo[data->senderID].lastPacketID = data->packetID;
    _nodeInfo[data->senderID].count = _nodeInfo[data->senderID].count + 1;

    // Send info to websocket clients
    {
        char buffer[200];
        snprintf_P(
            buffer,
            sizeof(buffer) - 1,
            PSTR("{\"nodeCount\": %d, \"packetCount\": %lu, \"packet\": {\"senderID\": %u, \"targetID\": %u, \"packetID\": %u, \"name\": \"%s\", \"value\": \"%s\", \"rssi\": %d, \"duplicates\": %d, \"missing\": %d}}"),
            _nodeCount, _packetCount,
            data->senderID, data->targetID, data->packetID, data->name, data->value, data->rssi,
            _nodeInfo[data->senderID].duplicates , _nodeInfo[data->senderID].missing);
        wsSend(buffer);
    }

    // Try to find a matching mapping
    bool found = false;
    unsigned int count = getSetting("mappingCount", "0").toInt();
    for (unsigned int i=0; i<count; i++) {
        if ((getSetting("nodeid" + String(i)) == String(data->senderID)) &&
            (getSetting("key" + String(i)) == data->name)) {
            mqttSendRaw((char *) getSetting("topic" + String(i)).c_str(), (char *) String(data->value).c_str());
            found = true;
            break;
        }
    }

    if (!found) {
        String topic = getSetting("defaultTopic", MQTT_TOPIC_DEFAULT);
        if (topic.length() > 0) {
            topic.replace("{nodeid}", String(data->senderID));
            topic.replace("{key}", String(data->name));
            mqttSendRaw((char *) topic.c_str(), (char *) String(data->value).c_str());
        }
    }

}

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------

void clearCounts() {
    for(unsigned int i=0; i<255; i++) {
        _nodeInfo[i].duplicates = 0;
        _nodeInfo[i].missing = 0;
    }
    _nodeCount = 0;
    _packetCount = 0;
}

unsigned char getNodeCount() {
    return _nodeCount;
}

unsigned long getPacketCount() {
    return _packetCount;
}

// -----------------------------------------------------------------------------
// RFM69
// -----------------------------------------------------------------------------

#ifndef USE_RESET_PIN
#define USE_RESET_PIN false
#endif

void radioSetup() {
    delay(10);
    _radio = new RFM69Manager(SPI_CS, IRQ_PIN, IS_RFM69HW, digitalPinToInterrupt(IRQ_PIN));
    _radio->initialize(FREQUENCY, NODEID, NETWORKID, ENCRYPTKEY, USE_RESET_PIN, RESET_PIN);
    _radio->promiscuous(PROMISCUOUS);
    _radio->onMessage(_processMessage);
}

void radioLoop() {
    _radio->loop();
}
