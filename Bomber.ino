/**
*Bomber V0.1
*Code Clement 
*Art Quirby64 & Clement
*
*/


#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#include <Wire.h>


typedef struct {
  char x, y,xt,yt,nextBombe;
  bool isAlive, isMonster;
}
Player;

typedef struct {
  char x, y;
  byte timer,distExplos;
  bool isAlive,explose;
}
Bombe;


/* fake button for slave*/
boolean bt_up,bt_down,bt_left,bt_right,bt_a,bt_b;

void DropBombe(byte x,byte y, Bombe * bombeArray );
byte getTile(byte x, byte y);
void setTile(byte x, byte y,byte value);
void drawPlayer(Player play,bool isP1);
void updatePlayer(Player *play);
bool TileIsOk(byte x, byte y);
void ExplosionBombe(Bombe *laBombe);
//void TestReactionEnChaineBombe(byte x,byte y);
bool SetTuileExplosion(byte tuileX,byte tuileY);
void updatePlayerAll(Player *play);
void updateMonstre(Player *monstre);
bool MonsterCanDropBombe(Player monster);
void chercherCheminPossible(int * cheminPossible, Player *monstre);
bool caseHavePlayer(char x,char y);
bool caseHaveBombe(char x, char y);
void loadMazeByNumero(int8_t numero);

void pressUp(Player *play);
void pressDown(Player *play);
void pressLeft(Player *play);
void pressRight(Player *play);
void pressA(Player *play);
void pressB(Player *play);

void updateSlavePlayer(Player *play);


//Shake the screen Rodot Code
byte shake_magnitude;
byte shake_timeLeft;

boolean isMaster = false;
boolean paused = true;
boolean disconnected = false;
boolean slave_updated = false;
boolean isSingle;


byte stateGame = 20; //0 les lvl , 20 main menu , 10 scores, 50 game over
int8_t currentLevel = -1;

//multijoueur code
#define SLAVE_PAUSED 1
#define I_AM_MASTER 2

#define STATE_GAME 3


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

#define NUM_LEVEL 50

#define SLAVE_DATA_BUFFER_LENGTH 12

/*SLAVE*/
#define BT_UP 61
#define BT_DOWN 62
#define BT_LEFT 63
#define BT_RIGHT 64
#define BT_A 65
#define BT_B 66


#define TIMER_BOMBE 80;
#define NB_BOMBE 6
#define DIST_EXPLOSION 3//Distance de base de l'explosion
#define NB_FRAME_EXPLOSION 18
#define DIST_VUE_IA 6

//Maze define
#define WIDTH_BLOCK 4
#define HEIGHT_BLOCK 4
#define WIDTH_MAZE 21
#define HEIGHT_MAZE 12
#define NB_MAZE 5

#define BOMBE_NETWORK 70
#define BOMBE_NETWORK_EXP 71
#define BOMBE_DIST_EXP 72
#define BOMBE_X 73
#define BOMBE_Y 74

Player masterPlayer, slavePlayer, monstre1, monstre2;

//tableau des winner
uint8_t winner[NB_MAZE];

Bombe masterBombe[NB_BOMBE];
Bombe slaveBombe[NB_BOMBE];
Bombe monstreBombe[NB_BOMBE];

byte distExplosion;
const char strSigle[] PROGMEM = "Sigle player";
const char strMaster[] PROGMEM = "Host (master)";
const char strSlave[] PROGMEM = "Join (slave)";
const char* const menu[3] PROGMEM = {
  strSigle,
  strMaster,
  strSlave
};

void setup()
{
  gb.begin();
  goTitleScreen();
  gb.battery.show = false;
  //can be either master or slave:
  setupMaster();
  setupSlave();
}
void initScore()
{
  for(byte i=0;i<NB_MAZE;i++)
  {
    winner[i] = 0;
  }
}

int8_t timerAnimEndGame =0;
void resetGame()
{
  currentLevel = -1;
  stateGame = 20;
}
void initGame()
{
  timerAnimEndGame =0;
  P1StartPos();
  P2StartPos();
  M1StartPos();
  M2StartPos();
  distExplosion = DIST_EXPLOSION;

  for(byte i=0;i<NB_BOMBE;i++)
  {
    masterBombe[i].isAlive = false;
    slaveBombe[i].isAlive = false;
    monstreBombe[i].isAlive = false;
  }
  
}

void loop(){

  if(gb.update())
  {
    if(stateGame == 20)
    {
      paused = true;
      switch(gb.menu(menu, 3)){
        case 0: //sigle player
          paused = false;
          disconnected = true;
          isMaster = true;
          isSingle = true;
          stateGame=1;
          break;
        case 1: //Host
          paused = false;
          disconnected = false;
          isMaster = true;
          isSingle = false;
          stateGame=1;
          break;
        case 2: //Join
          paused = false;
          disconnected = false;
          isMaster = false;
          isSingle = false;
          break;
        default:
          goTitleScreen();
          break;
      }
    }
    else if(stateGame==0)
    {
        if(gb.buttons.pressed(BTN_C))
        {
          stateGame = 51;
        }
  
        
        if(isMaster)
        {
          updatePlayer(&masterPlayer);
          updateMonstres();
          if(!isSingle) 
          {
            //updatePlayer(&slavePlayer);
            updateMaster();
            updatePlayerAll(&slavePlayer);
          }
          updatePlayerAll(&masterPlayer);
          updatePlayerAll(&monstre1);
          updatePlayerAll(&monstre2);
          UpdateBombes();
        }
        else
        {
          if(!isSingle){
            updateSlavePlayer(&slavePlayer);           
            updateSlave();
          }
        }
  
  
      DrawMaze();
      DrawPlayers();
      DrawBombes();
    
      if(!masterPlayer.isAlive ||  !slavePlayer.isAlive || (isSingle && !monstre1.isAlive &&  !monstre2.isAlive))
      {
        if(masterPlayer.isAlive)
        {
          winner[currentLevel] = 1;
        }
        else if(!isSingle && slavePlayer.isAlive)
        {
          winner[currentLevel] = 2;
        }
        else if(monstre1.isAlive && monstre2.isAlive)
        {
          winner[currentLevel] = random(3,5);
        }
        else if(monstre1.isAlive)
        {
          winner[currentLevel] = 3;
        }
        else if(monstre2.isAlive)
        {
          winner[currentLevel] = 4;
        }
        stateGame = 10;
      }
    }
    else if(stateGame == 11)
    {
      updateNetwork();
      ScoreScreen();
    }
    else if(stateGame == 10)
    {
      updateNetwork();
      endGame();
    }
    else if(stateGame == 12)
    {
      updateNetwork();
      endGame();
    }
    else if(stateGame == 1)
    {
      updateNetwork();
      if(isMaster)
      {
        initGame();
        stateGame = 0;
        currentLevel++;
        loadMazeByNumero(currentLevel);
      }
    }
    else if(stateGame == 50)
    {
      updateNetwork();
      gameOverScreen();
    }
    else if(stateGame == 51)
    {
      updateNetwork();
      goTitleScreen();
    }
  }
}

void updateNetwork()
{
  if(!isSingle)
  {
    if(isMaster)
    {
       updateMaster();
    }
    else
    {
      updateSlave();
    }
  }
}

const uint8_t soundfx[2][8] = {
    {1,4,113,10,7,19,7,52}, // Explosion{1,0,55,12,7,12,7,13}
    {0,19,9,1,7,15,7,4}, // Drop bombe
};

void playsoundfx(uint8_t fxno, uint8_t channel) {
    gb.sound.command(0,soundfx[fxno][6],0,channel);
    // set volume
    gb.sound.command(1,soundfx[fxno][0],0,channel);
    // set waveform
    gb.sound.command(2,soundfx[fxno][5],-soundfx[fxno][4],channel);
    // set volume slide
    gb.sound.command(3,soundfx[fxno][3],soundfx[fxno][2]-58,channel);
    // set pitch slide
    gb.sound.playNote(soundfx[fxno][1],soundfx[fxno][7],channel);
    // play note
}

















































