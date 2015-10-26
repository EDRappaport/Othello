#ifndef _PLAYERMOVER_H
#define _PLAYERMOVER_H

#include "OthelloPoint.hpp"
#include "Board.hpp"

class PlayerMover
{
public:
  virtual OthelloPoint SelectMove(Board board) = 0;
protected:
  Color _color;
};

class ComputerPlayerMover : public PlayerMover
{
public:
  ComputerPlayerMover(Color color);
  OthelloPoint SelectMove(Board board);
private:
  OthelloPoint AlphaBetaSearch(Board board);
  int MaxValueSearch(Board board, int alpha, int beta, int maxDepth, OthelloPoint& bestMove);
  int MinValueSearch(Board board, int alpha, int beta, int maxDepth);
  int BoardHeuristic(Board board);
  
  Board newBoard = Board(BlackPlayer);
  OthelloPoint fakeOthelloPoint = OthelloPoint();
};

class PersonPlayerMover : public PlayerMover
{
public:
  PersonPlayerMover(Color color);
  OthelloPoint SelectMove(Board board);
private:
  void GetUserMove(int& row, int& column);
};

#endif