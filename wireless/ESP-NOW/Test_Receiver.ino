#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure example to receive data
// Must match the sender structure
typedef struct test_message {
    char a[32];
    int b;
    float c;
    String d;
    bool e;
} test_message;

// Create a test_message called testData
test_message testData;

void sendReply(uint8_t *macAddr) {
  esp_now_add_peer(macAddr, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  strcpy(testData.a,"GOT MESSAGE");
  testData.b = random(1,20);
  testData.c = 1.2;
  testData.d = "DEEZ";
  testData.e = true;
  esp_now_send(macAddr, (uint8_t *) &testData, sizeof(testData));
  Serial.println("Sent Reply");
  esp_now_del_peer(macAddr);
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

  sendReply(mac);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
