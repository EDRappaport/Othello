#include <limits> 
#include <time.h>  

#include "PlayerMover.hpp"


ComputerPlayerMover::ComputerPlayerMover(Color color, int moveTimeoutSeconds)
{
  _color = color;
  _timeout = moveTimeoutSeconds * 1000;
}

OthelloPoint ComputerPlayerMover::SelectMove(Board board)
{
  srand (time(NULL));
  std::vector<OthelloPoint> allLegalMoves = board.GetAllLegalMoves(_color);
  if (allLegalMoves.size() == 0)
  {
    return OthelloPoint();
  }
 
  return AlphaBetaSearch(board);
}

OthelloPoint ComputerPlayerMover::AlphaBetaSearch(Board board)
{
  OthelloPoint othelloPoint = OthelloPoint();
  OthelloPoint bestMove = OthelloPoint();
  bool shouldUseThisResponse; // used to rule out recommended move from incomplete iterations
  bool didFinishGame = false;
  _searchStartTime = Clock::now();
  for (int limit = 1; limit < 64; limit++)
  {
    MaxValueSearch(board, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), limit,
		   shouldUseThisResponse, false, didFinishGame, othelloPoint);
    if (shouldUseThisResponse || didFinishGame)
    {
      bestMove = othelloPoint;
      if (didFinishGame)
      {
	std::cout << "Search depth: " << "To the END! (" << limit << ")" <<std::endl;
	break;
      }
    }
    else
    {
      std::cout << "Search depth: " << limit <<std::endl;
      break; //time out, we'll use the prev best
    }
  }
  return bestMove;
}

int ComputerPlayerMover::MaxValueSearch(Board board, int alpha, int beta, int maxDepth,
					bool& didCompleteToDepth, bool calledFromEmptyMove, bool& didFinishGame,
					OthelloPoint& bestMove)
{
  if (std::chrono::duration_cast<milliseconds>(Clock::now() - _searchStartTime).count() > _timeout - 10)
  {
    didCompleteToDepth = false;
    return 0;
  }
  if (maxDepth == 0)
  {
    didCompleteToDepth = true;
    return BoardHeuristic(board);
  }
  int val = std::numeric_limits<int>::min();
  int res;
  std::vector<OthelloPoint> allLegalMoves = board.GetAllLegalMoves(_color);
  if (allLegalMoves.size() == 0)
  {
    if (calledFromEmptyMove)
    {
      // prev player had no moves, and I also have no moves....
      int nothing;
      didFinishGame = true;
      return board.GetScore(_color,nothing,nothing);
    }
    allLegalMoves.push_back(OthelloPoint());
    calledFromEmptyMove = true;
  }
  for (int i = 0; i < allLegalMoves.size(); i++)
  {
    newBoard = board.GetResultantBoard(_color, allLegalMoves[i]);
    /*std::cout << "At MAX - trying this Board with maxDepth: " << maxDepth << std::endl;
    newBoard.DisplayBoard();*/
    res = MinValueSearch(newBoard, alpha, beta, maxDepth-1, didCompleteToDepth, calledFromEmptyMove, didFinishGame);
    
    if (res > val)
    {
      val = res;
      bestMove = allLegalMoves[i];
    }
    else if(res == val)
    {
      // randomly decide btwn moves if eq.
      // in the end there will a greater prob for the earlier bestMoves to remain,
      // but since they have a predicted eq value, this is not so bad. But this can be fixed
      if (rand()%2 == 1)
      {
	bestMove = allLegalMoves[i];
      }
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

int ComputerPlayerMover::MinValueSearch(Board board, int alpha, int beta, int maxDepth,
					bool& didCompleteToDepth, bool calledFromEmptyMove, bool& didFinishGame)
{
  if (std::chrono::duration_cast<milliseconds>(Clock::now() - _searchStartTime).count() > _timeout - 10)
  {
    didCompleteToDepth = false;
    return 0;
  }
  if (maxDepth == 0)
  {
    didCompleteToDepth = true;
    return BoardHeuristic(board);
  }
  int val = std::numeric_limits<int>::max();
  int res;
  std::vector<OthelloPoint> allLegalMoves = board.GetAllLegalMoves(board.GetOpposingColor(_color));
  if (allLegalMoves.size() == 0)
  {
    if (calledFromEmptyMove)
    {
      // prev player had no moves, and I also have no moves....
      int nothing;
      didFinishGame = true;
      return board.GetScore(board.GetOpposingColor(_color),nothing,nothing);
    }
    allLegalMoves.push_back(OthelloPoint());
    calledFromEmptyMove = true;
  }
  for (int i = 0; i < allLegalMoves.size(); i++)
  {
    newBoard = board.GetResultantBoard(board.GetOpposingColor(_color), allLegalMoves[i]);
    /*std::cout << "At MIN - trying this Board with maxDepth:" << maxDepth << std::endl;
    newBoard.DisplayBoard();*/
    res = MaxValueSearch(newBoard, alpha, beta, maxDepth-1, didCompleteToDepth, calledFromEmptyMove, didFinishGame, fakeOthelloPoint);
    val = res < val ? res : val;
    if (val <= alpha) {return val;}
    beta = beta < val ? beta : val;
  }
  return val;
}

int ComputerPlayerMover::BoardHeuristic(Board board)
{
  Color enemyColor = board.GetOpposingColor(_color);
  
  int myDetailedPointScore, myFrontierCount, enemyDetailedPointScore, enemyFrontierCount;
  float myCoins = board.GetScore(_color, myDetailedPointScore, myFrontierCount); // 0 -> 64
  float enemyCoins = board.GetScore(enemyColor, enemyDetailedPointScore, enemyFrontierCount); // 0 -> 64
  float coinRatio = 100*(myCoins - enemyCoins)/(myCoins + enemyCoins); // -100 -> 100
  
  float frontierRatio;
  if (myFrontierCount + enemyFrontierCount != 0)
  {
    frontierRatio = 100* ((float) (myFrontierCount - enemyFrontierCount))/((float)(myFrontierCount + enemyFrontierCount)); // -100 -> 100
  }
  else
  {
    frontierRatio = 0;
  }
  
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
    float heuristic = 5*coinRatio + 2500*cornerRatio + 10*mobility + 20*myDetailedPointScore + 10*frontierRatio;
    //std::cout << "B heuristic " << heuristic << std::endl;
    return heuristic;
  }
  else
  {
    float heuristic = 5*coinRatio + 2500*cornerRatio + 10*mobility + 20*myDetailedPointScore;// + 10*frontierRatio;
    //std::cout << "W heuristic " << coinRatio << std::endl;
    //return 20*coinRatio + 150*cornerRatio;
    return heuristic;
  }
}

