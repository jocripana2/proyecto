// *********************************************************
//                 Simple 2D Sprite handling
// This version handles images with R rows and C columns
// of sprites.
// 
// load        - Loads image, states number of frames of sprite
// draw        - Draws current frame
// nextFrame   - Moves to next frame (cyclic)
// resize      - Resizes image by a factor (holding aspect ratio)
// setPosition - States position where the sprite will be drawn
// *********************************************************
class Sprite
{
  // ********** CONSTRUCTORS
  public Sprite()
  {
    nRows = 0;
    nCols = 0;
    currentRow = 0;
    currentCol = 0;
    frameWidth = 0;
    frameHeight = 0;
    xPos = 0;
    yPos = 0;
  }
  
  public Sprite(String filename, int nRows, int nCols)
  {
    load(filename, nRows, nCols);
    xPos = 0;
    yPos = 0;
  }
  
  
  
  // ********** PUBLIC FUNCTIONS
  public boolean load(String filename, int nRows, int nCols)
  {
    image = loadImage(filename);
    if(image.width <= 0 || image.height <= 0)
      return false;
      
    this.nRows = nRows;
    this.nCols = nCols;
    this.currentRow = 0;
    this.currentCol = 0;
    this.frameWidth = float(image.width) / float(nCols);
    this.frameHeight = float(image.height) / float(nRows);
    
    return true;
  }
  
  public void draw()
  {
    if(nRows == 0 || nCols == 0)
      return;
      
    PImage imAux = image.get(int(currentCol*frameWidth), 
                             int(currentRow*frameHeight), 
                             int(frameWidth), 
                             int(frameHeight));
    image(imAux, xPos, yPos);
  }
  
  public void nextFrame()
  {
    currentCol++;
    if(currentCol >= nCols)
    {
      currentCol = 0;
      currentRow++;
      if(currentRow >= nRows)
        currentRow = 0;
    }
  }
  
  public void resize(float factor)
  {
    image.resize(int(image.width*factor), int(image.height*factor));
    frameWidth = int(frameWidth * factor);
    frameHeight = int(frameHeight * factor);
  }
  
  
  
  // ********** GET AND SET FUNCTIONS
  public void setPosition(int xPos, int yPos)
  {
    this.xPos = xPos;
    this.yPos = yPos;
  }
  
  public void setCurrentFrameColumn(int col)
  {
    if(col >= 0 && col < nCols)
      this.currentCol = col;
  }
  
  public void setCurrentFrameRow(int row)
  {
    if(row >= 0 && row < nRows)
      this.currentRow = row;
  }
  
  public void setCurrentFrame(int row, int col)
  {
    if(row >= 0 && row < nRows)
      this.currentRow = row;
      
    if(col >= 0 && col < nCols)
      this.currentCol = col;
  }
  
  public int getCurrentFrame()
  {
    return (this.currentRow*nCols + this.currentCol);
  }
  
  public int getFrameWidth()
  {
    return int(frameWidth);
  }
  
  public int getFrameHeight()
  {
    return int(frameHeight);
  }
  


  // ********** PRIVATE VARIABLES
  private PImage image;
  private int nRows, nCols;
  private int currentRow, currentCol;
  private float frameWidth, frameHeight;
  private float xPos, yPos;
};