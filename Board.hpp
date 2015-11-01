#ifndef _BOARD_H
#define _BOARD_H

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdlib.h>
#include <queue>
#include <unordered_set>

#include "Constants.hpp"
#include "OthelloPoint.hpp"

#define BoardWidth 8
#define BoardHeight 8

class Board
{
public:
  Board();
  Board(std::ifstream& savedBoardFile, int* timeoutSeconds);
  void DisplayBoard();
  
  // returns true if EOG
  bool MakeMove(Color color, OthelloPoint othelloPoint);
  
  Board GetResultantBoard(Color color, OthelloPoint othelloPoint);
  
  bool IsLegalMove(Color color, OthelloPoint othelloPoint);
  std::vector<OthelloPoint> GetAllLegalMoves(Color color);
  Color GetTurn();
  Color GetOpposingColor(Color color);
  
  int GetScore(Color color, int& detailedPointScore, int& frontierCount);
  int GetNumCornersOccupied(Color color);
  
private:
  Board(SpaceHolder currentBoard[BoardWidth][BoardHeight], SpaceHolder legalMoves[BoardWidth][BoardHeight],
	Color turn, OthelloPoint move);
  
  SpaceHolder GetOpposingColor(SpaceHolder color);
  void UpdateLegalMoves(bool flip = false, Color color = BlackPlayer, OthelloPoint point = OthelloPoint(0,0));
  void PrintColorSpace(SpaceHolder spaceHolder, SpaceHolder legalMove);

  Color _turn;
  SpaceHolder _board[BoardWidth][BoardHeight];
  SpaceHolder _legalMovesBoard [BoardWidth][BoardHeight];
  
  std::unordered_set<OthelloPoint> whiteLegalMovesSet;
  std::unordered_set<OthelloPoint> blackLegalMovestSet;
  void AddToLegalSet(SpaceHolder color, OthelloPoint point);
    
  std::priority_queue<OthelloPoint, std::vector<OthelloPoint>, OthelloPointComparer> movesQueue;
  
  int _pieceScore[8][8] = {
    {20, -3, 11, 8, 8, 11, -3, 20},
    {-3, -7, -4, 1, 1, -4, -7, -3},
    {11, -4, 2, 2, 2, 2, -4, 11},
    {8, 1, 2, -3, -3, 2, 1, 8},
    {8, 1, 2, -3, -3, 2, 1, 8},
    {11, -4, 2, 2, 2, 2, -4, 11},
    {-3, -7, -4, 1, 1, -4, -7, -3},
    {20, -3, 11, 8, 8, 11, -3, 20}
  };
  
  int _xDirections[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
  int _yDirections[8] = {0, 1, 1, 1, 0, -1, -1, -1};
};

#endif