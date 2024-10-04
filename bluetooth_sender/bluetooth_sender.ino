#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define CHANNEL 1

// This is all the data about the peer /
esp_now_peer_info_t slave;

// The all important data! /
uint8_t data = 0;

void setup() {
  tft.init();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
  ScanForSlave(); // WiFi.macAddress()
  esp_now_add_peer(&slave);

  pinMode(0,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP); 
  pinMode(16,INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);

  tft.fillScreen(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(4);
}

void loop() {
  tft.fillScreen(TFT_WHITE);

  if( !digitalRead(0) == HIGH )
  {
    data = 1;
  }else if( !digitalRead(14) == HIGH)
  {
    data = 2;
  }
  else if(digitalRead(16) == HIGH)
  {
    data = 3; 
  }
  else if(digitalRead(17) == HIGH)
  {
    data = 4; 
  }
  else{
    data = 0;
  }
  tft.drawString(String(data), 80, 100);
  esp_now_send(slave.peer_addr, &data, sizeof(data));
  delay(15);
}

// Scan for slaves in AP mode and ad as peer if found /
void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();

  for (int i = 0; i < scanResults; ++i) {
    String SSID = WiFi.SSID(i);
    String BSSIDstr = WiFi.BSSIDstr(i);

    if (SSID.indexOf("RX") == 0) {

      int mac[6];
      if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
        for (int ii = 0; ii < 6; ++ii ) {
          slave.peer_addr[ii] = (uint8_t) mac[ii];
        }
      }

      slave.channel = CHANNEL; // pick a channel
      slave.encrypt = 0; // no encryption
      break;
    }
  }
}

// callback when data is sent from Master to Slave /
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("I sent my data -> ");
  Serial.println(data);
}