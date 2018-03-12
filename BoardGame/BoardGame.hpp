//
//  BoardGame.hpp
//  BoardGame
//
//  Created by Sheila Gonzalez on 22/01/18.
//  Copyright © 2018 Sheila Gonzalez. All rights reserved.
//

#ifndef BoardGame_hpp
#define BoardGame_hpp

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <opencv2/opencv.hpp>
#include "ImageList.hpp"

class BoardGame
{
public:
    BoardGame();
    
    void draw(cv::Mat &image);
    void init();
    void highlight(int x, int y, bool on=true);
    void highlightOff();
    void blow();
    
    void setBoardSize(int boardRows, int boardCols);
    void setBoardRows(int boardRows);
    void setBoardCols(int boardCols);
    void setBoardOrigin(int x, int y);
    void setBoardElementSize(int width, int height);
    void setSpaceBetweenElements(int spaceBetweenElements);
    void setCurrentPos(int x, int y);
    
    bool getHighlightState(int x, int y);
    
    ImageList imageList;
    
private:
    int boardRows, boardCols;
    int spaceBetweenElements, currentSelection;
    cv::Point origin, currentPos;
    cv::Size elementSize;
    std::vector< std::vector <int> > board;
    std::vector< std::vector <bool> > highlightBoard;
    std::vector< cv::Point > highlightVec;
    
    void createBoardMatrix();
    bool highlightBoardEmpty();
    bool highlightPositionValid(int R, int C);
    bool validBlow();
    bool validBoard();
};

#endif /* BoardGame_hpp */
