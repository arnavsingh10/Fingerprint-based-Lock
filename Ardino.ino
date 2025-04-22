#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

// Pin Setup
#define IR_SENSOR 2
#define RST_PIN 9
#define SS_PIN 10

// RFID + Fingerprint
MFRC522 rfid(SS_PIN, RST_PIN);
SoftwareSerial fingerSerial(4, 5);  // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Allowed RFID UID
byte allowedUID[4] = {0x61, 0x1A, 0xAD, 0x10};

// Custom Characters
byte lockIcon[8] = {
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};

byte unlockIcon[8] = {
  B00110,
  B00101,
  B00101,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};

void setup() {
  pinMode(IR_SENSOR, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, lockIcon);
  lcd.createChar(1, unlockIcon);

  finger.begin(57600);
  SPI.begin();
  rfid.PCD_Init();

  if (!finger.verifyPassword()) {
    lcd.setCursor(0, 0);
    lcd.print("Fingerprint Fail");
    while (1);
  }

  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1500);
}

void loop() {
  if (digitalRead(IR_SENSOR) == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WELCOME!");
    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("->");
    lcd.write(byte(0)); // 🔒 lock icon
    lcd.print(" Enter");
    lcd.setCursor(0, 1);
    lcd.print("FP or RFID to unlock");

    unsigned long startTime = millis();
    bool attempted = false;
    bool granted = false;

    while (millis() - startTime < 10000) {
      // Fingerprint
      if (finger.getImage() == FINGERPRINT_OK) {
        attempted = true;
        if (finger.image2Tz() == FINGERPRINT_OK && finger.fingerFastSearch() == FINGERPRINT_OK) {
          granted = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fingerprint OK");
          delay(1000);
          unlockAnimation();
          break;
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Access Denied");
          delay(1500);
          break;
        }
      }

      // RFID
      if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        attempted = true;
        granted = checkRFID();
        lcd.clear();
        if (granted) {
          lcd.setCursor(0, 0);
          lcd.print("ID Correct");
          delay(1000);
          unlockAnimation();
        } else {
          lcd.setCursor(0, 0);
          lcd.print("Access Denied");
          delay(1500);
        }
        rfid.PICC_HaltA();
        break;
      }
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WELCOME!");
    delay(1000);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WAITING FOR");
    lcd.setCursor(0, 1);
    lcd.print("SOMEONE...");
  }

  delay(500);
}

void unlockAnimation() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(1));  // 🔓 unlock icon
  lcd.print(" Door Open");
  delay(3000);
}

bool checkRFID() {
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != allowedUID[i]) {
      return false;
    }
  }
  return true;
}
