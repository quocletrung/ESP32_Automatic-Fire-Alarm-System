#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LED_PIN 13
#define FIRE_DETECT_PIN 25
#define SERVO_1_PIN 27
#define SERVO_2_PIN 26
#define MQ_2_PIN 35
#define BUZZER_PIN 33
#define WATER_PUMP 14
#define FAN_PIN 32

Servo servo1;
Servo servo2;

int currentAngle = 60;
bool goingUp = true;
bool isBreak = false;
unsigned long lastUpdate = 0;
const unsigned long interval = 20;  // ms


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FIRE_DETECT_PIN, INPUT);
  pinMode(MQ_2_PIN, INPUT);
  // pinMode(SERVO_1_PIN, INPUT);
  // pinMode(SERVO_2_PIN, INPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(WATER_PUMP, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(115200);

  servo1.attach(SERVO_1_PIN, 500, 2500);
  servo1.write(currentAngle);
  servo2.attach(SERVO_2_PIN, 500, 2500);

  lcd.init();       
  lcd.backlight();  
}

void loop() {
  int khoi_value = analogRead(MQ_2_PIN);
  int fire_value = digitalRead(FIRE_DETECT_PIN);
  Serial.print("gia tri cua lua: ");
  Serial.println(fire_value);
  Serial.print("gia tri cua khoi: ");
  Serial.println(khoi_value);

  if(khoi_value>3500 && fire_value==LOW){
    lcd.setCursor(0, 1);
    lcd.print("DETECT KHOIVALUA");
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(WATER_PUMP, HIGH);
    digitalWrite(FAN_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else if (khoi_value > 3500) {
    lcd.setCursor(0, 1);
    lcd.print("DETECT KHOI!");
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(FAN_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);

  } else if (fire_value == LOW) {
    lcd.setCursor(0, 1);
    lcd.print("DETECT LUA!");
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(WATER_PUMP, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("HETHONGBAOCHAY");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(WATER_PUMP, LOW);
    digitalWrite(FAN_PIN, LOW);
  }

  if (digitalRead(FIRE_DETECT_PIN) == LOW) {
    isBreak = true;
  }

  if (!isBreak) {
    unsigned long now = millis();
    if ((now-lastUpdate)>=interval) {
      lastUpdate = now;

      // cập nhật góc
      if (goingUp) {
        currentAngle += 2;
        if (currentAngle >= 120) {
          goingUp = false;
        }
      } else {
        currentAngle -= 2;
        if (currentAngle <= 60) {
          goingUp = true;
        }
      }
      servo1.write(currentAngle);
    }
  } else {
    isBreak = false;
    servo2.write(currentAngle);
    // goingUp = true;
    delay(200);
  }
}
