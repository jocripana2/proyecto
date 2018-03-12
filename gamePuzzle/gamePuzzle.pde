// ********** GLOBAL VARIABLES
BoardGame board;

// ********** PROCESSING FUNCTIONS
void setup()
{
  size(500,500);
  background(200);
  
  board = new BoardGame(8,6);
  board.imageList = new ImageList("res", "fruit", 5);
  board.imageList.resizeAll(50,50);
  board.setBoardOrigin(20,20);
  board.setSpaceBetweenElements(5);
  board.init();
}

void draw()
{
  background(200);
  board.draw();
}

void mouseMoved()
{
  board.setCurrentPos(mouseX, mouseY);
}

void mousePressed()
{
  board.highlight(mouseX, mouseY, !board.getHighlightState(mouseX,mouseY));
}

void keyPressed()
{
  if(keyCode == ENTER)
    board.blow();
}



// ********** OTHER FUNCTIONS

/*Sprite sprite;

void setup()
{
  size(500,400);
  
  sprite = new Sprite("/Users/sheila/calibrating.png", 1, 5);
  sprite.setPosition(50,50);
}

void draw()
{
  background(200);
  sprite.draw();
}

void keyPressed()
{
  sprite.nextFrame();
}*/