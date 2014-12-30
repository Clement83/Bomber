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
    if(timout >= 40){
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
  output[1] = slavePlayer.xt; 
  output[2] = PLAYER_Y; 
  output[3] = slavePlayer.yt; 
  output[4] = PLAYER_DROP_BOMB; 
  output[5] = slavePlayer.dropBombe ? '1' : '0'; 
  output[6] = I_AM_DEAD; 
  output[7] = slavePlayer.isAlive? '1' : '0';
  output[8] = I_AM_ON_SCORE_SCREEN; 
  output[9] = isOnScoreScreen? '1' : '0';

  if(paused){
    output[0] = SLAVE_PAUSED;
    output[1] = SLAVE_PAUSED;
    output[2] = SLAVE_PAUSED;
    output[3] = SLAVE_PAUSED;
    output[4] = SLAVE_PAUSED;
    output[5] = SLAVE_PAUSED;
    output[6] = SLAVE_PAUSED;
    output[7] = SLAVE_PAUSED;
    output[8] = SLAVE_PAUSED;
    output[9] = SLAVE_PAUSED;
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
    output[8] = I_AM_MASTER;
    output[9] = I_AM_MASTER;
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
      masterPlayer.xt = Wire.read();
      break;
    case PLAYER_Y:
      masterPlayer.yt = Wire.read();
      break;
    case PLAYER_DROP_BOMB:
      if(Wire.read() == '1')
        DropBombe(masterPlayer.xt*4,masterPlayer.yt*4,masterBombe);
      break;
    case I_AM_DEAD:
      masterPlayer.isAlive = Wire.read() == '1';
      break;
    case MONSTRE1_X: //identifier
      monstre1.xt = Wire.read();
      break;
    case MONSTRE1_Y :
      monstre1.yt = Wire.read();
      break;
    case MONSTRE2_X :
      monstre2.xt = Wire.read();
      break;
    case MONSTRE2_Y :
      monstre2.yt = Wire.read();
      break;
    case MONSTRE1_DROP_BOMB :
      if(Wire.read() == '1')
        DropBombe(monstre1.xt*4,monstre1.yt*4,monstreBombe);
      break;
    case MONSTRE2_DROP_BOMB :
      if(Wire.read() == '1')
        DropBombe(monstre2.xt*4,monstre2.yt*4,monstreBombe);
      break;
    case MONSTRE1_DEAD : 
      monstre1.isAlive = Wire.read() == '1';
    break;
    case MONSTRE2_DEAD : 
      monstre2.isAlive = Wire.read() == '1';
    break;
    case I_AM_ON_SCORE_SCREEN :
      if(Wire.read() == '1') paused = true;
      break;
    default:
      break;
    }
  }
  slave_updated = true;
  disconnected = false;
}








