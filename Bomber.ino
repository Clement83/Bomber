#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#include <Wire.h>


typedef struct {
  char x, y,xt,yt,nextBombe;
  bool isAlive, dropBombe, isMonster;
}
Player;

typedef struct {
  char x, y;
  byte timer,distExplos;
  bool isAlive;
}
Bombe;


void DropBombe(byte x,byte y, Bombe * bombeArray );
byte getTile(byte x, byte y);
void setTile(byte x, byte y,byte value);
void drawPlayer(Player play,bool isP1);
void updatePlayer(Player *play);
bool TileIsOk(byte x, byte y);
void ExplosionBombe(Bombe laBombe);
void TestReactionEnChaineBombe(byte x,byte y);
bool SetTuileExplosion(byte tuileX,byte tuileY);
void updatePlayerAll(Player *play);
void updateMonstre(Player *monstre);
bool MonsterCanDropBombe(Player monster);
void chercherCheminPossible(int * cheminPossible, Player *monstre);
bool caseHavePlayer(char x,char y);
bool caseHaveBombe(char x, char y);
void loadMazeByNumero(int8_t numero);

//Shake the screen Rodot Code
byte shake_magnitude;
byte shake_timeLeft;

boolean isMaster = false;
boolean paused = true;
boolean disconnected = false;
boolean slave_updated = false;
boolean isSingle;
boolean isOnScoreScreen = false;


//multijoueur code
#define SLAVE_PAUSED 1
#define I_AM_MASTER 2
#define I_AM_ON_SCORE_SCREEN 3
#define PLAYER_X 10
#define PLAYER_Y 11
#define MONSTRE1_X 12
#define MONSTRE1_Y 13
#define MONSTRE2_X 14
#define MONSTRE2_Y 15
#define PLAYER_DROP_BOMB 21
#define MONSTRE1_DROP_BOMB 22
#define MONSTRE2_DROP_BOMB 23
#define I_AM_DEAD 30 //si je meurt je previent mon pote
#define MONSTRE1_DEAD 31 //si je meurt je previent mon pote
#define MONSTRE2_DEAD 32 //si je meurt je previent mon pote
#define NUM_LEVEL 40
#define SLAVE_DATA_BUFFER_LENGTH 10


#define TIMER_BOMBE 80;
#define NB_BOMBE 6
#define DIST_EXPLOSION 3//Distance de base de l'explosion
#define NB_FRAME_EXPLOSION 10
#define DIST_VUE_IA 6

//Maze define
#define WIDTH_BLOCK 4
#define HEIGHT_BLOCK 4
#define WIDTH_MAZE 21
#define HEIGHT_MAZE 12
#define NB_MAZE 5

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
void initGame()
{
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

  paused = true;

  switch(gb.menu(menu, 3)){
  case 0: //sigle player
    paused = false;
    disconnected = true;
    isMaster = true;
    isSingle = true;
    break;
  case 1: //Host
    paused = false;
    disconnected = false;
    isMaster = true;
    isSingle = false;
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
initScore();
  for(int8_t currentLevelCounter = 0; currentLevelCounter<NB_MAZE;currentLevelCounter++)
  {
    loadMazeByNumero(currentLevelCounter);
    while(true)
    {
      if(gb.update())
      {
        if(gb.buttons.pressed(BTN_C))
        {
          break;
        }

        updatePlayerAll(&masterPlayer);
        if(!isSingle) updatePlayerAll(&slavePlayer);//1 seul joueur je ne fait pas ça !
        updatePlayerAll(&monstre1);
        updatePlayerAll(&monstre2);

        if(isMaster)
        {
          updatePlayer(&masterPlayer);
          updateMonstres();
          if(!isSingle) updateMaster();
        }
        else
        {
          updatePlayer(&slavePlayer);
          if(!isSingle) updateSlave();
        }

        UpdateBombes();

        DrawMaze();
        DrawPlayers();
        DrawBombes();
      }

      if(!masterPlayer.isAlive ||  !slavePlayer.isAlive || (isSingle && !monstre1.isAlive &&  !monstre2.isAlive))
      {
        if(masterPlayer.isAlive)
        {
          winner[currentLevelCounter] = 1;
        }
        else if(!isSingle && slavePlayer.isAlive)
        {
          winner[currentLevelCounter] = 2;
        }
        else if(monstre1.isAlive && monstre2.isAlive)
        {
          winner[currentLevelCounter] = random(3,5);
        }
        else if(monstre1.isAlive)
        {
          winner[currentLevelCounter] = 3;
        }
        else if(monstre2.isAlive)
        {
          winner[currentLevelCounter] = 4;
        }
        break;
      };
    }
    
    if(ScoreScreen())
    {
      initGame();
    }
    else
    {
      break;
    }
  }

  gameOverScreen();
}



















































