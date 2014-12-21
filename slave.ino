///////////////////////////////////// SETUP SLAVE
void setupSlave(){
  Wire.begin(2);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

///////////////////////////////////// UPDATE SLAVE
void updateSlave(){
  byte timout = 0;
  paused = false;
  while(slave_updated == false){
    //wait for the master's interrupt
    delay(1);
    timout++;
    if(timout >= 20){
      gb.popup(F("No master"),2);
      disconnected = true;
      paused = true;
      slave_updated = false;
      break;
    }
  }
  slave_updated = false;
}

///////////////////////////////////// SLAVE SEND
// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
char output[SLAVE_DATA_BUFFER_LENGTH];
void requestEvent()
{

  //the slave can only answer with one "write" so you have to put all the variables in an string
  output[0] = PLAYER_X; //identifier
  output[1] = slavePlayer.x; 
  output[2] = PLAYER_Y; 
  output[3] = slavePlayer.y; 
  output[4] = PLAYER_DROP_BOMB; 
  output[5] = slavePlayer.dropBombe ? '1' : '0'; 
  output[6] = I_AM_DEAD; 
  output[7] = slavePlayer.isAlive? '1' : '0';

  if(paused){
    output[0] = SLAVE_PAUSED;
    output[1] = SLAVE_PAUSED;
    output[2] = SLAVE_PAUSED;
    output[3] = SLAVE_PAUSED;
    output[4] = SLAVE_PAUSED;
    output[5] = SLAVE_PAUSED;
    output[6] = SLAVE_PAUSED;
    output[7] = SLAVE_PAUSED;
  }

  if(isMaster){
    output[0] = I_AM_MASTER;
    output[1] = I_AM_MASTER;
    output[2] = I_AM_MASTER;
    output[3] = I_AM_MASTER;
    output[4] = I_AM_MASTER;
    output[5] = I_AM_MASTER;
    output[6] = I_AM_MASTER;
    output[7] = I_AM_MASTER;
  }

  Wire.write(output);
}

///////////////////////////////////// SLAVE RECEIVE
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(Wire.available())    // slave may send less than requested
  {
    char data_in = Wire.read(); // receive byte per byte
    byte numLevelTmp;
    switch(data_in){
   case PLAYER_X:
        masterPlayer.x = Wire.read();
        break;
      case PLAYER_Y:
        masterPlayer.y = Wire.read();
        break;
      case PLAYER_DROP_BOMB:
        if(Wire.read() == '1')
          DropBombe(masterPlayer.x,masterPlayer.y,masterBombe,DIST_EXPLOSION);
        break;
      case I_AM_DEAD:
        masterPlayer.isAlive = Wire.read() == '1';
        break;
      case NUM_LEVEL:
         numLevelTmp= (byte)Wire.read();
        if(currentMaze != numLevelTmp)
        {
          currentMaze = (numLevelTmp - 1);
          loadNextMaze();
        }
        break;
    default:
      break;
    }
  }
  slave_updated = true;
  disconnected = false;
}







