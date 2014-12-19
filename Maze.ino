
#define WIDTH_BLOCK 4
#define HEIGHT_BLOCK 4
#define WIDTH_MAZE 21
#define HEIGHT_MAZE 12


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
  1,0,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,0,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
  1,0,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,0,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};
const byte Maze3[HEIGHT_MAZE*WIDTH_MAZE] PROGMEM = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
  1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
  1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,1,
  1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
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


void DrawMaze()
{
  for(byte y = 0; y < HEIGHT_MAZE; y++){
    for(byte x = 0; x < WIDTH_MAZE; x++){
      byte spriteID = getTile(x,y);
      if(spriteID == 0)
      {
        continue;
      }
      else if(spriteID == 1)
      {
        gb.display.fillRect(x*WIDTH_BLOCK , y*HEIGHT_BLOCK,WIDTH_BLOCK,HEIGHT_BLOCK);
      }
      else if(spriteID > 2)
      {
        gb.display.drawPixel(random((x*WIDTH_BLOCK), (x*WIDTH_BLOCK + WIDTH_BLOCK) )  , random((y*HEIGHT_BLOCK),(y*HEIGHT_BLOCK+HEIGHT_BLOCK)));
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
byte currentMaze = 0;

void loadNextMaze()
{
  switch(currentMaze)
  {
    case 0 : 
    loadMaze1();
    break;
    case 1 : 
    loadMaze2();
    break;
    case 2 : 
    loadMaze3();
    break;
    
  }
  
  currentMaze++;
  
  currentMaze = currentMaze%3;
}

void loadMaze1()
{
  
  for(byte y = 0; y < HEIGHT_MAZE; y++){
    for(byte x = 0; x < WIDTH_MAZE; x++){
      Maze[(y*WIDTH_MAZE)+x] = pgm_read_byte(Maze1 + (y*WIDTH_MAZE)+x);
    }
  }
}
void loadMaze2()
{
  
  for(byte y = 0; y < HEIGHT_MAZE; y++){
    for(byte x = 0; x < WIDTH_MAZE; x++){
      Maze[(y*WIDTH_MAZE)+x] = pgm_read_byte(Maze2 + (y*WIDTH_MAZE)+x);
    }
  }
}
void loadMaze3()
{
  
  for(byte y = 0; y < HEIGHT_MAZE; y++){
    for(byte x = 0; x < WIDTH_MAZE; x++){
      Maze[(y*WIDTH_MAZE)+x] = pgm_read_byte(Maze3 + (y*WIDTH_MAZE)+x);
    }
  }
}

