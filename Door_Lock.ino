#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 53
#define RST_PIN 5

String UID = "05D07F22"; //ID of registred card
byte lock = 0;

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  servo.attach(9);
  servo.write(25);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("RFID Door Lock Initialized.");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String ID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
      ID.concat(String(rfid.uid.uidByte[i], HEX));
      ID.toUpperCase();
    }
    Serial.print("Card UID: ");
    Serial.println(ID);

    if (ID == UID && lock == 0) {
      Serial.println("Opening door...");
      servo.write(135);
      delay(1000);
      digitalWrite(10, HIGH);
      lock = 1;
    } else if (ID == UID && lock == 1) {
      Serial.println("Closing door...");
      servo.write(25);
      delay(1000);
      digitalWrite(10, LOW);
      lock = 0;
    }
    delay(500);
  }
}
