//
//  BarControl.cpp
//  BoardGame
//
//  Created by Sheila Gonzalez on 31/01/18.
//  Copyright © 2018 Sheila Gonzalez. All rights reserved.
//

#include "BarControl.hpp"

// ***********************************************************************
// **********           CONSTRUCTORS AND DESTRUCTOR             **********
// ***********************************************************************
BarControl::BarControl()
{
    level = 0.5f;
    minLevel = 0.0f;
    maxLevel = 1.0f;
    color = cv::Scalar(0,128,0);
    origin = cv::Point(20,20);
    size = cv::Size(200,20);
    barSize = cv::Size(100,20);
}

BarControl::~BarControl()
{
    
}



// ***********************************************************************
// **********                   PUBLIC FUNCTIONS                **********
// ***********************************************************************
void BarControl::draw(cv::Mat &image)
{
    // Background
    cv::rectangle(image, cv::Rect(origin, size), cv::Scalar(255,255,255), -1);
    // Bar
    cv::rectangle(image, cv::Rect(origin, barSize), this->color, -1);
    // Border
    cv::rectangle(image, cv::Rect(origin, size), cv::Scalar(0,0,0), 2);
}



// ***********************************************************************
// **********                   SET FUNCTIONS                   **********
// ***********************************************************************
void BarControl::setOrigin(int x, int y)
{
    if(x >= 0 && y >= 0)
        this->origin = cv::Point(x,y);
}

void BarControl::setOriginX(int x)
{
    if(x >= 0)
        this->origin.x = x;
}

void BarControl::setOriginY(int y)
{
    if(y >= 0)
        this->origin.y = y;
}

void BarControl::setSize(int width, int height)
{
    if(width >= 0 && height >= 0)
        this->size = cv::Size(width, height);
}

void BarControl::setWidth(int width)
{
    if(width >= 0)
        this->size.width = width;
}

void BarControl::setHeight(int height)
{
    if(height >= 0)
        this->size.height = height;
}

void BarControl::setLevel(float level)
{
    if(minLevel >= maxLevel)
        return;
    
    if(level >= minLevel && level <= maxLevel)
        this->level = level;
    if(level <= minLevel)
        this->level = minLevel;
    if(level >= maxLevel)
        this->level = maxLevel;
    
    barSize.width = (this->level - minLevel) * size.width / (maxLevel - minLevel);
}

void BarControl::setMinLevel(float minLevel)
{
    this->minLevel = minLevel;
}

void BarControl::setMaxLevel(float maxLevel)
{
    this->maxLevel = maxLevel;
}

void BarControl::setColor(int R, int G, int B)
{
    if(R >= 0 && R <= 255 &&
       G >= 0 && G <= 255 &&
       B >= 0 && B <= 255)
        this->color = cv::Scalar(B,G,R);
}