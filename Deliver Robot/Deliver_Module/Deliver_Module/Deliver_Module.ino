#include <ESP8266WiFi.h>
#include <espnow.h>

// MAC Address of UI and Grabber Robot
uint8_t UI_MAC[] = {0x2C, 0xF4, 0x32, 0x44, 0xE5, 0xAA};
uint8_t Grabber_MAC[] = {0xA4, 0xCF, 0x12, 0xDD, 0x30, 0x87};

// Message Structure
typedef struct message_info {
  char Data;  // message to send/received
} message_info;

message_info messageInfo;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  if(sendStatus == 0){
    //Serial.write("Success");
  }
  else{
    //Serial.write("Fail");
  }
}

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&messageInfo, incomingData, sizeof(messageInfo));
  Serial.write(messageInfo.Data);
}

void setup() {
  // Init Serial Port
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if(esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register peers (UI and Deliver)
  esp_now_add_peer(UI_MAC, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(Grabber_MAC, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}

void loop() {
  // Send message when data received from MSP
  while(Serial.available() <= 0);
  char input = Serial.read();
  Serial.write('x');

  while(Serial.available() <= 0);
  messageInfo.Data = Serial.read();
  
  // Send message to Grabber Module
  if(messageInfo.Data == 'R' || messageInfo.Data == 'B' || messageInfo.Data == 'Y' || messageInfo.Data == 'W' || messageInfo.Data == 'H' || messageInfo.Data == 'N'){
    esp_now_send(Grabber_MAC, (uint8_t *) &messageInfo, sizeof(messageInfo));
    Serial.write('x');
  }
  // Send message to UI Module
  else if(messageInfo.Data == 'D'){
    esp_now_send(UI_MAC, (uint8_t *) &messageInfo, sizeof(messageInfo));
    Serial.write('x');
  }

}
