// *********************************************************
//                  ImageList loader
// *********************************************************
class ImageList
{
  ///////////////////////////////////////////////
  class ImageObject
  {
    public ImageObject()
    {
      xPos = 0;
      yPos = 0;
    }
    
    public ImageObject(String filename)
    {
      xPos = 0;
      yPos = 0;
      image = loadImage(filename);
    }
    
    public void resize(float factor)
    {
      image.resize(int(image.width*factor), int(image.height*factor));
    }
    
    public void resize(int w, int h)
    {
      image.resize(w, h);
    }
    
    public PImage image;
    public int xPos, yPos;
  };
  ///////////////////////////////////////////////
  
  
  
  // ********** CONSTRUCTORS
  public ImageList()
  {
    nImages = 0;
  }
  
  public ImageList(String path, String name, int nFiles)
  {
    loadList(path, name, nFiles);
  }
  
  // ********** OTHER PUBLIC FUNCTIONS
  public void loadList(String path, String name, int nFiles)
  {
    nImages = nFiles;
    list = new ArrayList<ImageObject>();
    for(int i=0; i<nImages; i++)
    {
      String filename = path + "/" + name + "_" + i + ".png";
      list.add(new ImageObject(filename));
    }
  }
  
  public void resizeAll(float factor)
  {
    for(int i=0; i<nImages; i++)
      list.get(i).resize(factor);
  }
  
  public void resizeAll(int w, int h)
  {
    for(int i=0; i<nImages; i++)
      list.get(i).resize(w, h);
  }
  
  public void resizeElement(int index, float factor)
  {
    list.get(index).resize(factor);
  }
  
  public void resizeElement(int index, int w, int h)
  {
    list.get(index).resize(w,h);
  }
  
  public void drawElement(int index)
  {
    ImageObject obj = list.get(index);
    image(obj.image, obj.xPos, obj.yPos);
  }
  
  public void drawElement(int index, int xPos, int yPos)
  {
    image(list.get(index).image, xPos, yPos);
  }
  
  public ImageObject getElement(int index)
  {
    return list.get(index);
  }
  
  public int getNImages()
  {
    return nImages;
  }
   
  // ********** PRIVATE VARIABLES
  private ArrayList<ImageObject> list;
  private int nImages;
};