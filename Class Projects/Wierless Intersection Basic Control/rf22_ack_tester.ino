#include <SPI.h>
#include <RH_RF22.h>

const int MAX_SIZE = 30;
const int BOARD_SIZE = 8;

// Singleton instance of the radio driver
RH_RF22 rf22;

int board [BOARD_SIZE][BOARD_SIZE];

int idList [MAX_SIZE];
int directionList [MAX_SIZE];
int robotLocations [MAX_SIZE][2];
int nextLocations [MAX_SIZE][2];
int numberOfIds = 0;
int moveInProgress = 0;
int nextX = 0;
int nextY = 0;

//int moveInProgress = 0;


uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

void setup()
{
  Serial.begin(9600);
  delay(500);
  if (!rf22.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
  rf22.setFrequency(433.0, 0.05);
  rf22.setModemConfig(RH_RF22::GFSK_Rb2_4Fd36);
  rf22.setTxPower(RH_RF22_TXPOW_8DBM);
}

void loop()
{
  Serial.println("Sending to rf22_server");
  // Send a message to rf22_server


    String checksum = String(random(100, 500));
  String id = String(76);
  String toSend = id + "/f/" + checksum;

  toSend.toCharArray(buf,30);
  
  //uint8_t data[] = "Savino_Petit\n";
  rf22.send(buf, sizeof(buf));

  rf22.waitPacketSent();

  delay(3);
  // Now wait for a reply
  char buf[RH_RF22_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf22.waitAvailableTimeout(500))
  {
    // Should be a reply message for us now
    if (rf22.recv(buf, &len))
    {
        
        if (buf[len - 4] == '1' && buf[len - 3] == checksum.charAt(0) &&
            buf[len - 2] == checksum.charAt(1) && buf[len - 1] == checksum.charAt(2)) {
          Serial.println("Acknoweldged Move Request");



          //ackRecieved = 1;

        }
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is rf22_server running?");
  }
  delay(2000);
}





