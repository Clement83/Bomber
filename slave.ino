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
  //gb.popup(F("send data SL"),20);
  //the slave can only answer with one "write" so you have to put all the variables in an string
  output[0] = BT_UP; //identifier
  output[1] = bt_up? '1' : '0';
  output[2] = BT_DOWN; 
  output[3] = bt_down? '1' : '0'; 
  output[4] = BT_LEFT; 
  output[5] = bt_left? '1' : '0';
  output[6] = BT_RIGHT; 
  output[7] = bt_right? '1' : '0';
  output[8] = BT_A; 
  output[9] = bt_a? '1' : '0';
  output[10] = BT_B; 
  output[11] = bt_b? '1' : '0';

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
    output[10] = SLAVE_PAUSED;
    output[11] = SLAVE_PAUSED;
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
    output[10] = I_AM_MASTER;
    output[11] = I_AM_MASTER;
  }
  
  bt_up = false;
  bt_down = false;
  bt_left = false;
  bt_right = false;
  bt_a = false;
  bt_b = false;

  Wire.write(output);
}

///////////////////////////////////// SLAVE RECEIVE
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{  
  byte tmpData;
  byte lastDistExplosion;
  byte lastX,lastY;
  
  while(Wire.available())    // slave may send less than requested
  {
    //
    char data_in = Wire.read(); // receive byte per byte
    byte numLevelTmp;
    switch(data_in){
      case PLAYER1_X:
        masterPlayer.x = Wire.read();
        break;
      case PLAYER1_Y:
        masterPlayer.y = Wire.read();
        break;
      case PLAYER1_IS_ALIVE:
        masterPlayer.isAlive = Wire.read() == '1';    
        break;
     
      case PLAYER2_X:
        slavePlayer.x = Wire.read();
        break;
      case PLAYER2_Y:
        slavePlayer.y = Wire.read();
        break;
      case PLAYER2_IS_ALIVE:
        slavePlayer.isAlive = Wire.read() == '1';    
        break;
     
      case MONSTRE1_X:
        monstre1.x = Wire.read();
        break;
      case MONSTRE1_Y:
        monstre1.y = Wire.read();
        break;
      case MONSTRE1_IS_ALIVE:
        monstre1.isAlive = Wire.read() == '1';    
        break;
     
      case MONSTRE2_X:
        monstre2.x = Wire.read();
        break;
      case MONSTRE2_Y:
        monstre2.y = Wire.read();
        break;
      case MONSTRE2_IS_ALIVE:
        monstre2.isAlive = Wire.read() == '1';    
        break;
   
   
     case STATE_GAME :
        stateGame = Wire.read();    
        break;
     case NUM_LEVEL :
        tmpData = Wire.read();
        if(currentLevel != tmpData){
          currentLevel = tmpData;    
          loadMazeByNumero(currentLevel);
        }
        break;
    case BOMBE_DIST_EXP : 
      lastDistExplosion = Wire.read();
    break;
    case BOMBE_NETWORK :
        //0->9 bombe P1
        //10->19 bombe P2
        //20->29 bombe IA

        gb.popup(F("Get a bombe"),5);
        Bombe * laBombe1;
        tmpData = (byte)Wire.read();
        if(tmpData<10)
        {
          laBombe1 = &masterBombe[tmpData];
        }
        else if(tmpData<20)
        {
          tmpData = tmpData - 10;
           laBombe1 = &slaveBombe[tmpData];
        }
        else if(tmpData<30)
        {
          tmpData = tmpData - 20;
           laBombe1 = &monstreBombe[tmpData];
        }
        laBombe1->timer = TIMER_BOMBE;
        laBombe1->isAlive = true;
        laBombe1->x = lastX;
        laBombe1->y = lastY;
    break;
    
    case BOMBE_NETWORK_EXP :
        //0->9 bombe P1
        //10->19 bombe P2
        //20->29 bombe IA
        gb.popup(F("boom"),5);
        tmpData = (byte)Wire.read();
        Bombe * laBombe;
        if(tmpData<10)
        {
          laBombe = &masterBombe[tmpData];
        }
        else if(tmpData<20)
        {
          tmpData = tmpData - 10;
           laBombe = &slaveBombe[tmpData];
        }
        else if(tmpData<30)
        {
          tmpData = tmpData - 20;
           laBombe = &monstreBombe[tmpData];
        }
        laBombe->isAlive = false;
        laBombe->distExplos = lastDistExplosion;
        ExplosionBombe(laBombe);

    break;
    
    case BOMBE_X : 
      lastX = Wire.read();
      break;
    case BOMBE_Y : 
      lastY = Wire.read();
      break;
        
      default:
        break;
    }
  }
  slave_updated = true;
  disconnected = false;
}








