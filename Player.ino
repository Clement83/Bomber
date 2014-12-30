
const byte MiniBomber[] PROGMEM = {
  8,5,0x70,0x88,0xD8,0xA8,0xD8,};

const byte MiniBomberP2[] PROGMEM = {
  8,5,0x70,0xD8,0x88,0xA8,0x50,};

const byte MiniMonster[] PROGMEM = {
  8,5,0x70,0xA8,0xF8,0xF8,0x70,};


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
void drawPlayer(Player play,bool isP1)
{
  if(play.isAlive)
  {
    //gb.display.drawCircle(play.x + 2,play.y + 2,2);
    if(!play.isMonster)
    {
      if(isP1){
        gb.display.drawBitmap(play.x,play.y,MiniBomber);
      }
      else
      {
        gb.display.drawBitmap(play.x,play.y,MiniBomberP2);
      }
    }
    else
    {
      gb.display.drawBitmap(play.x,play.y,MiniMonster);
    }
  }
}

void DrawPlayers()
{
  drawPlayer(masterPlayer,true);
  if(!isSingle) drawPlayer(slavePlayer,false); //pas besoin de le desiner

  drawPlayer(monstre1,false);
  drawPlayer(monstre2,false);
}




void updateMonstres()
{
  updateMonstre(&monstre1);
  updateMonstre(&monstre2);
}

void updateMonstre(Player *monstre)
{

  if(!paused){
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


bool MonsterCanDropBombe(Player monster)
{
  if(monster.nextBombe > 0)
  {
    return false;
  }

  if(caseHaveBombe(monster.xt,monster.yt))
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
        if(!caseHaveBombe(play->xt,play->yt))
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
      }
      if(getTile(play->xt,play->yt)>2)
      {
        play->isAlive = false;
      }
    }
  }
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


