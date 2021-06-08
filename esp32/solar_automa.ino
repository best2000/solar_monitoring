#include <WiFi.h>
#include <Wire.h>

#define LED_BUILTIN 2

int ina226 = 0x40;
int ina226_2 = 0x45;
int ina219_1 = 0x41;
int ina219_2 = 0x44;
int reg_conf = 0x00;
int reg_vbus = 0x02;

const char* ssid = "office_2.4G";
const char* password = "123123123b";

WiFiServer server(80);
unsigned long currentTime;
unsigned long previousTime; 
const long timeoutTime = 250;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  
  Wire.begin();
  ina226_setup();
  ina219_setup();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  server.begin();
  
  led_boot();
}
void loop() {
  WiFiClient client = server.available();
  if (client) {
    digitalWrite(LED_BUILTIN, HIGH);
    currentTime = millis();
    previousTime = currentTime;
    boolean currentLineIsBlank = true;
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json; charset=utf-8");
          client.println("Server: Arduino");
          client.println("Connection: close");
          client.println();
          client.println(readv_JSON());
          client.println();
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    digitalWrite(LED_BUILTIN, LOW);
  }
 delay(100);
}
