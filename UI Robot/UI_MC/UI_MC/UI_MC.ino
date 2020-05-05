#include<LiquidCrystal.h>
LiquidCrystal lcd(P2_4, P2_3, P1_5, P2_0, P2_1, P2_2);

#define BLUE P2_7
#define RED P1_7
#define YELLOW P1_6
#define WHITE P2_5

int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;

char input;
char Message;

void setup()
{
  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.print("Welcome G3");
  lcd.setCursor(0,1);
  lcd.print("RoboTechs");
  delay(2000);
  lcd.clear();
  lcd.clear();
  lcd.print("Select Item");
  pinMode(BLUE, INPUT_PULLUP);
  pinMode(RED, INPUT_PULLUP);
  pinMode(YELLOW, INPUT_PULLUP);
  pinMode(WHITE, INPUT_PULLUP);
}

void loop()
{ 
  val1 = digitalRead(BLUE);
  val2 = digitalRead(RED);
  val3 = digitalRead(YELLOW);
  val4 = digitalRead(WHITE);
  
  if (val1 == LOW){
    lcd.clear();
    lcd.print("Blue!");
    delay(2000);
    lcd.clear();
    sendMessage('B');
    lcd.print("Item Currently:");
    lcd.setCursor(0,1);
    lcd.print("Retriving...");
  }
  if (val2 == LOW){
    lcd.clear();
    lcd.print("Red!");
    delay(2000);
    lcd.clear();
    sendMessage('R');
    lcd.print("Item Currently:");
    lcd.setCursor(0,1);
    lcd.print("Retriving...");
  }
  if (val3 == LOW){
    lcd.clear();
    lcd.print("Yellow!");
    delay(2000);
    lcd.clear();
    lcd.print("Sorry");
    lcd.setCursor(0,1);
    lcd.print("Out of Stock!");
  }
  if (val4 == LOW){
    lcd.clear();
    lcd.print("White!");
    delay(2000);
    lcd.clear();
    lcd.print("Sorry");
    lcd.setCursor(0,1);
    lcd.print("Out of Stock!");
  }

}

void sendMessage(char Data){
  // Notify WiFi module Data is available to send
  while(Serial.available() <= 0){
    Serial.write('x');
    // Break loop when WiFi module responds
    if(Serial.available() > 0){
      break;
    }
    delay(1000);
  }
  // Clear buffer 
  input = Serial.read();
  // Send Message Data to WiFi module
  Serial.write(Data);
}

void processMessage(char Data){
  if(Data == 'G'){
    lcd.clear();
    lcd.clear();
    lcd.print("Item Currently:");
    lcd.setCursor(0,1);
    lcd.print("Retrieved!");
  }
  else if(Data == 'H'){
    lcd.clear();
    lcd.clear();
    lcd.print("Select Item");
  }
  else if(Data == 'D'){
    lcd.clear();
    lcd.clear();
    lcd.print("Item Currently:");
    lcd.setCursor(0,1);
    lcd.print("Ready for Pickup!");
  }
}
