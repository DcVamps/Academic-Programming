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
char directionList [MAX_SIZE];
int robotLocation [MAX_SIZE][2];
int nextLocation [MAX_SIZE][2];
int numberOfIds = 1;
int moveInProgress = 0;
int nextX = 0;
int nextY = 0;
int currentRobot = 0;
String checksum = "000";


uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

void setup()
{
  for (int i = 0; i  < 10; i++) {
    for (int k = 0; k  < 10; k++) {
      board[i][k] = 0;
    }
  }
  Serial.begin(9600);
  delay(500);
  if (!rf22.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
  rf22.setFrequency(433.0, 0.05);
  rf22.setModemConfig(RH_RF22::GFSK_Rb2_4Fd36);
  rf22.setTxPower(RH_RF22_TXPOW_8DBM);

  robotLocation [0][0] = 2;
  robotLocation [0][1] = 5;

  nextLocation [0][0] = 3;
  nextLocation [0][1] = 5;
  directionList[0] = 'e';
  idList[0] = 6;
  nextX = 3;
  nextY = 5;


  
  robotLocation [1][0] = 5;
  robotLocation [1][1] = 7;

  nextLocation [1][0] = 5;
  nextLocation [1][Y] = 6;
  directionList[1] = 'n';
  idList[1] = 3;

  numberOfIds = 2;
  

  randomSeed(analogRead(0));
}

void loop()
{
  Serial.println("Move in progress: " + String(moveInProgress));
  if (numberOfIds != 0 && moveInProgress == 0)
  {
    Serial.println("Started Move");
    startNextMove();
  }
  else
  {
    if (rf22.waitAvailableTimeout(500))
    {
      rf22.recv(buf, &len);
      rf22.printBuffer("request: ", buf, len);
      Serial.print("got request: ");
      Serial.println((char*)buf);
      delay(3);

      if (len > 3 && ((char*)buf[2] == 'd'  &&
      (char*)buf[4] == '1' && (char*)buf[5] == checksum.charAt(0) &&
      (char*)buf[6] == checksum.charAt(1) && (char*)buf[7] == checksum.charAt(2))){
        Serial.println("Move DONE");
        updateBoardAndLocations();
        incrementRobot();
        moveInProgress = 0;
      }
    }
    else {
      Serial.println("Timed Out");
    }
  }
  delay(20);
}



void startNextMove(void) {

  for (int i = 0; moveInProgress == 0 && i < numberOfIds; i++) {
    if (checkNextMoveValid()) {
      Serial.println("Sending move: ");
      sendMove();
      moveInProgress = 1;
    }
    else {
      Serial.println("Incrementing robot: ");
      incrementRobot();
    }
  }
}

bool checkNextMoveValid(void) {


  Serial.print("Current Id: ");
  Serial.println(idList[currentRobot]);
  Serial.print("nextX: ");
  Serial.println(nextX);
  Serial.print("nextY: ");
  Serial.println(nextY);
  
  Serial.print("nextboard spot");
  Serial.println(board[nextX][nextY]);

  if (moveInProgress == 0) {
    if ( board[nextX][nextY] == 0) {
      return true;
    }

    return false;
  }
}

void incrementRobot(void) {
  currentRobot++;
  Serial.println("robot incremented");
  if (currentRobot > (numberOfIds - 1)) {
    currentRobot = 0;
  }

  Serial.print("Current Robot (in increment): ");
  Serial.println(currentRobot);
  nextX = nextLocation[currentRobot] [X];
  nextY = nextLocation[currentRobot] [Y];
}

void updateBoardAndLocations(void) {

  Serial.print("Current Robot (in update): ");
  Serial.println(currentRobot);

  //clear last loaction on board
  board [robotLocation [currentRobot] [X]] [robotLocation [currentRobot][Y]] = 0;

  //update current location
  robotLocation [currentRobot][X] = nextLocation [currentRobot] [X];
  robotLocation [currentRobot][Y] = nextLocation [currentRobot] [Y];

  nextX = nextLocation [currentRobot] [X];
  nextY = nextLocation [currentRobot] [Y];
  /*
    Serial.print("PRE ");
    Serial.print("nextX: ");
    Serial.println(nextX);
    Serial.print("nextY: ");
    Serial.println(nextY);
  */

  //increment next desired location
  switch (directionList[currentRobot]) {
    case 'n':
      nextY--;
      break;
    case 's':
      nextY++;
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
  int rssi = 0;
  uint8_t data[8];
  uint8_t buf[31];

  checksum = String(random(100, 500));
  String toSend;

  if ((nextX == 4 || nextX == 5) && (nextY == 4 || nextY == 5))
  {
    //need to move twice in the center 4 squares
    toSend = (String(idList[currentRobot]) + "/2/" + checksum);
  }
  else
  {
    toSend = (String(idList[currentRobot]) + "/1/" + checksum);
  }

  toSend.toCharArray(data, 30);

  while (ackRecieved == 0)
  {

    rssi = rf22.rssiRead();
    Serial.print("RSSI: ");
    Serial.println(rssi, DEC);

    if (rssi < 80) {
      Serial.println("Sending data: ");
      Serial.println((char*)data);
      rf22.send(data, sizeof(data));
      rf22.waitPacketSent();
      delay(5);
    }
    else {
      Serial.println("RSSI high skipped send");
      delay(100);
    }




    // Now wait for a reply
    if (rf22.waitAvailableTimeout(500))
    {
      // Should be a reply message for us now
      if (rf22.recv(buf, &len))
      {
        Serial.print("There was a reply: ");
        Serial.println((char*)buf);

        if ((char*)buf[2] == 'a' && (char*)buf[4] == '1' && (char*)buf[5] == checksum.charAt(0) &&
            (char*)buf[6] == checksum.charAt(1) && (char*)buf[7] == checksum.charAt(2))
        {
          Serial.println("Acknowledge Move Request");

          if (nextX > 0 && nextX < 9 && nextY > 0 && nextY < 9) {
            board [nextX] [nextY] = 1;
          }
          else {
            deleteCurrentRobot();
            moveInProgress = 0;
          }

          ackRecieved = 1;
        }
      }
    }
    else
    {

      delay(300);
      delay(random(50, 200));
    }
  }
}

void deleteCurrentRobot() {


  Serial.println("DELELTE");
  Serial.println(currentRobot);
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

  
  
  board [robotLocation[currentRobot][X]] [robotLocation[currentRobot][Y]] = 0;

  if (currentRobot == (numberOfIds - 1)) {

    robotLocation [currentRobot][X] = nextLocation [currentRobot] [X];
    robotLocation [currentRobot][Y] = nextLocation [currentRobot] [Y];
    nextLocation [currentRobot][X] = 0;
    nextLocation [currentRobot][Y] = 0;

  }
  else {
    for (int i = currentRobot; i < (numberOfIds -1); i++ ) {

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




