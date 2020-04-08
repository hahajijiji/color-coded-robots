#include <ESP8266WiFi.h>
#include <espnow.h>

// Client Module MAC Address
uint8_t clientMac[] = {0x2C, 0xF4, 0x32, 0x44, 0xE5, 0xAA};


// Test message structure
typedef struct test_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} test_message;

// Create a test_message called testData;
test_message testData;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
  Serial.print("Last Packet Send Status: ");
  if(sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&testData, incomingData, sizeof(testData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(testData.a);
  Serial.print("Int: ");
  Serial.println(testData.b);
  Serial.print("Float: ");
  Serial.println(testData.c);
  Serial.print("String: ");
  Serial.println(testData.d);
  Serial.print("Bool: ");
  Serial.println(testData.e);
  Serial.println();

  Serial.print("MacAddr: ");
  for (byte n = 0; n < 6; n++) {
    Serial.print (mac[n], HEX);
  }
  Serial.println();
}

void setup() {
  // Init Serial Monitor
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

  // Register peer
  esp_now_add_peer(clientMac, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}

void loop() {
  // Set values to send
  strcpy(testData.a,"THIS IS A CHAR");
  testData.b = random(1,20);
  testData.c = 1.2;
  testData.d = "Hello";
  testData.e = false;

  // Send message via ESP-NOW
  esp_now_send(clientMac, (uint8_t *) &testData, sizeof(testData));

  delay(2000);
}
