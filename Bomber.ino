#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#include <Wire.h>

const byte MiniBomber[] PROGMEM = {
  8,5,0x70,0x88,0xD8,0xA8,0xD8,};
const byte MiniMonster[] PROGMEM = {
  8,5,0x70,0xA8,0xF8,0xF8,0x70,};

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
void drawPlayer(Player play);
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

//Shake the screen Rodot Code
byte shake_magnitude;
byte shake_timeLeft;

boolean isMaster = false;
boolean paused = true;
boolean disconnected = false;
boolean slave_updated = false;
boolean isSingle;

byte currentMaze = 0;

//multijoueur code
#define SLAVE_PAUSED 1
#define I_AM_MASTER 2
#define PLAYER_X 10
#define PLAYER_Y 11
#define MONSTRE1_X 12
#define MONSTRE1_Y 13
#define MONSTRE2_X 14
#define MONSTRE2_Y 15
#define PLAYER_DROP_BOMB 21
#define MONSTRE1_DROP_BOMB 22
#define MONSTRE2_DROP_BOMB 23
#define I_AM_DEAD 30 //si je meurt je previen mon pote
#define NUM_LEVEL 40
#define SLAVE_DATA_BUFFER_LENGTH 8


#define TIMER_BOMBE 80;
#define NB_BOMBE 3
#define DIST_EXPLOSION 3//Distance de base de l'explosion
#define NB_FRAME_EXPLOSION 20
#define DIST_VUE_IA 6

Player masterPlayer, slavePlayer, monstre1, monstre2;

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
    monstreBombe[i].isAlive = false;
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
    break;
  case 2: //Join
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
  bool dontMove = false;
  if(monstre->isAlive)
  {
    monstre->dropBombe = false;
    if(monstre->x == (monstre->xt*4) && monstre->y == (monstre->yt*4))
    {
      byte cptSortie = 4;
      int cheminPossible[4];
      chercherCheminPossible(cheminPossible,monstre);

      int choix =150;
      int oldValue = 9999;

      for(byte i=0;i<4;i++)
      { 
        if(cheminPossible[i]<oldValue)
        {
          oldValue = cheminPossible[i];
          choix = i;
        }
        // Serial.print(cheminPossible[i]);
        //Serial.print(" | ");
      }
      //Serial.println("");

      //case a danger imminent
      if(oldValue >50)
      {
        int tmpValue = evaluateCase(monstre->xt , monstre->yt);
        if(tmpValue < 50)
        {
          dontMove = true;
        }
      }
      if(!dontMove)
      {
        //on vas choisir une nouvelle destination 
        switch (choix)
        {
        case 0 ://bas
          if(TileIsOkMonster(monstre->xt,(monstre->yt+1)))
          {
            monstre->yt++;
          }
          break;
        case 1 ://droite
          if(TileIsOkMonster((monstre->xt+1),(monstre->yt)))
          {
            monstre->xt++;
          }
          break;
        case 2 ://haut
          if(TileIsOkMonster(monstre->xt,(monstre->yt-1)))
          {
            monstre->yt--;
          }
          break;
        case 3 ://gauche
          if(TileIsOkMonster((monstre->xt - 1),(monstre->yt)))
          {
            monstre->xt--;
          }
          break;
        default : 
          break;
        }
        //on tente de poser une bombe 
        if(MonsterCanDropBombe(*monstre))
        {
          DropBombe(monstre->xt*4,monstre->yt*4,monstreBombe);
          monstre->dropBombe = true;
          monstre->nextBombe = NB_FRAME_EXPLOSION;
        }
        else
        {
          if(monstre->nextBombe > 0)
          {
            monstre->nextBombe--;
          }
        }
      }
    }
    if(getTile(monstre->xt,monstre->yt)>2)
    {
      monstre->isAlive = false;
    }
  }
}

void chercherCheminPossible(int * cheminPossible, Player *monstre)
{
  //on vas regarder sur toutes les case avoisinante si il ya une possiblité de chemin securisé
  //0 ou moins c'est secure 9999 c'est impossible a aller.
  for(byte i=0;i<4;i++)
  {
    cheminPossible[i] = 9999;//trop haut on peut pas y aller ça doit etre du dur
  }

  //on commence par verrifier qu'on peut y aller 
  if(TileIsOkMonster(monstre->xt,(monstre->yt+1)))
  {
    cheminPossible[0] =evaluateCase(monstre->xt,(monstre->yt+1));
  }
  if(TileIsOkMonster((monstre->xt+1),(monstre->yt)))
  {
    cheminPossible[1] = evaluateCase((monstre->xt+1),(monstre->yt));
  }
  if(TileIsOkMonster(monstre->xt,(monstre->yt-1)))
  {
    cheminPossible[2] = evaluateCase(monstre->xt,(monstre->yt-1));
  }
  if(TileIsOkMonster((monstre->xt - 1),(monstre->yt)))
  {
    cheminPossible[3] = evaluateCase((monstre->xt - 1),(monstre->yt));
  }
}

int evaluateCase(char x , char y)
{
  int levelDanger = 0;
  //x droite
  for(byte decalageX = 0;decalageX<DIST_EXPLOSION;decalageX++)
  {
    byte tuileX = x+ decalageX;
    byte tuileY = y;
    byte tileId =getTile(tuileX,tuileY);
    if(tileId == 1 || tileId == 2)
      break;

    levelDanger += incraseLevelDanger(tuileX,tuileY,decalageX); 
  }
  //x gauche
  for(byte decalageX = 0;decalageX<DIST_EXPLOSION;decalageX++)
  {
    byte tuileX = (x)- decalageX;
    byte tuileY = (y);
    byte tileId =getTile(tuileX,tuileY);
    if( tileId == 1 || tileId == 2)
      break;

    levelDanger += incraseLevelDanger(tuileX,tuileY,decalageX); 
  }
  //y bas
  for(byte decalageY = 0;decalageY<DIST_EXPLOSION;decalageY++)
  {
    byte tuileX = (x);
    byte tuileY = (y)+ decalageY;
    byte tileId =getTile(tuileX,tuileY);
    if( tileId == 1 || tileId == 2)
      break;

    levelDanger +=  incraseLevelDanger(tuileX,tuileY,decalageY); 
  }
  //y haut
  for(byte decalageY = 0;decalageY<DIST_EXPLOSION;decalageY++)
  {
    byte tuileX = (x);
    byte tuileY = (y)- decalageY;

    byte tileId =getTile(tuileX,tuileY);
    if( tileId == 1 || tileId == 2)
      break;

    levelDanger += incraseLevelDanger(tuileX,tuileY,decalageY);
  } 

  return levelDanger;
}

int incraseLevelDanger(char tuileX, char tuileY,byte decalage)
{
  //le random 0,10 sert a faire que les cases de meme poid soit mélanger
  //on cherche une bombe ou un joueur
  if(caseHaveBombe(tuileX,tuileY))
  {
    //plus on est pres de la bombe plus c'est dangereux
    return ((DIST_EXPLOSION - decalage) + 50) + random(0,10);//superieur a 10 c'est danger!
  }
  return random(0,10);//en dessous de 10 c'est safe !
}

bool caseHavePlayer(char x,char y)
{
  return false;
}

bool caseHaveBombe(char x, char y)
{
  for(byte i=0;i<NB_BOMBE;i++)
  {
    if(masterBombe[i].isAlive && (masterBombe[i].x/4) == x && (masterBombe[i].y/4) == y )
    {
      return true;
    }
    if(slaveBombe[i].isAlive && (slaveBombe[i].x/4) == x && (slaveBombe[i].y/4) == y )
    {
      return true;
    }
    if(monstreBombe[i].isAlive && (monstreBombe[i].x/4) == x && (monstreBombe[i].y/4) == y)
    {
      return true;
    }
  }
  return false;
}

bool MonsterCanDropBombe(Player monster)
{
  if(monster.nextBombe > 0)
  {
    return false;
  }

  //Serial.print("[");
  //Serial.print(evaluateCase((monster.xt-1),monster.yt));
  //Serial.print("]");
  if(evaluateCase((monster.xt-1),monster.yt)>50)
  {
    return false;
  }
  //Serial.print("[");
  //Serial.print(evaluateCase((monster.xt+1),monster.yt));
  //Serial.print("]");
  if(evaluateCase((monster.xt+1),monster.yt)>50)
  {
    return false;
  }

  //Serial.print("[");
  //Serial.print(evaluateCase((monster.xt),(monster.yt-1)));
  //Serial.print("]");
  if(evaluateCase((monster.xt),(monster.yt-1))>50)
  {
    return false;
  }

  //Serial.print("[");
  //Serial.print(evaluateCase((monster.xt),(monster.yt+1)));
  //Serial.print("]");
  if(evaluateCase((monster.xt),(monster.yt+1))>50)
  {
    return false;
  }

  if(random(0,30) == 0)
    return true;
  //haut
  byte tile = getTile((monster.xt-1),monster.yt);
  if(tile == 2)
  {
    return true;
  } 
  tile = getTile((monster.xt+1),monster.yt);
  if(tile == 2)
  {
    return true;
  }
  tile = getTile((monster.xt),(monster.yt-1));
  if(tile == 2)
  {
    return true;
  }
  tile = getTile((monster.xt),(monster.yt+1));
  if(tile == 2)
  {
    return true;
  }
  return false;
}

void gameOverScreen()
{
  endGame();
  while(true)
  {
    if(gb.update())
    {
      if(isSingle)
      {
        if(masterPlayer.isAlive)
        {
          gb.display.println(F("YOU WIN!"));
        }
        else
        {
          gb.display.println(F("YOU LOOSE!"));
        }
      }
      else 
      {
        if(!masterPlayer.isAlive)
        {
          gb.display.println(F("Player 2 WIN!"));
        }
        else
        {
          gb.display.println(F("Player 1 WIN!"));
        }
      }
      gb.display.print(F("Press any key"));
      if(gb.buttons.pressed(BTN_A) ||gb.buttons.pressed(BTN_B) ||gb.buttons.pressed(BTN_C))
      {
        break;
      }
    }
    masterPlayer.isAlive = true;
    slavePlayer.isAlive = true;
  }
}

void updatePlayerAll(Player *play)
{

  if(play->isAlive)
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
}
void updatePlayer(Player *play)
{
  if(play->isAlive)
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
  if(caseHaveBombe(x,y))
    return false;

  byte tile = getTile(x,y);
  return tile == 0 || tile > 2;
}
bool TileIsOkMonster(byte x,byte y)
{
  //recherche des bombes si il y a une bombe on ne peut pas bougé
  byte tile = getTile(x,y);
  return tile == 0;
}

void DrawPlayers()
{
  drawPlayer(masterPlayer);
  if(!isSingle) drawPlayer(slavePlayer); //pas besoin de le desiner

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
    if(monstreBombe[i].isAlive && monstreBombe[i].timer%20>5)//1/4 du temps eteint
    {
      gb.display.fillCircle(monstreBombe[i].x+2,monstreBombe[i].y+2,1);
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
    if(monstreBombe[i].isAlive)
    {
      if(monstreBombe[i].timer == 0)
      {
        monstreBombe[i].isAlive = false;
        //explosion de la bombe
        ExplosionBombe(monstreBombe[i]);
      }
      monstreBombe[i].timer--;
    }
  }
}

void drawPlayer(Player play)
{
  if(play.isAlive)
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
}

void ExplosionBombe(Bombe laBombe)
{

  shake_magnitude = 2;
  shake_timeLeft = 3;
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
    if(monstreBombe[i].isAlive && monstreBombe[i].x == x && monstreBombe[i].y == y)
    {
      monstreBombe[i].isAlive = false;
      ExplosionBombe(monstreBombe[i]);
    }
  }
}

void endGame()
{
  int timer =0;
  while(1){
    if(gb.update()){

      DrawMaze();
      DrawPlayers();
      DrawBombes();

      //gb.display.drawBitmap(x-1, y-1, playerSprite, playerDir, NOFLIP);
      gb.display.setColor(WHITE);
      gb.display.fillRect(0,0,timer*2,LCDHEIGHT);
      gb.display.fillRect(LCDWIDTH-timer*2,0,timer*2,LCDHEIGHT);
      gb.display.setColor(BLACK, WHITE);
      gb.display.cursorX = 12;
      gb.display.cursorY = 1;
      gb.display.print(F("GAME OVER!"));
      timer++;
      if(timer==((LCDWIDTH/4)+10))
        break;
    }
  }
}















































