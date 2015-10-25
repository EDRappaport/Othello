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
  int len = allLegalMoves.size();
  std::cout << allLegalMoves[0].ToString();
  int random = rand() % len;
  std::cout << "Random num: " << random << "; Len: " << len << std::endl;
  return allLegalMoves[random];
}
