#include <SPI.h>
#include <RH_RF22.h>

const int MAX_SIZE = 30;
const int BOARD_SIZE = 10;
const int X = 0;
const int Y = 1;

// Singleton instance of the radio driver
RH_RF22 rf22;

int board [BOARD_SIZE][BOARD_SIZE];

int idList [MAX_SIZE];
int directionList [MAX_SIZE];
int robotLocation [MAX_SIZE][2];
int nextLocation [MAX_SIZE][2];
int numberOfIds = 0;
int moveInProgress = 0;
int nextX = 0;
int nextY = 0;
int currentRobot = 0;


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
  if (moveInProgress == 0) {
    startNextMove();
  }
  else {

    rf22.waitAvailable();

    rf22.recv(buf, &len);
    rf22.printBuffer("request: ", buf, len);
    Serial.print("got request: ");
    Serial.println((char*)buf);
    delay(3);

    if (len > 3 && buf[3] == 'd') {
      updateBoardAndLocations();
      incrementRobot();
      moveInProgress = 0;
    }
  }
  delay(20);
}



void startNextMove(void) {

  for (int i = 0; moveInProgress == 0 || i > numberOfIds; i++) {
    if (checkNextMoveValid()) {
      sendMove();
      moveInProgress = 1;
    }
    else {
      incrementRobot();
    }
  }
}

bool checkNextMoveValid(void) {

  if (moveInProgress == 0) {
    if ( board[nextX][nextY] == 0) {
      return true;
    }

    return false;
  }
}

void incrementRobot(void) {
  currentRobot++;
  if (currentRobot > numberOfIds) {
    currentRobot = 0;
  }

  nextX = nextLocation[currentRobot] [X];
  nextY = nextLocation[currentRobot] [Y];
}

void updateBoardAndLocations(void) {

  //clear last loaction on board
  board [robotLocation [currentRobot] [X]] [robotLocation [currentRobot][Y]] = 0;

  //update current location
  robotLocation [currentRobot][X] = nextLocation [currentRobot] [X];
  robotLocation [currentRobot][Y] = nextLocation [currentRobot] [Y];

  int nextX = nextLocation [currentRobot] [X];
  int nextY = nextLocation [currentRobot] [Y];


  //increment next desired location
  switch (directionList[currentRobot]) {
    case 'n':
      nextY++;
      break;
    case 's':
      nextY--;
      break;
    case 'e':
      nextX++;
      break;
    case 'w':
      nextX--;
      break;
  }

  nextLocation [currentRobot][X] = nextX;
  nextLocation [currentRobot][Y] = nextY;
}




void sendMove(void) {
  int ackRecieved = 0;
  uint8_t data[8];
  uint8_t buf[31];

  String checksum = String(random(100, 500));
  String toSend;

  if((nextX == 4 || nextX ==5) && (nextY == 4 || nextY = 5)) {
    //need to move twice in the center 4 squares 
    toSend= (String(idList[currentRobot]) + "/2/" + checksum);
  }
  else{
    toSend= (String(idList[currentRobot]) + "/1/" + checksum);
  }
  
  toSend.toCharArray(data,30);

  while (ackRecieved == 0) {
                         
    rf22.send(data, sizeof(data));

    rf22.waitPacketSent();
    delay(3);

    // Now wait for a reply
    if (rf22.waitAvailableTimeout(500))
    {
      // Should be a reply message for us now
      if (rf22.recv(buf, &len))
      {
        if (buf[3] == '1' && buf[4] == checksum.charAt(0) &&
            buf[5] == checksum.charAt(1) && buf[6] == checksum.charAt(2)) {
          Serial.println("Acknoweldged Move Request");

          if (nextX > 0 && nextX < 9 && nextY > 0 && nextY < 9) {
            deleteCurrentRobot();
            moveInProgress = 0;
          }
          else {
            board [nextX] [nextY] = 1;
          }

          ackRecieved = 1;

        }
      }
    }
    else {
      Serial.println("retry send");
      delay(random(50, 200));
    }
  }
}

void deleteCurrentRobot() {
  board [robotLocation[currentRobot][X]] [robotLocation[currentRobot][Y]] = 0;

  if (currentRobot == numberOfIds) {

    robotLocation [currentRobot][X] = nextLocation [currentRobot] [X];
    robotLocation [currentRobot][Y] = nextLocation [currentRobot] [Y];
    nextLocation [currentRobot][X] = 0;
    nextLocation [currentRobot][Y] = 0;

  }
  else {
    for (int i = currentRobot; i < numberOfIds; i++ ) {

      directionList [i] = directionList [i + 1];
      idList [i] = idList [i + 1];
      robotLocation [i] [X] = robotLocation [i + 1] [X];
      robotLocation [i] [Y] = robotLocation [i + 1] [Y];
      nextLocation [i] [X] = nextLocation [i + 1] [X];
      nextLocation [i] [Y] = nextLocation [i + 1] [Y];
    }
  }
  numberOfIds--;
}




