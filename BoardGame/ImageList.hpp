/***********************************************************************
                        ImageList CLASS
************************************************************************
 - Reads a list of images from a given path. All images have similar
 names, and a given consecutive number. For example:
        path/imageName_1.png
        path/imageName_2.png
        ...
        path/imageName_n.png
 path:      variable representing the physical path to image list
 imageName: variable representing the base name for the image list
 
 - Images with alpha channel can be used and displayed in front of 
 a given image.
 - Initial imageNumber is 1.
 - All images have the same extension
***********************************************************************/

#ifndef ImageList_hpp
#define ImageList_hpp

#include <cstdio>
#include <opencv2/opencv.hpp>

class ImageList
{
public:
    // ********** CONSTRUCTORS
    ImageList();
    ImageList(std::string path, std::string imageName, std::string imageExtension, int numImages);
    ~ImageList();
    
    // ********** PUBLIC FUNCTIONS
    void loadImageList();
    void loadImageList(std::string path, std::string imageName, std::string imageExtension, int numImages);
    void clear();
    void push_back(cv::Mat &image);
    void drawElement(cv::Mat &image, int element, int xPosition, int yPosition);
    void resizeImages(int width, int height);
    void resizeImages(float factor);
    
    // ********** SET AND GET FUNCTIONS
    void setPath(std::string path);
    void setImageName(std::string imageName);
    void setImageExtension(std::string imageExtension);
    void setNumImages(int numImages);
    
    int getNumImages();
    bool listHasAlphaChannel();
    bool empty();
    
private:
    std::string path, imageName, imageExtension;
    bool imageHasAlphaChannel;
    int numImages;
    std::vector< cv::Mat > imageList, alphaMaskList;
    
    
    // ********** PRIVATE FUNCTIONS
};

#endif /* ImageList_hpp */
