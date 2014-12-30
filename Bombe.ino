
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



