#include <WiFi.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <WakeOnLan.h>

#define LED_BUILTIN 2

int ina226 = 0x40;
int ina219_1 = 0x41;
int ina219_2 = 0x44;
int ina219_3 = 0x45;
int reg_conf = 0x00;
int reg_vbus = 0x02;

const char* ssid = "office_2.4G";
const char* password = "123123123b";

const char *MACAddress = "2C:F0:5D:5A:34:AC";
int wake = 0;
WiFiUDP UDP;
WakeOnLan WOL(UDP);

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
  
  WOL.setRepeat(3, 100);
  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());
  
  led_boot();
}
void loop() {
  float v_bat = readv_ina226();
  if (wake == 0 && v_bat >= 27.0 && v_bat <= 36.0) {
    digitalWrite(LED_BUILTIN, HIGH);
    WOL.sendMagicPacket(MACAddress);
    digitalWrite(LED_BUILTIN, LOW);
    wake = 1;
  } else if (wake == 1 && v_bat <= 24.0) {
    wake = 0;
  }
  
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
