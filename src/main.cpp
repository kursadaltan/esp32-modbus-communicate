#include <Arduino.h>
#include <esp32ModbusRTU.h>
#include <algorithm>  // for std::reverse

esp32ModbusRTU modbus(&Serial1, GPIO_NUM_33);  // use Serial1 and pin 16 as RTS


uint16_t response ;
uint16_t response2 ;
uint8_t var[2];
int number ;
char *text ; 
void setup() {
  Serial.begin(115200);  // Serial output
  Serial1.begin(9600, SERIAL_8N1, GPIO_NUM_5, GPIO_NUM_17, false);  // Modbus connection

  modbus.onData([](uint8_t serverAddress, esp32Modbus::FunctionCode fc, uint8_t* data, size_t length) {
    Serial.printf("id 0x%02x fc 0x%02x len %u: 0x", serverAddress, fc, length);
   
    for (size_t i = 0; i < length; ++i) {
      Serial.printf("%02x", data[i]);
      var[i] = data[i];
    }
    Serial.print("\n\n Data = \t");
    for(size_t i = 0; i < sizeof(length); ++i){
      Serial.printf("%02x \t", data[i]);
    }
    Serial.println("\n");

    std::reverse(var, var + sizeof(var));  // fix endianness  
    memcpy(&response2, &var, sizeof(var));
    Serial.print("\nDegree =");
    Serial.println(response2);
    Serial.print("\n\n");
  });
  modbus.onError([](esp32Modbus::Error error) {
    Serial.printf("error: 0x%02x\n\n", static_cast<uint8_t>(error));
  });
  modbus.begin();
}

void loop() {
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 100) {
    lastMillis = millis();
    Serial.print("sending Modbus request...\n");
    //modbus.readInputRegisters(0x01, 93, 4);
    modbus.readHoldingRegisters(93, 0, 1);
   
  }
}


