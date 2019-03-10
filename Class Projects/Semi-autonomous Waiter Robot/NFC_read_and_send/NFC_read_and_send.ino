#include <Adafruit_PN532.h>
#include <Wire.h>
#include <SPI.h>

#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

int BDY_PIN = 12;    //black wire
int DAT_PIN_0 = 11;  //brown wire
int DAT_PIN_1 = 10;  //red wire
int DAT_PIN_2 = 9;   //orange wire
int DAT_PIN_3 = 8;   //yellow wire


Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

#if defined(ARDUINO_ARCH_SAMD)
   #define Serial SerialUSB
#endif

void setup() {
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(115200);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  pinMode(BDY_PIN, OUTPUT);
  pinMode(DAT_PIN_0, OUTPUT);
  pinMode(DAT_PIN_1, OUTPUT);
  pinMode(DAT_PIN_2, OUTPUT);
  pinMode(DAT_PIN_3, OUTPUT);

  digitalWrite(BDY_PIN, LOW);
  digitalWrite(DAT_PIN_0, LOW);
  digitalWrite(DAT_PIN_1, LOW);
  digitalWrite(DAT_PIN_2, LOW);
  digitalWrite(DAT_PIN_3, LOW);
  
}

void loop() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  String readData;
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    if (uidLength == 4) {
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);

      if (success) {
        uint8_t data[16];
        success = nfc.mifareclassic_ReadDataBlock(4,data);
        if (success){
          readData = nfc.PrintChar(data,16);
          Serial.println(readData);
        }
      }
    }
  }

  if(readData == "FLR_K"){
    Serial.println("01111");
    digitalWrite(DAT_PIN_0, HIGH);
    digitalWrite(DAT_PIN_2, HIGH);
    digitalWrite(DAT_PIN_2, HIGH);
    digitalWrite(DAT_PIN_3, HIGH);
    delay(1000);
    digitalWrite(DAT_PIN_0, LOW);
    digitalWrite(DAT_PIN_1, LOW);
    digitalWrite(DAT_PIN_2, LOW);
    digitalWrite(DAT_PIN_3, LOW);
  }
  else if(readData == "BDY"){
    Serial.println("10000");
    digitalWrite(BDY_PIN, HIGH);
    delay(1000);
    digitalWrite(BDY_PIN, LOW);
  }
  else if(readData == "FLR_1" || readData == "CAL_1"){
    Serial.println("00001");
    digitalWrite(DAT_PIN_0, HIGH);
    delay(1000);
    digitalWrite(DAT_PIN_0, LOW);
  }
  else if(readData == "FLR_2" || readData == "CAL_2"){
    Serial.println("00011");
    digitalWrite(DAT_PIN_0, HIGH);
    digitalWrite(DAT_PIN_1, HIGH);
    delay(1000);
    digitalWrite(DAT_PIN_0, LOW);
    digitalWrite(DAT_PIN_1, LOW);
  }
  else if(readData == "FLR_3" || readData == "CAL_3"){
    Serial.println("00010");
    digitalWrite(DAT_PIN_1, HIGH);
    delay(1000);
    digitalWrite(DAT_PIN_1, LOW);
  }
  else if(readData == "FLR_4" || readData == "CAL_4"){
    Serial.println("00110");
    digitalWrite(DAT_PIN_2, HIGH);
    digitalWrite(DAT_PIN_1, HIGH);
    delay(1000);
    digitalWrite(DAT_PIN_2, LOW);
    digitalWrite(DAT_PIN_1, LOW);
  }
  else if(readData == "FLR_5" || readData == "CAL_5"){
    Serial.println("00111");
    digitalWrite(DAT_PIN_2, HIGH);
    digitalWrite(DAT_PIN_1, HIGH);
    digitalWrite(DAT_PIN_0, HIGH);
    delay(1000);
    digitalWrite(DAT_PIN_2, LOW);
    digitalWrite(DAT_PIN_1, LOW);
    digitalWrite(DAT_PIN_0, LOW);
  }
  else if(readData == "FLR_6" || readData == "CAL_6"){
    Serial.println("00101");
    digitalWrite(DAT_PIN_2, HIGH);
    digitalWrite(DAT_PIN_0, HIGH);
    delay(1000);
    digitalWrite(DAT_PIN_2, LOW);
    digitalWrite(DAT_PIN_0, LOW);
  }
  /*&else{
    Serial.println("00000");
    digitalWrite(BDY_PIN, LOW);
    digitalWrite(DAT_PIN_0, LOW);
    digitalWrite(DAT_PIN_1, LOW);
    digitalWrite(DAT_PIN_2, LOW);
    digitalWrite(DAT_PIN_3, LOW);
  }*/
  delay(250);
}
