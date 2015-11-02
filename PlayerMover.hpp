#ifndef _PLAYERMOVER_H
#define _PLAYERMOVER_H

#include <chrono>

#include "OthelloPoint.hpp"
#include "Board.hpp"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;

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
  ComputerPlayerMover(Color color, int moveTimeoutSeconds);
  OthelloPoint SelectMove(Board board);
private:
  OthelloPoint AlphaBetaSearch(Board board);
  
  int MaxValueSearch(Board board, int alpha, int beta, int maxDepth,
		     bool& didCompleteToDepth, bool calledFromEmptyMove,
		     OthelloPoint& bestMove);
  int MinValueSearch(Board board, int alpha, int beta, int maxDepth,
		     bool& didCompleteToDepth, bool calledFromEmptyMove);
  
  int BoardHeuristic(Board board);
  
  Board newBoard = Board();
  OthelloPoint fakeOthelloPoint = OthelloPoint();
  
  Clock::time_point _searchStartTime;
  
  double _timeout;
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