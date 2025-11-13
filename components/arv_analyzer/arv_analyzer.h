#include "esphome.h"
#include "uart.h" 
class ArvAnalyzer : public Component, public uart::UARTDevice {
 public:
  ArvAnalyzer(uart::UARTComponent *parent) : uart::UARTDevice(parent) {}

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
        char buf[4];
        for (uint8_t b : packet) {
          sprintf(buf, "%02X ", b);
          hex += buf;
        }
        ESP_LOGD("arv_analyzer", "Packet (delta %dms): %s", delta, hex.c_str());
        packet.clear();
      }
      packet.push_back(byte);
      last_millis = now;
      now = millis();
    }
  }
};