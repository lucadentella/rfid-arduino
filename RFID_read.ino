// RFID_Read v1.0
// Luca Dentella, 04/09/2017
// 
// Reads the RFID ID and displays it on a LCD shield

// libraries
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <Adafruit_PN532.h>

// LCD size and object
#define LCD_COLS      16
#define LCD_ROWS      2
hd44780_I2Cexp lcd(0x27, I2Cexp_PCF8574, 0,1,2,4,5,6,7,3,HIGH);

// PN532 breakout board
#define PN532_IRQ     2
#define PN532_RESET   3
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

// speaker
#define SPEAKER_PIN   8
#define TONE_FREQ     400
#define TONE_TIME     200

void setup() {

  // init serial, LCD and NFC
  
  Serial.begin(9600);
  Serial.println("NFC_Read started");
  Serial.println();
  
  if(lcd.begin(LCD_COLS, LCD_ROWS) != 0) {
    Serial.println("- Unable to initialize LCD!");
    while(1);
  }
  lcd.noBacklight();
  lcd.print("Waiting for RFID");
  Serial.println("- LCD initialized");
  
  
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if(!versiondata) {
    Serial.println("- Unable to find a PN532 board");
    while(1);
  }
  Serial.print("- found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX);

  nfc.SAMConfig();
  Serial.println("- chip configured to read RFID tags");
  Serial.println();
  Serial.println("NFC_Read ready!");
}

void loop() {

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  // wait for a NFID tag
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    tone(SPEAKER_PIN, TONE_FREQ, TONE_TIME);
    lcd.clear();
    lcd.print("Found RFID tag!");
    lcd.setCursor(1,2);
    lcd.print("ID: 0x");
    for(int i = 0; i < uidLength; i++) {
      if(uid[i] <= 0xF) lcd.print("0");
      lcd.print(uid[i] & 0xFF, HEX);    
    }
    delay(3000);
    lcd.clear();
    lcd.print("Waiting for RFID");
  }
}
