#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#include <Wire.h>

const byte MiniBomber[] PROGMEM = {
  8,5,0x70,0x88,0xD8,0xA8,0xD8,};
const byte MiniMonster[] PROGMEM = {
  8,5,0x70,0xA8,0xF8,0xF8,0x70,};

typedef struct {
  char x, y,xt,yt;
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
void drawPlayer(Player play);
void updatePlayer(Player *play);
bool TileIsOk(byte x, byte y);
void ExplosionBombe(Bombe laBombe);
void TestReactionEnChaineBombe(byte x,byte y);
bool SetTuileExplosion(byte tuileX,byte tuileY);
void updatePlayerAll(Player *play);
void updateMonstre(Player *monstre);

boolean isMaster = false;
boolean paused = true;
boolean disconnected = false;
boolean slave_updated = false;


byte currentMaze = 0;

//multijoueur code
#define SLAVE_PAUSED 1
#define I_AM_MASTER 2
#define PLAYER_X 10
#define PLAYER_Y 11
#define PLAYER_DROP_BOMB 21
#define I_AM_DEAD 30 //si je meurt je previen mo pote
#define NUM_LEVEL 40
#define SLAVE_DATA_BUFFER_LENGTH 8


#define TIMER_BOMBE 80;
#define NB_BOMBE 3
#define DIST_EXPLOSION 3//Distance de base de l'explosion
#define NB_FRAME_EXPLOSION 30

Player masterPlayer, slavePlayer, monstre1, monstre2;

Bombe masterBombe[NB_BOMBE];
Bombe slaveBombe[NB_BOMBE];

Bombe M1Bombe[NB_BOMBE];
Bombe M2Bombe[NB_BOMBE];

byte distExplosion;
const char strMaster[] PROGMEM = "Host (master)";
const char strSlave[] PROGMEM = "Join (slave)";
const char* const menu[2] PROGMEM = {
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

void goTitleScreen()
{
  gb.titleScreen(F("Bomber"));
  initGame();
  currentMaze = 0;
}

void initGame()
{
  P1StartPos();
  P2StartPos();
  M1StartPos();
  M2StartPos();
  loadNextMaze();
  distExplosion =DIST_EXPLOSION;

  for(byte i=0;i<NB_BOMBE;i++)
  {
    masterBombe[i].isAlive = false;
    slaveBombe[i].isAlive = false;
  }

}

void P1StartPos()
{
  masterPlayer.x = 4;
  masterPlayer.y = 4;
  masterPlayer.xt = 1;
  masterPlayer.yt = 1;
  masterPlayer.isAlive = true;
  masterPlayer.isMonster = false;
}
void P2StartPos()
{
  slavePlayer.x = 76;
  slavePlayer.y = 40;
  slavePlayer.xt = 19;
  slavePlayer.yt = 10;
  slavePlayer.isAlive = true;
  slavePlayer.isMonster = false;
}

void M1StartPos()
{
  monstre1.x = 4;
  monstre1.y = 40;
  monstre1.xt = 1;
  monstre1.yt = 10;
  monstre1.isAlive = true;
  monstre1.isMonster = true;
}
void M2StartPos()
{
  monstre2.x = 76;
  monstre2.y = 4;
  monstre2.xt = 19;
  monstre2.yt = 1;
  monstre2.isAlive = true;
  monstre2.isMonster = true;
}


void loop(){

  paused = true;

  switch(gb.menu(menu, 2)){
  case 0: //Host
    paused = false;
    disconnected = false;
    isMaster = true;
    break;
  case 1: //Join
    paused = false;
    disconnected = false;
    isMaster = false;
    break;
  default:
    goTitleScreen();
    break;
  }

  while(true)
  {
    if(gb.update())
    {
      if(gb.buttons.pressed(BTN_C))
      {
        break;
      }

      updatePlayerAll(&masterPlayer);
      updatePlayerAll(&slavePlayer);
      updatePlayerAll(&monstre1);
      updatePlayerAll(&monstre2);

      if(isMaster)
      {
        updatePlayer(&masterPlayer);
        updateMonstres();
        // updateMaster();
      }
      else
      {
        updatePlayer(&slavePlayer);
        // updateSlave();
      }

      UpdateBombes();

      DrawMaze();
      DrawPlayers();
      DrawBombes();
    }

    if(!masterPlayer.isAlive ||  !slavePlayer.isAlive)
    {
      gameOverScreen();
      initGame();
    };
  }

}

void updateMonstres()
{
  updateMonstre(&monstre1);
  updateMonstre(&monstre2);
}

void updateMonstre(Player *monstre)
{
  if(monstre->x == (monstre->xt*4))
  {
    //on vas choisir une nouvelle destination 
  }
  
}

void gameOverScreen()
{
  if(!masterPlayer.isAlive ||  !slavePlayer.isAlive)
  {
    while(true)
    {
      if(gb.update())
      {
        if(!masterPlayer.isAlive)
        {
          gb.display.print(F("Player 2 WIN!"));
        }
        else
        {
          gb.display.print(F("Player 1 WIN!"));
        }
        if(gb.buttons.pressed(BTN_A) ||gb.buttons.pressed(BTN_B) ||gb.buttons.pressed(BTN_C))
        {
          break;
        }
      }
    }
    masterPlayer.isAlive = true;
    slavePlayer.isAlive = true;
  }
}

void updatePlayerAll(Player *play)
{
  bool isMove = ((play->x != play->xt*4) || (play->y != play->yt*4));

  if(isMove)
  {
    if(play->x != play->xt*4)
    {
      if(play->xt*4 > play->x)
      {
        play->x++;
      }
      else
      {
        play->x--;
      }
    }
    else if(play->y != play->yt*4)
    {
      if(play->yt*4 > play->y)
      {
        play->y++;
      }
      else
      {
        play->y--;
      }
    }
  }
}
void updatePlayer(Player *play)
{
  if(!paused){
    play->dropBombe = false;

    bool isMove = ((play->x != play->xt*4) || (play->y != play->yt*4));

    if(!isMove)
    {
      if(gb.buttons.repeat(BTN_RIGHT,1))
      {
        if(TileIsOk((play->xt+1),play->yt))
          play->xt++;
      }
      else if  (gb.buttons.repeat(BTN_LEFT,1))
      {
        if(TileIsOk((play->xt-1),play->yt))
          play->xt--;
      }
      else if(gb.buttons.repeat(BTN_UP,1))
      {
        if(TileIsOk(play->xt,(play->yt-1)))
          play->yt--;
      }
      else if  (gb.buttons.repeat(BTN_DOWN,1))
      {
        if(TileIsOk(play->xt,(play->yt+1)))
          play->yt++;
      }
    }

    if  (gb.buttons.pressed(BTN_A))
    {
      if(isMaster)
      {
        DropBombe(play->xt*4,play->yt*4,masterBombe);
      }
      else
      {
        DropBombe(play->xt*4,play->yt*4,slaveBombe);
      }
      play->dropBombe = true;
    }

    if(getTile(play->xt,play->yt)>2)
    {
      play->isAlive = false;
    }
  }
}

void DropBombe(byte x,byte y, Bombe * bombeArray )
{
  byte cpt = 0;
  do
  {
    if(bombeArray[cpt].isAlive == false)
    {
      bombeArray[cpt].timer = TIMER_BOMBE;
      bombeArray[cpt].distExplos = distExplosion;
      bombeArray[cpt].isAlive = true;
      bombeArray[cpt].x = x;
      bombeArray[cpt].y = y;
      break;
    }

    cpt++;
  }
  while(cpt<NB_BOMBE);
}



bool TileIsOk(byte x,byte y)
{
  //recherche des bombes si il y a une bombe on ne peut pas bougé
  byte tile = getTile(x,y);
  return tile == 0 || tile > 2;
}

void DrawPlayers()
{
  drawPlayer(masterPlayer);
  drawPlayer(slavePlayer);

  drawPlayer(monstre1);
  drawPlayer(monstre2);
}

void DrawBombes()
{
  for(byte i=0;i<NB_BOMBE;i++)
  {
    if(masterBombe[i].isAlive && masterBombe[i].timer%20>5)//1/4 du temps eteint
    {
      gb.display.fillCircle(masterBombe[i].x+2,masterBombe[i].y+2,1);
    }
    if(slaveBombe[i].isAlive && slaveBombe[i].timer%20>5)//1/4 du temps eteint
    {
      gb.display.fillCircle(slaveBombe[i].x+2,slaveBombe[i].y+2,1);
    }
  }
}

void UpdateBombes()
{
  for(byte i=0;i<NB_BOMBE;i++)
  {
    if(masterBombe[i].isAlive)
    {
      if(masterBombe[i].timer == 0)
      {
        masterBombe[i].isAlive = false;
        //explosion de la bombe
        ExplosionBombe(masterBombe[i]);
      }
      masterBombe[i].timer--;
    }
    if(slaveBombe[i].isAlive)
    {
      if(slaveBombe[i].timer == 0)
      {
        slaveBombe[i].isAlive = false;
        //explosion de la bombe
        ExplosionBombe(slaveBombe[i]);
      }
      slaveBombe[i].timer--;
    }
  }

}

void drawPlayer(Player play)
{
  //gb.display.drawCircle(play.x + 2,play.y + 2,2);
  if(!play.isMonster)
  {
    gb.display.drawBitmap(play.x,play.y,MiniBomber);
  }
  else 
  {
    gb.display.drawBitmap(play.x,play.y,MiniMonster);
  }
}

void ExplosionBombe(Bombe laBombe)
{
  //x droite
  for(byte decalageX = 0;decalageX<laBombe.distExplos;decalageX++)
  {
    byte tuileX = laBombe.x/4 + decalageX;
    byte tuileY = laBombe.y/4 ;
    if(SetTuileExplosion(tuileX,tuileY))
    {
      break;
    }
  }
  //x gauche
  for(byte decalageX = 0;decalageX<laBombe.distExplos;decalageX++)
  {
    byte tuileX = laBombe.x/4 - decalageX;
    byte tuileY = laBombe.y/4 ;
    if(SetTuileExplosion(tuileX,tuileY))
    {
      break;
    }
  }
  //y bas
  for(byte decalageY = 0;decalageY<laBombe.distExplos;decalageY++)
  {
    byte tuileX = laBombe.x/4;
    byte tuileY = laBombe.y/4  + decalageY;
    if(SetTuileExplosion(tuileX,tuileY))
    {
      break;
    }
  }
  //y haut
  for(byte decalageY = 0;decalageY<laBombe.distExplos;decalageY++)
  {
    byte tuileX = laBombe.x/4;
    byte tuileY = laBombe.y/4  - decalageY;
    if(SetTuileExplosion(tuileX,tuileY))
    {
      break;
    }
  }
}

bool SetTuileExplosion(byte tuileX,byte tuileY)
{

  byte tuileId = getTile(tuileX,tuileY);
  if( tuileId != 1 )
  {
    setTile(tuileX,tuileY,NB_FRAME_EXPLOSION);
    TestReactionEnChaineBombe(tuileX*4,tuileY*4);
  }
  if(tuileId == 1 || tuileId == 2)
  {
    //si c'est du dur on arrete
    return true;
  }
  return false;
}

void TestReactionEnChaineBombe(byte x,byte y)
{
  //recherche des reaction en chaine
  for(byte i=0;i<NB_BOMBE;i++)
  {
    if(masterBombe[i].isAlive && masterBombe[i].x == x && masterBombe[i].y == y)
    {
      masterBombe[i].isAlive = false;
      ExplosionBombe(masterBombe[i]);
    }
    if(slaveBombe[i].isAlive && slaveBombe[i].x == x && slaveBombe[i].y == y)
    {
      slaveBombe[i].isAlive = false;
      ExplosionBombe(slaveBombe[i]);
    }
  }

}




















