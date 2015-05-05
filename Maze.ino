

const byte MiniExplode[] PROGMEM = {8,4,0x20,0xC0,0x30,0xA0,};


const byte Maze1[HEIGHT_MAZE*WIDTH_MAZE] PROGMEM = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
  1,0,2,1,2,1,0,0,0,0,0,1,2,2,2,2,2,1,2,0,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2,1,
  1,2,2,1,2,2,1,2,2,2,2,2,2,1,2,0,1,2,2,2,1,
  1,2,1,0,0,0,0,1,2,2,2,2,2,2,2,0,2,2,2,2,1,
  1,2,2,2,2,0,2,2,2,2,2,2,1,0,0,0,0,1,2,2,1,
  1,2,2,1,2,0,1,2,2,2,2,2,2,1,2,2,1,2,2,2,1,
  1,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,0,2,1,2,2,1,2,2,1,0,0,0,0,0,2,2,1,2,0,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

const byte Maze2[HEIGHT_MAZE*WIDTH_MAZE] PROGMEM = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
  1,0,1,2,1,2,1,2,0,2,0,2,1,2,1,2,1,2,1,0,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,2,1,2,0,2,1,2,1,2,1,2,0,2,1,2,1,2,1,2,1,
  1,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,1,
  1,2,2,2,0,2,2,2,1,2,2,2,2,2,2,0,2,2,2,2,1,
  1,2,1,2,1,2,1,2,0,2,1,2,1,2,1,2,0,2,1,2,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,0,1,2,1,2,1,2,1,2,0,2,1,2,1,2,1,2,1,0,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};
const byte Maze3[HEIGHT_MAZE*WIDTH_MAZE] PROGMEM = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
  1,0,2,2,2,2,2,0,2,2,2,2,2,2,0,2,2,2,2,0,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,2,2,2,1,2,2,2,2,1,1,1,2,2,2,2,1,2,2,2,1,
  1,2,2,2,2,0,2,2,2,1,0,0,2,2,2,2,2,2,2,2,1,
  1,2,2,2,2,2,2,2,2,0,0,1,2,2,2,2,2,2,2,2,1,
  1,2,2,2,1,2,2,2,2,1,1,1,2,2,2,2,1,2,2,2,1,
  1,2,2,2,2,0,2,2,2,2,2,2,2,0,2,2,2,2,2,2,1,
  1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

const byte Maze4[HEIGHT_MAZE*WIDTH_MAZE] PROGMEM = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,1,
  1,0,1,2,2,2,0,2,2,0,0,0,2,2,1,2,2,2,1,0,1,
  1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,1,
  1,1,2,2,1,2,1,2,2,1,1,1,2,2,2,2,1,2,2,1,1,
  1,2,2,2,2,2,2,2,2,2,0,0,2,2,2,1,2,1,2,1,1,
  1,1,2,1,2,1,2,2,2,0,0,2,2,2,2,2,2,2,2,2,1,
  1,1,2,2,1,2,1,2,2,1,1,1,2,2,2,2,1,2,2,2,1,
  1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,1,
  1,0,1,2,2,2,1,2,2,0,0,0,2,2,1,2,2,2,1,0,1,
  1,0,0,0,1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

byte Maze5[HEIGHT_MAZE*WIDTH_MAZE] PROGMEM = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,2,2,0,0,0,0,2,2,2,2,2,2,2,0,0,0,1,
  1,0,1,2,1,2,1,2,0,2,1,2,1,2,1,2,1,2,1,0,1,
  1,2,0,2,1,2,1,2,0,2,1,2,1,2,1,2,1,2,1,2,1,
  1,2,1,0,0,0,1,2,0,2,1,2,0,2,1,2,0,2,1,2,1,
  1,2,1,2,0,2,1,2,1,2,1,2,1,2,0,2,1,2,1,2,1,
  1,2,0,2,1,2,0,2,1,2,1,2,1,2,0,2,1,2,1,2,1,
  1,2,1,2,1,2,1,2,0,2,1,2,0,2,0,2,0,2,1,2,1,
  1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
  1,0,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,0,1,
  1,0,0,0,2,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

byte Maze[HEIGHT_MAZE*WIDTH_MAZE]  = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
  1,0,2,1,2,1,0,0,0,0,0,1,2,2,2,2,2,1,2,0,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2,1,
  1,2,2,1,2,2,1,2,2,2,2,2,2,1,2,0,1,2,2,2,1,
  1,2,1,0,0,0,0,1,2,2,2,2,2,2,2,0,2,2,2,2,1,
  1,2,2,2,2,0,2,2,2,2,2,2,1,0,0,0,0,1,2,2,1,
  1,2,2,1,2,0,1,2,2,2,2,2,2,1,2,2,1,2,2,2,1,
  1,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,0,2,1,2,2,1,2,2,1,0,0,0,0,0,2,2,1,2,0,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};



byte getTile(byte x, byte y){
  return (Maze[(y*WIDTH_MAZE)+x]);
}

void setTile(byte x, byte y,byte value){
  Maze[(y*WIDTH_MAZE)+x] = value;
}
int cameraX, cameraY;
void shakeScreen(){
  if(shake_timeLeft){
    shake_timeLeft--;
    cameraX += random(-shake_magnitude,shake_magnitude+1);
    cameraY += random(-shake_magnitude,shake_magnitude+1);
    byte backlightStep = gb.backlight.backlightMax / 4;
    gb.backlight.set(gb.backlight.backlightValue-random(0,backlightStep*shake_magnitude));
  }
  else 
  {
    cameraX = 0;
    cameraY = 0;
  }
}


void DrawMaze()
{
  shakeScreen();
  for(byte y = 0; y < HEIGHT_MAZE; y++){
    for(byte x = 0; x < WIDTH_MAZE; x++){
      byte spriteID = getTile(x,y);
      if(spriteID == 0)
      {
        continue;
      }
      else if(spriteID == 1)
      {
        gb.display.fillRect(((x*WIDTH_BLOCK) - cameraX), ((y*HEIGHT_BLOCK) - cameraY),WIDTH_BLOCK,HEIGHT_BLOCK);
      }
      else if(spriteID > 2)
      {
        //gb.display.drawPixel(random((x*WIDTH_BLOCK), (x*WIDTH_BLOCK + WIDTH_BLOCK) )  , random((y*HEIGHT_BLOCK),(y*HEIGHT_BLOCK+HEIGHT_BLOCK)));
        //gb.display.fillCircle((x*WIDTH_BLOCK)+(WIDTH_BLOCK/2),(y*HEIGHT_BLOCK)+(HEIGHT_BLOCK/2),1);
        gb.display.drawBitmap((x*WIDTH_BLOCK),(y*HEIGHT_BLOCK),MiniExplode);
        spriteID--;
        if(spriteID == 2)
          spriteID = 0;
          
        setTile(x,y,spriteID);
      }
      else 
      {
        gb.display.drawRect((x*WIDTH_BLOCK)+1 , (y*HEIGHT_BLOCK)+1,WIDTH_BLOCK/2,HEIGHT_BLOCK/2);
      }
    }
  }
}

void loadMazeByNumero(int8_t numero)
{
  switch(numero)
  {
    case 0 : 
    loadMaze(Maze1);
    break;
    case 1 : 
    loadMaze(Maze2);
    break;
    case 2 : 
    loadMaze(Maze3);
    break;
    case 3 : 
    loadMaze(Maze4);
    break;
    case 4 : 
    loadMaze(Maze5);
    break;
    
  }
}

void loadMaze(const byte *maze)
{
  for(byte y = 0; y < HEIGHT_MAZE; y++){
    for(byte x = 0; x < WIDTH_MAZE; x++){
      Maze[(y*WIDTH_MAZE)+x] = pgm_read_byte(maze + (y*WIDTH_MAZE)+x);
    }
  }
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

