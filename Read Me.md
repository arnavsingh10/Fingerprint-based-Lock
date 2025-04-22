Hardware Used
Arduino Uno / Nano

IR Sensor

MFRC522 RFID Module

Fingerprint Sensor (Adafruit / R305)

I2C LCD Display (16x2)

Buzzer or Lock (optional for real-world use)

Jumper Wires, Breadboard


Pin Connections

Component	Arduino Pin
IR Sensor	D2
RFID RST	D9
RFID SS (SDA)	D10
Fingerprint RX	D4
Fingerprint TX	D5
LCD SDA	A4
LCD SCL	A5





Libraries Required
Make sure to install these libraries from the Library Manager in the Arduino IDE:

Adafruit Fingerprint Sensor Library

MFRC522 by Miguel Balboa

LiquidCrystal_I2C

Wire

SPI

SoftwareSerial



How It Works
When someone is detected via the IR sensor, the system prompts them to scan a fingerprint or an RFID card.

If the fingerprint matches a stored fingerprint OR the RFID UID matches the allowed UID, access is granted.

A door unlock animation (with a custom icon) is shown on the LCD.

If the verification fails or times out after 10 seconds, access is denied.

The system goes back to waiting for someone.




byte allowedUID[4] = {0x61, 0x1A, 0xAD, 0x10};


Author
Arnav Singh –  MPCA Project
