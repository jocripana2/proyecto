//
//  main.cpp
//  BoardGame
//
//  Created by Sheila Gonzalez on 22/01/18.
//  Copyright © 2018 Sheila Gonzalez. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "ImageList.hpp"
#include "BarControl.hpp"
#include "BoardGame.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
    VideoCapture cap(0);
    if(!cap.isOpened())
        return 1;
    
    Mat img;
    BoardGame boardGame;
    boardGame.setBoardOrigin(100, 50);
    boardGame.imageList.setPath("/Users/sheila/Documents/PostDocProj/img/food_icons/fruit");
    boardGame.imageList.setImageName("icon");
    boardGame.imageList.setImageExtension("png");
    boardGame.imageList.setNumImages(10);
    boardGame.imageList.loadImageList();
    boardGame.imageList.resizeImages(50, 50);
    boardGame.init();
    
    int x = 110, y=70;
    bool running = true;
    while(running){
        cap >> img;
        if(img.empty())
            continue;
        flip(img, img, 1);
        
        boardGame.setCurrentPos(x,y);
        boardGame.draw(img);
        
        imshow("Imagen", img);
        int key = waitKey(5);
        switch (key) {
            case 'a':
            case 'A':
                x -= 50;
                if(x < 110)
                    x = 360;
                break;
            case 's':
            case 'S':
                y += 50;
                if(y > 270)
                    y =70;
                break;
            case 'd':
            case 'D':
                x += 50;
                if(x > 360)
                    x = 110;
                break;
            case 'w':
            case 'W':
                y -= 50;
                if(y < 70)
                    y = 270;
                break;
            case 'p':
            case 'P':
                boardGame.blow();
                break;
            case 13:
                boardGame.highlight(x, y, !boardGame.getHighlightState(x, y));
                break;
            case 27:
                running = false;
                break;
        }
    }
    cap.release();
    return 0;
}

/*int main(int argc, const char * argv[]) {
    VideoCapture cap(0);
    if(!cap.isOpened())
        return 1;
    
    Mat img;
    ImageList list;
    list.loadImageList("/Users/sheila/Documents/PostDocProj/img/Avatar/newStretch",
                   "avatar", "png", 4);
    list.resizeImages(0.1);
    
    BarControl bar;
    bar.setOrigin(500, 100);
    bar.setLevel(1.0);
    
    float level = 1.0;
    float inc = -0.01f;
    int nAvatar = 0;
    while(1){
        cap >> img;
        if(img.empty())
            continue;
        flip(img, img, 1);
        
        list.drawElement(img, nAvatar, 200, 100);
        bar.draw(img);
        imshow("Imagen", img);
        if(waitKey(5) == 27)
            break;
        
        
        level += inc;
        if(level < 0)
        {
            level = 1.0f;
            nAvatar++;
            if(nAvatar > 3)
                nAvatar = 0;
        }
        bar.setLevel(level);
    }
    cap.release();
    
    return 0;
}*/
