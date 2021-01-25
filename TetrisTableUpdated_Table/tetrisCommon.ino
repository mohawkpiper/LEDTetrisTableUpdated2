/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty - Modified to be like SNES classic by: Greg Beckman
 * 
 * Common code and defines for the Tetris game
 */
 //tetris is 10x20///
//Maximum size of bricks. Individual bricks can still be smaller (eg 3x3)
#define  MAX_BRICK_SIZE    4
#define  BRICKOFFSET       -1 // Y offset for new bricks

#define TETRIS_FIELD_WIDTH    10
#define TETRIS_FIELD_HEIGHT   20
#define TETRIS_FIELD_YOFF     6
#define TETRIS_FIELD_XOFF     1
int ghostON = 0;
int rngON = 0;
int invisibleON = 0;

// Playing field
struct Field{
  uint8_t pix[TETRIS_FIELD_WIDTH][TETRIS_FIELD_HEIGHT+1];//Make field one larger so that collision detection with bottom of field can be done in a uniform way
  unsigned long color[TETRIS_FIELD_WIDTH][TETRIS_FIELD_HEIGHT];
  unsigned long color2[TETRIS_FIELD_WIDTH][TETRIS_FIELD_HEIGHT];
};
Field field;

//Structure to represent active brick on screen
struct Brick{
  boolean enabled;//Brick is disabled when it has landed
  int xpos,ypos;
  int yOffset;//Y-offset to use when placing brick at top of field
  uint8_t siz;
  uint8_t pix[MAX_BRICK_SIZE][MAX_BRICK_SIZE];
  unsigned int long col;
  boolean isFlat;
};
Brick activeBrick;
Brick nextBrick;
Brick ghostBrick;

//Struct to contain the different choices of blocks
struct AbstractBrick{
  int yOffset;//Y-offset to use when placing brick at top of field
  uint8_t siz;
  uint8_t pix[MAX_BRICK_SIZE][MAX_BRICK_SIZE];
  uint8_t colorIdentifier;
};

//Brick "library"
AbstractBrick brickLib[7] = {
  {
      0,//yoffset when adding brick to field
      4,
      { {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
      },
      0
  },
  {
      1,
      4,
      { {0,0,0,0},
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0}
      },
      0
  },
  {
      0,
      3,
      { {0,0,0,0},
        {1,1,1,0},
        {0,0,1,0},
        {0,0,0,0}
      },
      2
  },
  {
      0,
      3,
      { {0,0,0,0},
        {1,1,1,0},
        {1,0,0,0},
        {0,0,0,0}
      },
      1
  },
  {
      0,
      3,
      { {0,0,0,0},
        {1,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
      },
      0
  },
  {
      0,
      3,
      { {0,0,0,0},
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0}
      },
      2
  },
  {
      0,
      3,
      { {0,0,0,0},
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
      },
      1}
};
