//
//  BarControl.hpp
//  BoardGame
//
//  Created by Sheila Gonzalez on 31/01/18.
//  Copyright © 2018 Sheila Gonzalez. All rights reserved.
//

#ifndef BarControl_hpp
#define BarControl_hpp

#include <cstdio>
#include <cstdlib>
#include <opencv2/opencv.hpp>

class BarControl
{
public:
    BarControl();
    ~BarControl();
    
    void draw(cv::Mat &image);
    
    void setOrigin(int x, int y);
    void setOriginX(int x);
    void setOriginY(int y);
    void setSize(int width, int height);
    void setWidth(int width);
    void setHeight(int height);
    void setLevel(float level);
    void setMinLevel(float minLevel);
    void setMaxLevel(float maxLevel);
    void setColor(int R, int G, int B);
    
private:
    cv::Point origin;
    cv::Size size, barSize;
    cv::Scalar color;
    float level;
    float minLevel, maxLevel;
};

#endif /* BarControl_hpp */
