#include "esphome.h"

class ArvAnalyzer : public Component, public UARTDevice {
 public:
  ArvAnalyzer(UARTComponent *parent) : UARTDevice(parent) {}

  void loop() override {
    static std::vector<uint8_t> packet;
    static uint32_t last_millis = 0;
    uint32_t now = millis();
    while (available()) {
      uint8_t byte;
      read_byte(&byte);
      uint32_t delta = now - last_millis;
      if (delta > 2 && !packet.empty()) {  // Packet end (pause > 2ms)
        std::string hex = "";
        for (uint8_t b : packet) hex += String::format("%02X ", b);
        ESP_LOGD("arv_analyzer", "Packet (delta %dms): %s", delta, hex.c_str());
        packet.clear();
      }
      packet.push_back(byte);
      last_millis = now;
      now = millis();
    }
  }
};