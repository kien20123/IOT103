#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SOIL_PIN 14
#define RELAY_PIN 27
#define LIGHT_PIN 5
#define LED_PIN 33

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  pinMode(SOIL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();
}

void loop() {

  int soilState = digitalRead(SOIL_PIN);
  int lightValue = digitalRead(LIGHT_PIN);
  if (soilState == HIGH) {
    lcd.setCursor(0, 0);
    lcd.print("Soil: Dry");
    if(lightValue == 0){
      lcd.setCursor(0,1);
      lcd.print("Time: Day");
      lcd.print("                ");
      digitalWrite(RELAY_PIN, HIGH);  // bật bơm
      digitalWrite(LED_PIN, HIGH);
    }else{
      lcd.setCursor(0,1);
      lcd.print("Time: Night");
      digitalWrite(RELAY_PIN, LOW); // tắt bơm
      digitalWrite(LED_PIN, LOW);
    }
  } else {
    digitalWrite(RELAY_PIN, LOW); // tắt bơm
    digitalWrite(LED_PIN, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Soil: Wet");
  }
  delay(1000);
}