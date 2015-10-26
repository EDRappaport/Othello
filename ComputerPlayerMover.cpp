#include <limits> 
#include <time.h>  

#include "PlayerMover.hpp"

ComputerPlayerMover::ComputerPlayerMover(Color color)
{
  _color = color;
}

OthelloPoint ComputerPlayerMover::SelectMove(Board board)
{
  srand (time(NULL));
  std::vector<OthelloPoint> allLegalMoves = board.GetAllLegalMoves(_color);
  if (allLegalMoves.size() == 0)
  {
    return OthelloPoint();
  }
  /*int len = allLegalMoves.size();
  std::cout << allLegalMoves[0].ToString();
  int random = rand() % len;
  std::cout << "Random num: " << random << "; Len: " << len << std::endl;*/
  
  return AlphaBetaSearch(board);
  
  //return allLegalMoves[random];
}

OthelloPoint ComputerPlayerMover::AlphaBetaSearch(Board board)
{
  OthelloPoint othelloPoint = OthelloPoint();
  int limit = _color == BlackPlayer ? 4 : 4;
  MaxValueSearch(board, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), limit, othelloPoint);
  return othelloPoint;
}

int ComputerPlayerMover::MaxValueSearch(Board board, int alpha, int beta, int maxDepth, OthelloPoint& bestMove)
{
  if (maxDepth == 0) {return BoardHeuristic(board);}
  int val = std::numeric_limits<int>::min();
  int res;
  std::vector<OthelloPoint> allLegalMoves = board.GetAllLegalMoves(_color);
  if (allLegalMoves.size() == 0) {allLegalMoves.push_back(OthelloPoint());}
  for (int i = 0; i < allLegalMoves.size(); i++)
  {
    newBoard = board.GetResultantBoard(_color, allLegalMoves[i]);
    /*std::cout << "At MAX - trying this Board with maxDepth: " << maxDepth << std::endl;
    newBoard.DisplayBoard();*/
    res = MinValueSearch(newBoard, alpha, beta, maxDepth-1);
    
    if (res > val)
    {
      val = res;
      bestMove = allLegalMoves[i];
    }
    else
    {
      val = val;
    }
    
    if (val >= beta) {return val;}
    alpha = alpha > val ? alpha : val;
  }
  return val;
}

int ComputerPlayerMover::MinValueSearch(Board board, int alpha, int beta, int maxDepth)
{
  if (maxDepth == 0) {return BoardHeuristic(board);}
  int val = std::numeric_limits<int>::max();
  int res;
  std::vector<OthelloPoint> allLegalMoves = board.GetAllLegalMoves(board.GetOpposingColor(_color));
  if (allLegalMoves.size() == 0) {allLegalMoves.push_back(OthelloPoint());}
  for (int i = 0; i < allLegalMoves.size(); i++)
  {
    newBoard = board.GetResultantBoard(board.GetOpposingColor(_color), allLegalMoves[i]);
    /*std::cout << "At MIN - trying this Board with maxDepth:" << maxDepth << std::endl;
    newBoard.DisplayBoard();*/
    res = MaxValueSearch(newBoard, alpha, beta, maxDepth-1, fakeOthelloPoint);
    val = res < val ? res : val;
    if (val <= alpha) {return val;}
    beta = beta < val ? beta : val;
  }
  return val;
}

int ComputerPlayerMover::BoardHeuristic(Board board)
{
  Color enemyColor = board.GetOpposingColor(_color);
  
  float myCoins = board.GetScore(_color); // 0 -> 64
  float enemyCoins = board.GetScore(enemyColor); // 0 -> 64
  float parity = 100*(myCoins - enemyCoins)/(myCoins + enemyCoins); // -100 -> 100
  
  float myNumMoves = board.GetAllLegalMoves(_color).size(); // 0 -> lt 64
  float enemyNumMoves = board.GetAllLegalMoves(enemyColor).size(); // 0 -> lt 64
  float mobility;  // -100 -> 100
  if (myNumMoves + enemyNumMoves != 0)
  {
    mobility = 100 * (myNumMoves - enemyNumMoves)/(myNumMoves + enemyNumMoves);
  }
  else
  {
    mobility = 0;
  }
  
  float myNumCorners = board.GetNumCornersOccupied(_color); // 0 -> 4
  float enemyNumCorners = board.GetNumCornersOccupied(enemyColor); // 0 -> 4
  float cornerRatio; // -100 -> 100
  if (myNumCorners + enemyNumCorners != 0)
  {
    cornerRatio = 100 * (myNumCorners - enemyNumCorners)/(myNumCorners + enemyNumCorners);
  }
  else
  {
    cornerRatio = 0;
  }
  
  if (_color == BlackPlayer)
  {
    float heuristic = 20*parity + 15*cornerRatio + 10*mobility; //-220200 -> 220200
    std::cout << "B heuristic " << heuristic << std::endl;
    return heuristic;
  }
  else
  {
    std::cout << "W heuristic " << parity << std::endl;
    return parity;
  }
}

