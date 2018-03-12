//
//  ImageList.cpp
//  BoardGame
//
//  Created by Sheila Gonzalez on 31/01/18.
//  Copyright © 2018 Sheila Gonzalez. All rights reserved.
//

#include "ImageList.hpp"

// ***********************************************************************
// **********           CONSTRUCTORS AND DESTRUCTOR             **********
// ***********************************************************************
ImageList::ImageList()
{
    imageHasAlphaChannel = false;
    numImages = 0;
}

ImageList::ImageList(std::string path, std::string imageName, std::string imageExtension, int numImages)
{
    this->setPath(path);
    this->setImageName(imageName);
    this->setImageExtension(imageExtension);
    this->setNumImages(numImages);
    this->loadImageList();
}

ImageList::~ImageList()
{
    
}



// ***********************************************************************
// **********                   PUBLIC FUNCTIONS                **********
// ***********************************************************************
void ImageList::loadImageList()
{
    if(!this->imageList.empty())
        this->imageList.clear();
    if(!this->alphaMaskList.empty())
        this->alphaMaskList.clear();
    
    // baseName the portion of image name that is similar in all list. Example, in:
    // path/image_n.png
    // the baseName would be:
    // path/image_
    std::string baseName = this->path;
    baseName += "/";
    baseName += this->imageName;
    baseName += "_";
    
    // Image list starts with image number 1
    for(int nImg=1; nImg <= numImages; nImg++)
    {
        // Create file name
        std::string fileName = baseName;
        fileName += std::to_string(nImg);   // add image number
        fileName += ".";
        fileName += this->imageExtension;   // add image extension
        // Read image, including alpha channel
        cv::Mat image = cv::imread(fileName, -1);
        // Verify if image has alpha channel. All images in list
        // are supposed to have the same number of channels
        if(image.channels() == 4)
        {
            std::vector< cv::Mat > imChannels(4);
            split(image, imChannels);
            // Isolate image alpha channel and invert (for displaying purposes)
            cv::Mat imAux = 255 - imChannels[3];
            cv::cvtColor(imAux, imAux, cv::COLOR_GRAY2BGR);
            this->alphaMaskList.push_back(imAux);
            this->imageHasAlphaChannel = true;
            // Add regular image to list (without alpha channel)
            cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
            this->imageList.push_back(image);
        }
        else
            this->imageList.push_back(image);
    }
}

void ImageList::loadImageList(std::string path, std::string imageName, std::string imageExtension, int numImages)
{
    this->setPath(path);
    this->setImageName(imageName);
    this->setImageExtension(imageExtension);
    this->setNumImages(numImages);
    this->loadImageList();
}

void ImageList::clear()
{
    this->imageList.clear();
    this->alphaMaskList.clear();
    this->imageHasAlphaChannel = false;
}

void ImageList::push_back(cv::Mat &image)
{
    if(this->imageList.empty())
    {
        if(image.channels() == 4)
        {
            std::vector< cv::Mat > imChannels(4);
            split(image, imChannels);
            // Isolate image alpha channel and invert
            cv::Mat imAux = 255 - imChannels[3];
            cv::cvtColor(imAux, imAux, cv::COLOR_GRAY2BGR);
            this->alphaMaskList.push_back(imAux);
            this->imageHasAlphaChannel = true;
            // Add regular image to list (without alpha channel)
            cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
            this->imageList.push_back(image);
        }
        else
            this->imageList.push_back(image);
    }
    else if(this->imageHasAlphaChannel && image.channels() == 4)
    {
        std::vector< cv::Mat > imChannels(4);
        split(image, imChannels);
        // Isolate image alpha channel and invert
        cv::Mat imAux = 255 - imChannels[3];
        cv::cvtColor(imAux, imAux, cv::COLOR_GRAY2BGR);
        this->alphaMaskList.push_back(imAux);
        // Add regular image to list (without alpha channel)
        cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
        this->imageList.push_back(image);
    }
}

void ImageList::drawElement(cv::Mat &image, int element, int xPosition, int yPosition)
{
    if(this->imageList.size() < element  ||  element < 0)
        return;
    if(this->imageList[element].rows+yPosition > image.rows || this->imageList[element].cols+xPosition > image.cols)
        return;
    
    cv::Rect rect(xPosition, yPosition, imageList[element].cols, imageList[element].rows);
    if(this->imageHasAlphaChannel)
    {
        cv::Mat imAux;
        cv::bitwise_and(image(rect), alphaMaskList[element], imAux);
        cv::bitwise_or(imAux, imageList[element], image(rect));
    }
    else
    {
        imageList[element].copyTo(image(rect));
    }
    
}

void ImageList::resizeImages(int width, int height)
{
    if(width <= 0 || height <= 0)
        return;
    
    for(int i=0; i<imageList.size(); i++)
    {
        cv::resize(imageList[i], imageList[i], cv::Size(width, height));
        cv::resize(alphaMaskList[i], alphaMaskList[i], cv::Size(width, height));
    }
}

void ImageList::resizeImages(float factor)
{
    if(factor <= 0)
        return;
    
    for(int i=0; i<imageList.size(); i++)
    {
        cv::resize(imageList[i], imageList[i], cv::Size(0, 0), factor, factor);
        cv::resize(alphaMaskList[i], alphaMaskList[i], cv::Size(0, 0), factor, factor);
    }
}




// ***********************************************************************
// **********                   PRIVATE FUNCTIONS               **********
// ***********************************************************************



// ***********************************************************************
// **********                   SET FUNCTIONS                   **********
// ***********************************************************************
void ImageList::setPath(std::string path)
{
    if(!this->path.empty())
        this->path.clear();
    this->path.assign(path);
}

void ImageList::setImageName(std::string imageName)
{
    if(!this->imageName.empty())
        this->imageName.clear();
    this->imageName.assign(imageName);
}

void ImageList::setImageExtension(std::string imageExtension)
{
    if(!this->imageExtension.empty())
        this->imageExtension.clear();
    this->imageExtension.assign(imageExtension);
}

void ImageList::setNumImages(int numImages)
{
    if(numImages > 0)
        this->numImages = numImages;
}


int ImageList::getNumImages()
{
    return this->numImages;
}

bool ImageList::listHasAlphaChannel()
{
    return imageHasAlphaChannel;
}

bool ImageList::empty()
{
    if(this->numImages > 0)
        return false;
    return true;
}