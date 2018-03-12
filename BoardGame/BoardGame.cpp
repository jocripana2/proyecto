//
//  BoardGame.cpp
//  BoardGame
//
//  Created by Sheila Gonzalez on 22/01/18.
//  Copyright © 2018 Sheila Gonzalez. All rights reserved.
//

#include "BoardGame.hpp"

// ***********************************************************************
// **********           CONSTRUCTORS AND DESTRUCTOR             **********
// ***********************************************************************
BoardGame::BoardGame()
{
    boardRows = 4;
    boardCols = 5;
    elementSize = cv::Size(50,50);
    spaceBetweenElements = 10;
    origin = cv::Point(0,0);
    currentPos = cv::Point(0,0);
    currentSelection = -1;
    
    createBoardMatrix();
}



// ***********************************************************************
// **********                   PUBLIC FUNCTIONS                **********
// ***********************************************************************
void BoardGame::draw(cv::Mat &image)
{
    cv::Mat aux;
    aux.create(image.rows, image.cols, CV_8UC3);
    aux = cv::Scalar::all(0);
    
    // First, draw the icons on a black auxiliar image
    for(int i=0; i<boardRows; i++)
    {
        for(int j=0; j<boardCols; j++)
        {
            int x = j*(elementSize.width+spaceBetweenElements)+origin.x;
            int y = i*(elementSize.height+spaceBetweenElements)+origin.y;
            
            if(!board.empty())
                imageList.drawElement(aux, board[i][j], x, y);
        }
    }
    // Then mix the black auxiliar image with the current frame
    // passed by argument. This way, board will have a transparency
    cv::addWeighted(image, 0.5, aux, 0.5, 0, image);
    
    // Draw again all currently selected icons on board, so they do not look
    // transparent (highlight) and draw rectangles around icons.
    for(int i=0; i<boardRows; i++)
        for(int j=0; j<boardCols; j++)
        {
            int x = j*(elementSize.width+spaceBetweenElements)+origin.x;
            int y = i*(elementSize.height+spaceBetweenElements)+origin.y;
            if(highlightBoard[i][j])
            {
                imageList.drawElement(image, board[i][j], x, y);
                cv::rectangle(image,cv::Rect(x,y,elementSize.width,elementSize.height),
                              cv::Scalar(0,0,255), 2);
            }
            else
                cv::rectangle(image,cv::Rect(x,y,elementSize.width,elementSize.height),
                              cv::Scalar(80,80,80), 2);
        }
    
    // Current position will appear on a different color
    int x = currentPos.x*(elementSize.width+spaceBetweenElements)+origin.x;
    int y = currentPos.y*(elementSize.height+spaceBetweenElements)+origin.y;
    cv::rectangle(image,cv::Rect(x,y,elementSize.width,elementSize.height),
                  cv::Scalar(0,255,255), 2);
}

void BoardGame::init()
{
    if(boardRows <= 0 && boardCols <= 0)
        return;
    
    // Assign random values to the board, each integer value represent a different
    // icon. Verify if the new board is valid (has at least one chain to break),
    // and reassign new values when it is not.
    std::srand((unsigned int)std::time(NULL));
    do{
        for(int i=0; i<boardRows; i++)
        {
            for(int j=0; j<boardCols; j++)
            {
                board[i][j] = rand() % imageList.getNumImages();
                highlightBoard[i][j] = false;
            }
        }
    }while(!validBoard());
    
    // Initialize other important variables
    if(!highlightVec.empty())
        highlightVec.clear();
    currentPos = cv::Point(0,0);
    currentSelection = -1;
}

void BoardGame::highlight(int x, int y, bool on)
{
    int C = (int)(x - origin.x) / (int)(elementSize.width+spaceBetweenElements);
    int R = (int)(y - origin.y) / (int)(elementSize.height+spaceBetweenElements);
    
    // If (R,C) is a valid position
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
            // Verify current selection matches highlight position. It it does
            // not match, then do not highlight this element.
            if(currentSelection == board[R][C] && highlightPositionValid(R, C))
                highlightBoard[R][C] = on;
            return;
        }
        if(currentSelection >= 0 && on == false)
        {
            // Verify if highlightBoard has only one element active,
            // if so, deactivate currentSelection
            highlightBoard[R][C] = on;
            if(highlightBoardEmpty())
                currentSelection = -1;
        }
    }
}

void BoardGame::highlightOff()
{
    // Put all hightlightBoard to false, so the
    // selection can start again.
    for(int i=0; i<boardRows; i++)
        for(int j=0; j<boardCols; j++)
            highlightBoard[i][j] = false;
    currentSelection = -1;
}

void BoardGame::blow()
{
    if(!validBlow())
        return;
    
    std::srand((unsigned int)std::time(NULL));
    // All the elements above the blow position, will slide down.
    // Empty elements will be replaced by new random values.
    for(int i=0; i<boardRows; i++)
    {
        for(int j=0; j<boardCols; j++)
            if(highlightBoard[i][j])
            {
                if(i-1 >= 0)
                    for(int R=i; R>0; R--)
                        board[R][j] = board[R-1][j];
                board[0][j] = rand() % imageList.getNumImages();
            }
    }
    // Reassign new random values if board is not valid
    while(!validBoard())
    {
        for(int i=0; i<boardRows; i++)
        {
            for(int j=0; j<boardCols; j++)
                if(highlightBoard[i][j])
                {
                    board[0][j] = rand() % imageList.getNumImages();
                }
        }
    }
    highlightOff();
}



// ***********************************************************************
// **********                   PRIVATE FUNCTIONS               **********
// ***********************************************************************
void BoardGame::createBoardMatrix()
{
    if(!board.empty())
    {
        board.clear();
        highlightBoard.clear();
    }
    
    // New memory allocation to board. This
    // function is called when creating a new game, or
    // board size has changed
    std::vector< int > vecAux(boardCols);
    std::vector< bool > vecBoolAux(boardCols);
    for(int i=0; i<boardRows; i++)
    {
        board.push_back(vecAux);
        highlightBoard.push_back(vecBoolAux);
    }
}

bool BoardGame::highlightBoardEmpty()
{
    // Check if there is any element highlighted
    for(int i=0; i<boardRows; i++)
        for(int j=0; j<boardCols; j++)
            if(highlightBoard[i][j])
                return false;
    
    return true;
}

bool BoardGame::highlightPositionValid(int R, int C)
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

bool BoardGame::validBlow()
{
    // If the chaing you want to blow has three or more elements,
    // then you can blow it
    int count=0;
    
    for(int i=0; i<boardRows; i++)
        for(int j=0; j<boardCols; j++)
            if(highlightBoard[i][j])
                count++;
    if(count >= 3)
        return true;
    return false;
}

bool BoardGame::validBoard()
{
    // Verify there is at least one chain of three elements to blow
    
    // Auxiliar board to save if one position has already been tested.
    // Initial values in this board will all be initially false.
    std::vector< std::vector< bool > > auxBoard;
    for(int i=0; i<boardRows; i++)
    {
        std::vector<bool> m;
        m.assign(boardCols, false);
        auxBoard.push_back(m);
    }
    
    // Test all positions on the real board
    for(int i=0; i<boardRows; i++)
    {
        for(int j=0; j<boardCols; j++)
        {
            if(auxBoard[i][j])    // If position has been tested
                continue;
            // Create a stack of positions. This represents all
            // positions in the 8-neighborhood of (i,j) that share
            // the same icon
            std::queue< cv::Point > pos;
            pos.push(cv::Point(i,j));
            int count=1;
            auxBoard[i][j] = true;
            do{
                // Test all 8-neighborhood
                for(int r=pos.front().x-1; r<=pos.front().x+1; r++)
                {
                    if(r < 0 || r >= boardRows) continue;
                    for(int c=pos.front().y-1; c<=pos.front().y+1; c++)
                    {
                        if(c < 0 || c >= boardCols) continue;
                        if(r == c) continue;
                        if(auxBoard[r][c]) continue;
                        if(board[r][c] == board[i][j])
                        {
                            pos.push(cv::Point(r,c));
                            auxBoard[r][c] = true;
                            count++;
                        }
                    }
                }
                // remove the first element in queue
                pos.pop();
            }while(pos.size() > 0);
            if(count >= 3)
                return true;
        }
    }
    
    return false;
}



// ***********************************************************************
// **********                   SET FUNCTIONS                   **********
// ***********************************************************************
void BoardGame::setBoardSize(int boardRows, int boardCols)
{
    if(boardRows >= 0)
        this->boardRows = boardRows;
    if(boardCols >= 0)
        this->boardCols = boardCols;
    
    createBoardMatrix();
}

void BoardGame::setBoardRows(int boardRows)
{
    if(boardRows >= 0)
        this->boardRows = boardRows;
    createBoardMatrix();
}

void BoardGame::setBoardCols(int boardCols)
{
    if(boardCols >= 0)
        this->boardCols = boardCols;
    createBoardMatrix();
}

void BoardGame::setBoardOrigin(int x, int y)
{
    if(x >= 0 && y >= 0)
        this->origin = cv::Point(x,y);
}

void BoardGame::setBoardElementSize(int width, int height)
{
    if(width > 0 && height > 0)
        this->elementSize = cv::Size(width, height);
}

void BoardGame::setSpaceBetweenElements(int spaceBetweenElements)
{
    if(spaceBetweenElements >= 0)
        this->spaceBetweenElements = spaceBetweenElements;
}

void BoardGame::setCurrentPos(int x, int y)
{
    currentPos.x = (int)(x - origin.x) / (int)(elementSize.width+spaceBetweenElements);
    currentPos.y = (int)(y - origin.y) / (int)(elementSize.height+spaceBetweenElements);
}

bool BoardGame::getHighlightState(int x, int y)
{
    int C = (int)(x - origin.x) / (int)(elementSize.width+spaceBetweenElements);
    int R = (int)(y - origin.y) / (int)(elementSize.height+spaceBetweenElements);
    
    if(R >= 0 && R < boardRows && C >= 0 && C <= boardCols)
        return highlightBoard[R][C];
    return false;
}