class FallingItem
{ 
  // CONSTRUCTORS
  public FallingItem(PImage itemUp, PImage itemDown)
  {
    this.itemUp = itemUp;
    this.itemDown = itemDown;
    
    xUp = int(random(width - itemUp.width*2)) + itemUp.width;
    xDown = xUp + itemUp.width/2 - itemDown.width/2;
    yUp = -itemUp.height/2 - itemDown.height;
    yDown = -itemDown.height;
    velUp = velDown = 0.5;
    dropping = false;
  }
  
  public void draw()
  {
    //imageMode(CENTER);
    image(itemUp, xUp, yUp);
    image(itemDown, xDown, yDown);
  }
  
  public void move()
  {
    yUp += velUp;
    yDown += velDown;
  }
  
  public void drop()
  {
    if(dropping)
      return; 
      
    velUp = -2*velUp;
    velDown = 2*velDown;
    dropping = true;
  }
  
  // SET AND GET FUNCTIONS 
  public void setVelocity(float velocity)
  {
    this.velUp = this.velDown = velocity;
  }
  
  public void setXPos(float x)
  {
    this.xUp = x;
    this.xDown = xUp + itemUp.width/2 - itemDown.width/2;
  }
  
  public float getXPosition()
  {
    return this.xDown;
  }
  
  public float getYPosition()
  {
    return yDown;
  }
  
  public boolean isDropping()
  {
    return dropping;
  }
  
  public boolean isVisible()
  {
    if(yUp < -height)
      return false;
    return true;
  }
  
  public boolean isGrabbed(float x, float y)
  {
    if(x >= xUp && x <= xUp+itemUp.width &&
       y >= yUp && y <= yUp+itemUp.height+itemDown.height/2)
       return true;
    return false;
  }
  
  private PImage itemUp, itemDown;
  private float xUp, xDown, yUp, yDown;
  private float velUp, velDown;
  private boolean dropping;
  public FoodCategory category;
};