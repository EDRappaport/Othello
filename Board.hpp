#ifndef _BOARD_H
#define _BOARD_H

#include <iostream>
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
  Board(Color firstPlayer);
  void DisplayBoard();
  
  // returns true if EOG
  bool MakeMove(Color color, OthelloPoint othelloPoint);
  
  Board GetResultantBoard(Color color, OthelloPoint othelloPoint);
  
  bool IsLegalMove(Color color, OthelloPoint othelloPoint);
  std::vector<OthelloPoint> GetAllLegalMoves(Color color);
  Color GetTurn();
  Color GetOpposingColor(Color color);
  
  int GetScore(Color color);
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
};

#endif