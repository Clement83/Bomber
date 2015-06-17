///////////////////////////////////// SETUP MASTER
void setupMaster(){
  Wire.begin();
}

///////////////////////////////////// UPDATE MASTER
void updateMaster(){
  //read from slave
  masterRead();
  masterWrite();
}

///////////////////////////////////// MASTER READ
void masterRead(){
  if(Wire.requestFrom(2, SLAVE_DATA_BUFFER_LENGTH)){    // request 6 bytes from slave device #2
    disconnected = false;
    paused = false;
    while(Wire.available()){    // slave may send less than requested
      char data_in = Wire.read(); // receive byte per byte
      switch(data_in){
      case BT_UP:
        if(Wire.read() == '1')
          pressUp(&slavePlayer);
        break;
      case BT_DOWN:
        if(Wire.read() == '1')
          pressDown(&slavePlayer);
      break;
      case BT_LEFT:
        if(Wire.read() == '1')
          pressLeft(&slavePlayer);
      break;
      case BT_RIGHT:
        if(Wire.read() == '1')
          pressRight(&slavePlayer);
      break;
      case BT_A:
        if(Wire.read() == '1')
          pressA(&slavePlayer);
      break;
      case BT_B:
        if(Wire.read() == '1')
          pressB(&slavePlayer);
      break;
      case SLAVE_PAUSED:
        gb.popup(F("Slave paused"),2);
        paused = true;
        break;
      case I_AM_MASTER:
        gb.popup(F("1 master max"),2);
        paused = true;
        break;
      default:
        //gb.popup(F("Wrong slave data"),2);
        //wrongSlaveData = data_in;
        //paused = true;
        break;
      }
    }
  }
  else{
    gb.popup(F("Slave disconnected"),2);
    paused = true;
    disconnected = true;
  }
}

///////////////////////////////////// MASTER WRITE
void masterWrite(){
  // Player masterPlayer, slavePlayer, monstre1, monstre2;
  /*
  Bombe masterBombe[NB_BOMBE];
Bombe slaveBombe[NB_BOMBE];

Bombe monstreBombe[NB_BOMBE];

#define PLAYER1_X 10
#define PLAYER1_Y 11
#define PLAYER1_IS_ALIVE 12
#define PLAYER2_X 20
#define PLAYER2_Y 21
#define PLAYER2_IS_ALIVE 22
#define MONSTRE1_X 30
#define MONSTRE1_Y 31
#define MONSTRE1_IS_ALIVE 32
#define MONSTRE2_X 40
#define MONSTRE2_Y 41
#define MONSTRE2_IS_ALIVE 42

  */
  
  Wire.beginTransmission(2); // transmit to device #2
  for(byte i=0;i<NB_BOMBE;i++)
  {
    if(masterBombe[i].isAlive && masterBombe[i].timer%20>5)//1/4 du temps eteint
    {
      Wire.write(BOMBE_X); 
      Wire.write(masterBombe[i].x);
      Wire.write(BOMBE_Y); 
      Wire.write(masterBombe[i].y);
      Wire.write(BOMBE_NETWORK); 
      Wire.write(i);
    }
    if(masterBombe[i].explose)
    {
      masterBombe[i].explose = false;
      
      Wire.write(BOMBE_DIST_EXP); 
      Wire.write(masterBombe[i].distExplos);
      Wire.write(BOMBE_NETWORK_EXP); 
      Wire.write(i);
      
    }

    if(slaveBombe[i].isAlive && masterBombe[i].timer%20>5)//1/4 du temps eteint
    {
      Wire.write(BOMBE_X); 
      Wire.write(slaveBombe[i].x);
      Wire.write(BOMBE_Y); 
      Wire.write(slaveBombe[i].y);
      Wire.write(BOMBE_NETWORK); 
      Wire.write((10+i));
    }
    if(slaveBombe[i].explose)
    {
      slaveBombe[i].explose = false;
      
      Wire.write(BOMBE_DIST_EXP); 
      Wire.write(slaveBombe[i].distExplos);
      Wire.write(BOMBE_NETWORK_EXP); 
      Wire.write((10+i));
      
    }

    if(monstreBombe[i].isAlive && masterBombe[i].timer%20>5)//1/4 du temps eteint
    {
      Wire.write(BOMBE_X); 
      Wire.write(monstreBombe[i].x);
      Wire.write(BOMBE_Y); 
      Wire.write(monstreBombe[i].y);
      Wire.write(BOMBE_NETWORK); 
      Wire.write((20+i));
    }
    if(monstreBombe[i].explose)
    {
      monstreBombe[i].explose = false;
      
      Wire.write(BOMBE_DIST_EXP); 
      Wire.write(monstreBombe[i].distExplos);
      Wire.write(BOMBE_NETWORK_EXP); 
      Wire.write((20 + i));
      
    }
    //Wire.endTransmission(); // stop transmitting
  }
    
 //Wire.endTransmission(); // stop transmitting
  
 //Wire.beginTransmission(2); // transmit to device #2
  
  //game state
  Wire.write(STATE_GAME); 
  Wire.write(stateGame);
  Wire.write(NUM_LEVEL); 
  Wire.write(currentLevel); 
 
  
  
  Wire.write(PLAYER1_X); 
  Wire.write(masterPlayer.x); 
  Wire.write(PLAYER1_Y); 
  Wire.write(masterPlayer.y); 
  Wire.write(PLAYER1_IS_ALIVE); 
  Wire.write(masterPlayer.isAlive? '1' : '0');
  
  
  Wire.write(PLAYER2_X); 
  Wire.write(slavePlayer.x); 
  Wire.write(PLAYER2_Y); 
  Wire.write(slavePlayer.y); 
  Wire.write(PLAYER2_IS_ALIVE); 
  Wire.write(slavePlayer.isAlive? '1' : '0');
  
  
  Wire.write(MONSTRE1_X); 
  Wire.write(monstre1.x); 
  Wire.write(MONSTRE1_Y); 
  Wire.write(monstre1.y); 
  Wire.write(MONSTRE1_IS_ALIVE); 
  Wire.write(monstre1.isAlive? '1' : '0');
  
  
  Wire.write(MONSTRE2_X); 
  Wire.write(monstre2.x); 
  Wire.write(MONSTRE2_Y); 
  Wire.write(monstre2.y); 
  Wire.write(MONSTRE2_IS_ALIVE); 
  Wire.write(monstre2.isAlive? '1' : '0');
  Wire.endTransmission();    // stop transmitting
  
  
     
    
}






