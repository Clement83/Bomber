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
      case PLAYER_X:
        slavePlayer.xt = Wire.read();
        break;
      case PLAYER_Y:
        slavePlayer.yt = Wire.read();
        break;
      case PLAYER_DROP_BOMB:
        if(Wire.read() == '1')
          DropBombe(slavePlayer.xt*4,slavePlayer.yt*4,slaveBombe);
        break;
      case I_AM_DEAD:
        slavePlayer.isAlive = Wire.read() == '1';
        break;
      case SLAVE_PAUSED:
        //gb.popup(F("Slave paused"),2);
        paused = true;
        break;
      case I_AM_MASTER:
        gb.popup(F("1 master max"),2);
        paused = true;
        break;
      case I_AM_ON_SCORE_SCREEN :
      if(Wire.read() == '1')
      {
        paused = true;
        //gb.popup(F("Slave Score screen"),2);
      }
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
  Wire.beginTransmission(2); // transmit to device #2
  Wire.write(PLAYER_X); //identifier
  Wire.write(masterPlayer.xt); //identifier
  Wire.write(PLAYER_Y); //identifier
  Wire.write(masterPlayer.yt); //identifier
  Wire.write(PLAYER_DROP_BOMB); //identifier
  Wire.write(masterPlayer.dropBombe ? '1' : '0'); //identifier
  Wire.write(I_AM_DEAD); //identifier
  Wire.write(masterPlayer.isAlive? '1' : '0'); //identifier
  Wire.write(I_AM_ON_SCORE_SCREEN); //identifier
  Wire.write(isOnScoreScreen? '1' : '0'); //identifier
  
  //Gestion Monstre
  Wire.write(MONSTRE1_X); //identifier
  Wire.write(monstre1.xt); //identifier
  Wire.write(MONSTRE1_Y); //identifier
  Wire.write(monstre1.yt); //identifier
  Wire.write(MONSTRE2_X); //identifier
  Wire.write(monstre2.xt); //identifier
  Wire.write(MONSTRE2_Y); //identifier
  Wire.write(monstre2.yt); //identifier
  Wire.write(MONSTRE1_DROP_BOMB); //identifier
  Wire.write(monstre1.dropBombe ? '1' : '0'); //identifier
  Wire.write(MONSTRE2_DROP_BOMB); //identifier
  Wire.write(monstre2.dropBombe ? '1' : '0'); //identifier
  Wire.write(MONSTRE1_DEAD); //identifier
  Wire.write(monstre1.isAlive? '1' : '0'); //identifier
  Wire.write(MONSTRE2_DEAD); //identifier
  Wire.write(monstre2.isAlive? '1' : '0'); //identifier
  
  Wire.endTransmission();    // stop transmitting
}






