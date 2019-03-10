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
int TX_PIN = 7;
int RX_PIN = 9;
int id_number = 3
;

uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

void setup()
{
  //Setup pin modes
  pinMode(TX_PIN, OUTPUT);
  pinMode(RX_PIN, INPUT);

  digitalWrite(TX_PIN, 0);

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
  Serial.println("Waiting for message from server");

  char buf[RH_RF22_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf22.waitAvailableTimeout(500))
  {
    // Wait for instructions
    if (rf22.recv(buf, &len))
    {
      Serial.println("Data Recived: ");
      Serial.println((char*)buf);

      String stringData = (char*)buf;
      String passData = (char*)buf;
      Serial.println("ID: ");
      Serial.println(stringData.substring(0, 1));

      if (stringData.substring(0, 1).toInt() == id_number) {
        Serial.println("Move Blocks: ");
        Serial.println(stringData.substring(2, 3));

        Serial.println("Checksum: ");
        Serial.println("1" + stringData.substring(4, 7));

        Serial.println("Sending Acknowledment");

        passData = stringData.substring(0, 1) + "/" + stringData.substring(2, 3) + "/1" + stringData.substring(4, 7);

        stringData = stringData.substring(0, 1) + "/a/1" + stringData.substring(4, 7);

        uint8_t dataSent[stringData.length() + 2];
        stringData.toCharArray(dataSent, stringData.length() + 2);

        Serial.println((char*)dataSent);

        rf22.send(dataSent, sizeof(dataSent));

        rf22.waitPacketSent();

        delay(3);

        Serial.println("Data sent");

        doneMoving(passData);
      }
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("Nothing recieved, is rf22_server running?");
  }
  delay(2000);
}
void doneMoving(String stringData) {
  Serial.println("In Done");
  Serial.println(stringData);

  //String stringData = (char*)buf;
  String stringDataSent = stringData.substring(0, 1) + "/d/" + stringData.substring(4, 8);
  uint8_t dataSent[stringData.length() + 1];
  stringDataSent.toCharArray(dataSent, stringDataSent.length() + 1);

  Serial.println("We need to move x blocks:");
  int moveBlocks = stringData.substring(2, 3).toInt();
  int blocksMoved = 0;

  Serial.println(moveBlocks);

  Serial.println("Moving Robot");

  //Waiting for robot to move every block
  while (blocksMoved < moveBlocks) {
    Serial.println("Moving robot one block");
    sendMove();

    int confirmed = 0;

    while (confirmed != 1) {
      confirmed = 0;
      Serial.println("Waiting for robot to move");
      confirmed = confirmMove();
    }

    blocksMoved++;
    delay(500);
  }

  Serial.println("Done Moving Robot");

  Serial.println("Sending Done: ");

  Serial.println((char*)dataSent);

  while (rf22.rssiRead() < 80) {
    delay(5);
  }

  rf22.send(dataSent, sizeof(dataSent));

  rf22.waitPacketSent();

  delay(3);

  rf22.waitAvailable();

  Serial.println("New data found, continuing");

}

//Lets the robot know we need to move one block
void sendMove(void) {
  //Writing to TX
  digitalWrite(TX_PIN, 1);

  Serial.println("Transmitting");

  delay(200);
}

//Confirmation from the robot that we moved one block
int confirmMove(void) {
  int x1 = 0;

  int value = 0;

  while (x1 < 3) {
    //Checking Recieve
    value = digitalRead(RX_PIN);

    Serial.print("Recieving: ");
    Serial.println(value);

    if (value == 1) {
      //Writing to TX
      digitalWrite(TX_PIN, 0);
      return 1;
    }

    delay(1000);
    x1++;
  }

  return 0;
}


