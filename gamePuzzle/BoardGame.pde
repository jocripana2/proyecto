public class BoardGame
{
  ///////////////////////////////////////////////
  class Point
  {
    Point(int x, int y)
    {
      this.x = x;
      this.y = y;
    }
    
    public int x, y;
  };
  ///////////////////////////////////////////////
  
  
  
  // **************************************************
  // **********         CONSTRUCTORS         **********
  public BoardGame()
  {
    boardRows = 4;
    boardCols = 5;
    nElements = boardRows*boardCols;
    iconWidth = 50;
    iconHeight = 50;
    spaceBetweenElements = 10;
    origin = new Point(0,0);
    currentPos = new Point(0,0);
    currentSelection = -1;
    
    board = new int[boardRows][boardCols];
    highlightBoard = new boolean[boardRows][boardCols];
    highlightVec = new Point[nElements];
    for(int i=0; i<nElements; i++)
      highlightVec[i] = new Point(-1,-1);
  }
  
  public BoardGame(int boardRows, int boardCols)
  {
    this.boardRows = boardRows;
    this.boardCols = boardCols;
    nElements = boardRows*boardCols;
    iconWidth = 50;
    iconHeight = 50;
    spaceBetweenElements = 10;
    origin = new Point(0,0);
    currentPos = new Point(0,0);
    currentSelection = -1;
    
    board = new int[boardRows][boardCols];
    highlightBoard = new boolean[boardRows][boardCols];
    highlightVec = new Point[nElements];
    for(int i=0; i<nElements; i++)
      highlightVec[i] = new Point(-1,-1);
  }
  
  // **************************************************
  // **********       PUBLIC FUNCTIONS       **********
   public void draw()
   {
     strokeWeight(2);
     noFill();
     
     for(int i=0; i<boardRows; i++)
     {
       for(int j=0; j<boardCols; j++)
       {
         int x = j*(iconWidth+spaceBetweenElements) + origin.x;
         int y = i*(iconHeight+spaceBetweenElements) + origin.y;
         // First, draw the icons of the board
         imageList.drawElement(board[i][j], x, y);
         // Highlighted elements
         if(highlightBoard[i][j])
           stroke(#FF0000);
         else
           stroke(#808080);
         rect(x, y, iconWidth, iconHeight);
       }
     }
     
     int x = currentPos.x*(iconWidth+spaceBetweenElements) + origin.x;
     int y = currentPos.y*(iconHeight+spaceBetweenElements) + origin.y;
     stroke(#FFFF00);
     rect(x, y, iconWidth, iconHeight);
   }
 
   public void init()
   {
     // Assign random values to the board, each integer value represent
     // a different icon. Verify if the new board is valid (has at least
     // one chain to break), and reassign new values when it is not.
     randomSeed(millis());
     float N = imageList.getNImages();
     do{
       for(int i=0; i<boardRows; i++)
       {
         for(int j=0; j<boardCols; j++)
         {
           board[i][j] = int(random(N));
           highlightBoard[i][j] = false;
         }
       }
     }while(!validBoard());
     
     currentPos.x = 0; currentPos.y = 0;
     currentSelection = -1;
   }
 
   public void highlight(int x, int y, boolean on)
   {
     int C = (x-origin.x) / (iconWidth+spaceBetweenElements);
     int R = (y-origin.y) / (iconHeight+spaceBetweenElements);
     
     if(R >= 0 && R < boardRows && C >= 0 && C < boardCols)
     {
       if(currentSelection == -1 && on == true)
       {
         // if currentSelection deactivated, activate
         highlightBoard[R][C] = on;
         currentSelection = board[R][C];
         return;
       }
       if(currentSelection >= 0 && on == true)
       {
         // Verify current selection matches highlight position. 
         // If it does not match, then do not highlight this element
         if(currentSelection == board[R][C] && highlightPositionValid(R,C))
           highlightBoard[R][C] = on;
         return;
       }
       if(currentSelection >= 0 && on == false)
       {
         // Verify if highlightBoard has only one element active,
         // if so, deactivate current selection
         highlightBoard[R][C] = on;
         if(highlightBoardEmpty())
           currentSelection = -1;
       }
     }
   }
   
   public void highlightOff()
   {
     // Put all highlightBoard to false, so the
     // selection can start again.
     for(int i=0; i<boardRows; i++)
       for(int j=0; j<boardCols; j++)
         highlightBoard[i][j] = false;
     currentSelection = -1;
   }
   
   public void blow()
   {
     if(!validBlow())
       return;
       
     randomSeed(millis());
     // All the elements above the blow position, will slide down.
     // Empty elements will be replaced by new random values
     for(int i=0; i<boardRows; i++)
     {
       for(int j=0; j<boardCols; j++)
       {
         if(highlightBoard[i][j])
         {
           if(i-1 >= 0)
             for(int R=i; R>0; R--)
             {
               board[R][j] = board[R-1][j];
             }
           board[0][j] = int(random(imageList.getNImages()));
         }
       }
     }
     
     // Reassign new random values if board is not valid
     while(!validBoard())
     {
       for(int i=0; i<boardRows; i++)
         for(int j=0; j<boardCols; j++)
           if(highlightBoard[i][j])
             board[0][j] = int(random(imageList.getNImages()));
     }
     highlightOff();
   }
  
  // **************************************************
  // **********     SET AND GET FUNCTIONS    **********
  public void setBoardOrigin(int x, int y)
  {
    if(x >= 0 && y >= 0)
    {
      origin.x = x;
      origin.y = y;
    }
  }
  
  public void setBoardElementSize(int w, int h)
  {
    iconWidth = w;
    iconHeight = h;
  }
  
  public void setSpaceBetweenElements(int spaceBetweenElements)
  {
    this.spaceBetweenElements = spaceBetweenElements;
  }
  
  public void setCurrentPos(int x, int y)
  {
    int C = (x-origin.x) / (iconWidth+spaceBetweenElements);
    int R = (y-origin.y) / (iconHeight+spaceBetweenElements);
    
    if(C < 0 || C >= boardCols || R < 0 || R >= boardRows)
      return;
      
    currentPos.x = C;
    currentPos.y = R;
  }
  
  public boolean getHighlightState(int x, int y)
  {
    int C = (x-origin.x) / (iconWidth+spaceBetweenElements);
    int R = (y-origin.y) / (iconHeight+spaceBetweenElements);
    
    if(R>=0 && R<boardRows && C>= 0 && C<=boardCols)
      return highlightBoard[R][C];
    return false;
  }
  
  // **************************************************
  // **********       PRIVATE FUNCTIONS      **********
  private boolean highlightBoardEmpty()
  {
    for(int i=0; i<boardRows; i++)
        for(int j=0; j<boardCols; j++)
          if(highlightBoard[i][j])
            return false;
    return true;
  }
  
  private boolean highlightPositionValid(int R, int C)
  {
    // Verify that position you want to highlight is neighboard
    // of the last highlighted element
    for(int i=R-1; i<=R+1; i++)
    {
      if(i<0 || i>=boardRows)
        continue;
      for(int j=C-1; j<=C+1; j++)
      {
        if(j<0 || j>=boardCols || (i == R && j == C))
          continue;
        if(highlightBoard[i][j])
          return true;
      }
    }
    return false;
  }
  
  private boolean validBlow()
  {
    int count = 0;
    
    for(int i=0; i<boardRows; i++)
      for(int j=0; j<boardCols; j++)
        if(highlightBoard[i][j])
          count++;
          
    if(count >= 3)
      return true;
    return false;
  }
  
  private boolean validBoard()
  {
    return true;
  }

  
  // **************************************************
  // **********       PRIVATE VARIABLES      **********
  private int boardRows, boardCols, nElements;
  private int spaceBetweenElements, currentSelection;
  private Point origin, currentPos;
  private int iconWidth, iconHeight;
  private int[][] board;
  private boolean[][] highlightBoard;
  private Point[] highlightVec;
  
  // **************************************************
  // **********       PUBLIC VARIABLES       **********
  ImageList imageList;
};