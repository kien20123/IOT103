


#define BLYNK_TEMPLATE_ID "TMPL6G4WqWpWY"
#define BLYNK_TEMPLATE_NAME "SmartPlant"
#define BLYNK_AUTH_TOKEN "e9L59IGJYHNfYIcPhRv9uyYyAE4LVKZt"


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Rabb1tK";
char pass[] = "Kyenbean2006";

#define SOIL_PIN 34
#define RELAY_PIN 27
#define LIGHT_PIN 5
#define LED_PIN 33
LiquidCrystal_I2C lcd(0x27, 16, 2);

bool autoMode = true;
bool manualPump = false;
int moisture = 1600;
void setup() {
  Serial.begin(115200);
  pinMode(SOIL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  delay(2000);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.print("Connecting");
}

BLYNK_WRITE(V1) {
  manualPump = param.asInt();

  if (!autoMode) {
    digitalWrite(RELAY_PIN, manualPump);
  }
}
BLYNK_WRITE(V2) {
  autoMode = param.asInt();

  if (!autoMode) {
    digitalWrite(RELAY_PIN, manualPump);
  }
}

void sendSensor() {
  int soilValue = analogRead(SOIL_PIN);
  Serial.print(soilValue);
  int maxValue= 2000;
  int dryValue = 1480;
  int wetValue = 1220;
  int minValue= 700;
  int moisturePercent = map(soilValue, maxValue, minValue, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);
  int lightValue = digitalRead(LIGHT_PIN);
  lcd.setCursor(0, 0);
  lcd.print("Moisture:");
  lcd.setCursor(10, 0);
  lcd.print("       ");
  lcd.setCursor(10, 0);
  lcd.print(moisturePercent);
  lcd.print("%   ");
  if (lightValue == 1) {
    lcd.setCursor(0, 1);
    lcd.print("Time: Night");
    digitalWrite(LED_PIN, HIGH);
    if (autoMode) {
      if (soilValue > dryValue) {
        digitalWrite(RELAY_PIN, HIGH);  // bật bơm
      } else if(soilValue < wetValue){
        digitalWrite(RELAY_PIN, LOW);  // tắt bơm
      }
    }
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Time: Day");
    lcd.print("         ");
    digitalWrite(LED_PIN, LOW);
    if (autoMode) {
      digitalWrite(RELAY_PIN, LOW);  // tắt bơm
    }
  }
  Blynk.virtualWrite(V0, moisturePercent);
  Blynk.virtualWrite(V3, lightValue);
  delay(1000);
}
void loop() {

  Blynk.run();
  if (Blynk.connected()) {
    Serial.println("Connected");
  } else {
    Serial.println("Can't connect");
  }
  sendSensor();
}