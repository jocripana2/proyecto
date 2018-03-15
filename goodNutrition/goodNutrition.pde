// GLOBAL VARIABLES
PImage[] background;
PImage parachute;
ArrayList<FallingItem> item;
ImageList veggies, grains, meat, milk;
FallingItem selectedItem;
Sprite score10_sprite, scoreGlobal_sprite;
Sprite start, gameOver, exitSprite;
GameState gameState;
PFont font;

int backWidth, backHeight, dropLine;
int addItemTime, pointAnimTime, levelTime;
int addItemLimit;
int score;
boolean selecting, scoring;

// PROCESSING FUNCTIONS
void setup()
{
  fullScreen();
  background(200);
  
  backWidth = width/4;
  backHeight = height;
  dropLine = height*3/4;
  
  font = createFont("/Library/Fonts/Arial Rounded Bold.ttf", 20);
  
  // Load images
  background = new PImage[4];
  background[0] = loadImage("res/veggies_background.png");
  background[1] = loadImage("res/grains_background.png");
  background[2] = loadImage("res/milk_background.png");
  background[3] = loadImage("res/meat_background.png");
  for(int i=0; i<4; i++)
    background[i].resize(backWidth, backHeight);
    
  parachute = loadImage("res/parachute.png");
  parachute.resize(parachute.width/2,parachute.height/2);
    
  // Image lists
  veggies = new ImageList("res/veggies", "icon", 9);
  grains = new ImageList("res/grains", "icon", 5);
  meat = new ImageList("res/meat", "icon", 10);
  milk = new ImageList("res/milk", "icon", 3);
  
  veggies.resizeAll(0.3);
  grains.resizeAll(0.3);
  meat.resizeAll(0.3);
  milk.resizeAll(0.3);
  
  // Falling items
  item = new ArrayList<FallingItem>();
  
  // Sprites
  score10_sprite = new Sprite("res/points_add_10.png", 1, 11);
  score10_sprite.resize(0.2);
  
  scoreGlobal_sprite = new Sprite("res/points_total_score.png", 5, 10);
  scoreGlobal_sprite.resize(0.2);
  scoreGlobal_sprite.setPosition(width-260, 10);
  
  start = new Sprite("res/start.png", 1, 2);
  start.setPosition(width/2-start.getFrameWidth()/2, 
                    height/2-start.getFrameHeight()/2);
  
  gameOver = new Sprite("res/game_over.png", 1, 1);
  gameOver.setPosition(width/2-gameOver.getFrameWidth()/2, 
                       height/2-gameOver.getFrameHeight()/2);
  
  exitSprite = new Sprite("res/exit.png", 1, 2);
  exitSprite.resize(0.5);
  exitSprite.setPosition(width/3-exitSprite.getFrameWidth(), height*2/3);
  
  initGame();
}

void draw()
{
  background(200);
  
  switch(gameState)
  {
    case START:
      start.draw();
      break;
    case PLAYING:
      gameMechanics();
      break;
    case GAMEOVER:
      gameOver.draw();
      exitSprite.draw();
  }
}

void mousePressed()
{
  if(gameState == GameState.START)
  {
    if(mouseX >= width/2-start.getFrameWidth()/2 && 
       mouseX <= width/2+start.getFrameWidth()/2 &&
       mouseY >= height/2-start.getFrameHeight()/2 && 
       mouseY <= height/2+start.getFrameHeight()/2)
      gameState = GameState.PLAYING;
  }
  else if(gameState == GameState.PLAYING)
  {
    if(selecting == true)
    {
      selecting = false;
      selectedItem.drop();
      if(!scoring && scoreValid(selectedItem.category, 
                                  int(selectedItem.getXPosition())))
        {
          score++;
          scoreGlobal_sprite.nextFrame();
          score10_sprite.setPosition(int(selectedItem.getXPosition()), 
                                     int(selectedItem.getYPosition()));
          score10_sprite.setCurrentFrame(0,0);
          scoring = true;
        }
      return;
    }
    
    for(int i=0; i<item.size(); i++)
    {
      FallingItem auxItem = item.get(i);
      if(auxItem.isGrabbed(mouseX, mouseY))
      {
        selectedItem = auxItem;
        selecting = true;
      }
    }
  }
  else if(gameState == GameState.GAMEOVER)
  {
    if(mouseX >= width/3-exitSprite.getFrameWidth() && 
       mouseX <= width/3 &&
       mouseY >= height*2/3 && 
       mouseY <= height*2/3+exitSprite.getFrameHeight())
      exit();
  }
}

void mouseMoved()
{
  if(gameState == GameState.START)
  {
    if(mouseX >= width/2-start.getFrameWidth()/2 && 
       mouseX <= width/2+start.getFrameWidth()/2 &&
       mouseY >= height/2-start.getFrameHeight()/2 && 
       mouseY <= height/2+start.getFrameHeight()/2)
      start.setCurrentFrameColumn(1);
    else
      start.setCurrentFrameColumn(0);
  }
  else if(gameState == GameState.PLAYING)
  {
    if(!selecting)
      return;
    selectedItem.setXPos(mouseX);
  }
  else if(gameState == GameState.GAMEOVER)
  {
    if(mouseX >= width/3-exitSprite.getFrameWidth() && 
       mouseX <= width/3 &&
       mouseY >= height*2/3 && 
       mouseY <= height*2/3+exitSprite.getFrameHeight())
      exitSprite.setCurrentFrameColumn(1);
    else
      exitSprite.setCurrentFrameColumn(0);
  }
}

// OTHER FUNCTIONS
void initGame()
{
  // Falling items
  randomSeed(second());
  item.clear();
  addItem();
  
  // Time counters
  addItemTime = millis();
  addItemLimit = 5000;
  levelTime = millis();
  pointAnimTime = millis();
  
  // Flags
  selecting = false;
  scoring = false;
  score = 0;
  
  // gameState
  gameState = GameState.GAMEOVER;
}

void gameMechanics()
{
  // Background images
  imageMode(CORNER);
  for(int i=0; i<4; i++)
    image(background[i], i*backWidth, 0);
    
  // Global score
  scoreGlobal_sprite.draw();
    
  // Drop line
  strokeWeight(3);
  stroke(0);
  line(0, dropLine, width, dropLine);
  fill(0,0,0);
  textSize(16);
  textFont(font);
  text("DROP LINE", 0, dropLine-10);
    
  // Draw falling items
  for(int i=0; i<item.size(); i++)
  {
    FallingItem auxItem = item.get(i);
    auxItem.draw();
    auxItem.move();
    if(auxItem.getYPosition() > dropLine && !auxItem.isDropping())
    {
      auxItem.drop();
      if(!scoring && scoreValid(auxItem.category, 
                                int(auxItem.getXPosition())))
      {
        score++;
        scoreGlobal_sprite.nextFrame();
        score10_sprite.setPosition(int(auxItem.getXPosition()), 
                                   int(auxItem.getYPosition()));
        scoring = true;
      }
    }
    if(!auxItem.isVisible())
      item.remove(i);
  }
  
  // Scoring
  if(scoring)
    score10_sprite.draw();
  
  // Add a new item every 5 seconds
  if(millis() - addItemTime > addItemLimit)
  {
    addItem();
    addItemTime = millis();
  }
  if(millis() - pointAnimTime > 100)
  {
    score10_sprite.nextFrame();
    if(score10_sprite.getCurrentFrame() == 0)
      scoring = false;
    pointAnimTime = millis();
  }
  if(millis() - levelTime > 20000)
  {
    addItemLimit -= 500;
    if(addItemLimit <= 1000)
      gameState = GameState.GAMEOVER;
    levelTime = millis();
  }
}

void addItem()
{
  FallingItem auxItem;
  int category = int(random(4));
  int index;
  
  switch(category)
  {
  case 0:  // VEGGIES
    index = int(random(veggies.getNImages()));
    auxItem = new FallingItem(parachute, 
                              veggies.getElement(index).image);
    auxItem.category = FoodCategory.VEGGIES;
    break;
  case 1:  // GRAINS
    index = int(random(grains.getNImages()));
    auxItem = new FallingItem(parachute, 
                              grains.getElement(index).image);
    auxItem.category = FoodCategory.GRAINS;
    break;
  case 2:  // MILK
    index = int(random(milk.getNImages()));
    auxItem = new FallingItem(parachute, 
                              milk.getElement(index).image);
    auxItem.category = FoodCategory.MILK;
    break;
  default:  // MEAT
    index = int(random(meat.getNImages()));
    auxItem = new FallingItem(parachute, 
                              meat.getElement(index).image);
    auxItem.category = FoodCategory.MEAT;
    break;
  }
  item.add(auxItem);
}

boolean scoreValid(FoodCategory category, int x)
{
  if(x < backWidth && category == FoodCategory.VEGGIES)
    return true;
  if(x >= backWidth && x < 2*backWidth && 
     category == FoodCategory.GRAINS)
    return true;
  if(x >= 2*backWidth && x < 3*backWidth && 
     category == FoodCategory.MILK)
    return true;
  if(x >= 3*backWidth && x < width &&
     category == FoodCategory.MEAT)
    return true;
  return false;
}