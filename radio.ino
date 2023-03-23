#include "settings.h"
#include "messages.h"
#define BANDWIDTH 41.7E3 // Possible 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3.
#define CODING_RATE 5
#define SPREADING_FACTOR 7

#define RX_BUFF_LENGTH 10
#define TX_BUFF_LENGTH 4

void radioInit(){
    // Setup Lora
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setCodingRate4(CODING_RATE);
  // LoRa.setTxPowerMax(int level)
  LoRa.setSpreadingFactor(SPREADING_FACTOR);
  // Set recieve callback
  LoRa.onReceive(onReceive);
}

// Sets radio to recieve mode
void radioRecieve(){
  LoRa.receive();
}

void sendMessage(){
  uint8_t txBuff[TX_BUFF_LENGTH];

  remoteMessage.enable = remoteState.enable;
  remoteMessage.rewind = remoteState.rewind;
  remoteMessage.throttle = remoteState.throttle;
  remoteMessage.error = remoteState.error;
  remoteMessage.time = millis();

  // Fill Buffer
  uint8_t flags = remoteMessage.enable |remoteMessage.rewind << 1;
  txBuff[0] = flags;     
  txBuff[1] = remoteMessage.throttle;   // First byte of line remaining
  txBuff[2] = remoteMessage.id >> 8;    // Message id
  txBuff[3] = remoteMessage.id & 0xFF;  // Message id

  // Send Buffer
  LoRa.beginPacket();
  for(int i = 0; i < TX_BUFF_LENGTH; i++){
    LoRa.write(txBuff[i]);
  }
  LoRa.endPacket(); // Sends packet
}

void onReceive(int packetSize){
  uint8_t rxBuff[RX_BUFF_LENGTH];
  resetBaseMessage();
  if (packetSize == 0) return; // No packet, return

  for (int i =0; i < packetSize; i++) {
  rxBuff[i] = LoRa.read();
  }

  baseMessage.batteryLevel = rxBuff[0];
  baseMessage.lineRemaining = rxBuff[1] << 8;
  baseMessage.lineRemaining = baseMessage.lineRemaining | rxBuff[2]; 
  baseMessage.id = (uint16_t)(rxBuff[3]) << 8;
  baseMessage.id = remoteMessage.id | rxBuff[4];
  baseMessage.speed = (int8_t)rxBuff[5];
  baseMessage.direction = rxBuff[6];
  baseMessage.time = millis();
  baseMessage.rssi = LoRa.packetRssi();
  remoteState.newMessage = true;
}

// A telemetry request is just a 2 byte packet
void requestTelemetry(){
  remoteState.lastTelemetry = millis();
  remoteMessage.time = millis();

  // Send Buffer
  LoRa.beginPacket();
  LoRa.write(0xFF);
  LoRa.write(0xFF);
  LoRa.endPacket(); // Sends packet
}

// Increments the message id and sets all other variables to 0
void resetBaseMessage(){
  baseMessage.batteryLevel = 0;
  baseMessage.error = 0;
  baseMessage.lineRemaining = 0;
  baseMessage.speed = 0;
  baseMessage.direction =0;
}

