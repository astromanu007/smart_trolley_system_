#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// Define RFID pins
#define RST_PIN 9  // Reset pin
#define SS_PIN 10  // Slave select pin

// Create objects for LCD and RFID
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C address might vary (0x27 or 0x3F)
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("Place your RFID card near the reader...");

  // Initialize LCD
  lcd.init();           // Initialize the LCD
  lcd.backlight();      // Turn on the LCD backlight
  lcd.setCursor(0, 0);
  lcd.print("Place RFID Card");

  // Initialize SPI and RFID
  SPI.begin();          // Initialize SPI bus
  rfid.PCD_Init();      // Initialize RFID reader
}

void loop() {
  // Check if a new card is present
  if (!rfid.PICC_IsNewCardPresent()) {
    return; // No new card detected
  }

  // Select the card
  if (!rfid.PICC_ReadCardSerial()) {
    return; // Unable to read the card
  }

  // Clear the LCD and display the card UID
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Card UID:");

  Serial.print("Card UID: ");
  lcd.setCursor(0, 1);

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      Serial.print(" 0");
      lcd.print(" 0");
    } else {
      Serial.print(" ");
      lcd.print(" ");
    }
    Serial.print(rfid.uid.uidByte[i], HEX); // Print UID on Serial Monitor
    lcd.print(rfid.uid.uidByte[i], HEX);   // Print UID on LCD
  }
  Serial.println();

  // Halt the card
  rfid.PICC_HaltA();

  // Pause to let user read the LCD
  delay(2000);

  // Reset LCD to initial message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place RFID Card");
}
